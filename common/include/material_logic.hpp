#ifndef RENDER_MATERIAL_LOGIC_HPP
#define RENDER_MATERIAL_LOGIC_HPP

#include "hit_record.hpp"
#include "materials.hpp"
#include "math_utilities.hpp"
#include "ray.hpp"
#include "vector.hpp"

namespace render {

  // declaración a priori de clase RNG para lógica de aleatoriedad
  class RNG;

  // FUNCIONES ESTÁTICAS DE DISPERSIÓN

  // para material mate (no recibe r_in porque es independiente al ángulo)
  inline bool scatter_matte(hit_record const & rec, MatteMaterial const * mat,
                            color_vector & attenuation, ray & scattered, RNG & rng) {
    // generar vector aleatorio con componentes en [-1, 1]
    direction_vector random_offset = random_vec(rng);
    // sumamos la normal del objeto al offset aleatorio
    direction_vector scatter_direction = rec.normal + random_offset;

    // si es "demasiado pequeño" (usando near_zero en vector.hpp) el vector de dispersión es la
    // normal
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    } else {
      scatter_direction = unit_vector(scatter_direction);
    }

    // nuevo rayo disperso con origen en el punto de colisión y dirección previamente calculada
    scattered = ray(rec.intersect, scatter_direction);
    // atenuación determinada por atributos del material
    attenuation = color_vector(mat->reflectance_r, mat->reflectance_g, mat->reflectance_b);

    return true;
  }

  // para material metálico
  inline bool scatter_metal(ray const & r_in, hit_record const & rec, MetalMaterial const * mat,
                            color_vector & attenuation, ray & scattered, RNG & rng) {
    // difusión obtenida de los atributos del material
    double diffusion          = mat->diffusion;
    direction_vector unit_dir = unit_vector(r_in.dir);

    // dirección de reflexión inicial d_1r
    direction_vector reflected = unit_dir - 2 * dot(unit_dir, rec.normal) * rec.normal;

    // vector de difusión phi: aleatorio entre [-diffusion, diffusion]
    direction_vector phi_diffusion = random_vec(rng, -diffusion, diffusion);

    // dirección final: dr = (d_1r normalizado) + phi_diffusion
    direction_vector scattered_direction = unit_vector(reflected) + phi_diffusion;

    // nuevo rayo disperso con origen en el punto de colisión y dirección previamente calculada
    scattered = ray(rec.intersect, scattered_direction);
    // atenuación determinada por atributos del material
    attenuation = color_vector(mat->reflectance_r, mat->reflectance_g, mat->reflectance_b);

    // solo dispersar si el rayo no se dispersa hacia dentro
    return dot(scattered.dir, rec.normal) > 0;
  }

  // para material refractivo (no utiliza la lógica de rng)
  inline bool scatter_refractive(ray const & r_in, hit_record const & rec,
                                 RefractiveMaterial const * mat, color_vector & attenuation,
                                 ray & scattered) {
    // índice de refracción obtenido de los atributos del material
    double ir = mat->refractive_index;

    // reflectancia resultante SIEMPRE (1, 1, 1)
    attenuation = color_vector(1.0, 1.0, 1.0);

    // RELACIÓN DE ÍNDICES DE REFRACCIÓN

    // Se usa 1.0 (aire) si el rayo viene de fuera (front_face=true en hit_record)
    double ir_ratio = rec.front_face ? (1.0 / ir) : ir;

    // cálculo de ángulos
    direction_vector unit_direction = unit_vector(r_in.dir);
    double cos_theta                = std::min(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta                = std::sqrt(1.0 - cos_theta * cos_theta);

    // cálculo de p' por sin de theta para dos posibilidades
    bool cannot_refract = ir_ratio * sin_theta > 1.0;
    direction_vector direction;

    // si es mayor que 1
    if (cannot_refract) {
      direction = unit_direction - 2 * dot(unit_direction, rec.normal) * rec.normal;
    }
    // si es menor que 1
    else
    {
      // la dirección se obtiene a partir de la suma de u y v
      // u
      direction_vector r_out_perp = ir_ratio * (unit_direction + cos_theta * rec.normal);
      // v
      double discriminant             = 1.0 - r_out_perp.magnitude_squared();
      direction_vector r_out_parallel = -std::sqrt(std::abs(discriminant)) * rec.normal;
      // suma
      direction = r_out_perp + r_out_parallel;
    }

    // nuevo rayo disperso con origen en el punto de colisión y dirección previamente calculada
    scattered = ray(rec.intersect, direction);
    // de nuevo reflectancia siempre es (1, 1, 1)
    return true;
  }

  // FUNCIÓN GENERAL PARA EL CÁLCULO DE RAYO DISPERSO
  inline bool scatter(ray const & r_in, hit_record const & rec, color_vector & attenuation,
                      ray & scattered, RNG & rng) {
    // se castea el puntero base al puntero concreto, leyendo el tipo de material (type)
    MaterialBase const * base_mat = rec.mat_pointer;

    // verificación de que el objeto tiene tipo de material
    if (!base_mat) {
      return false;
    }

    // selección de la función lógica a través de un switch según el tipo
    switch (base_mat->type) {
      case MATTE_TYPE:
        return scatter_matte(rec, dynamic_cast<MatteMaterial const *>(base_mat), attenuation,
                             scattered, rng);
      case METAL_TYPE:
        return scatter_metal(r_in, rec, dynamic_cast<MetalMaterial const *>(base_mat), attenuation,
                             scattered, rng);
      case REFRACTIVE_TYPE:
        return scatter_refractive(r_in, rec, dynamic_cast<RefractiveMaterial const *>(base_mat),
                                  attenuation, scattered);
      default: return false;
    }
  }

}  // namespace render

#endif
