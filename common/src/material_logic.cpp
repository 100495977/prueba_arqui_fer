#include "../include/material_logic.hpp"
#include "../include/hit_record.hpp"
#include "../include/materials.hpp"
#include "../include/math_utilities.hpp"
#include "../include/ray.hpp"
#include "../include/vector.hpp"

#include <cmath>

namespace render {

  // lógica de dispersión material mate
  // calcula un rayo dispersado con dirección aleatoria alrededor de la normal y la atenuación
  // asociada al material
  bool scatter_matte(hit_record const & rec, MatteMaterial const * mat, ScatterIO & io) {
    direction_vector random_offset     = random_vec(*io.rng);
    direction_vector scatter_direction = rec.normal + random_offset;
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    } else {
      scatter_direction = unit_vector(scatter_direction);
    }
    *io.scattered   = ray(rec.intersect, scatter_direction);
    *io.attenuation = color_vector(mat->reflectance_r, mat->reflectance_g, mat->reflectance_b);
    return true;
  }

  // lógica de dispersión material metálico
  // refleja el rayo de entrada con un término de difusión aleatorio controlado por el parámetro
  // 'diffusion' del material
  bool scatter_metal(ray const & r_in, hit_record const & rec, MetalMaterial const * mat,
                     ScatterIO & io) {
    double diffusion                     = mat->diffusion;
    direction_vector unit_dir            = unit_vector(r_in.dir);
    direction_vector reflected           = unit_dir - 2.0 * dot(unit_dir, rec.normal) * rec.normal;
    direction_vector phi_diffusion       = random_vec(*io.rng, -diffusion, diffusion);
    direction_vector scattered_direction = unit_vector(reflected + phi_diffusion);
    *io.scattered                        = ray(rec.intersect, scattered_direction);
    *io.attenuation = color_vector(mat->reflectance_r, mat->reflectance_g, mat->reflectance_b);
    return dot(io.scattered->dir, rec.normal) > 0.0;
  }

  // lógica de dispersión material refractivo
  // calcula refracción (o reflexión en caso de TIR) según el índice de refracción del material y
  // rellena el rayo dispersado y la atenuación
  bool scatter_refractive(ray const & r_in, hit_record const & rec, RefractiveMaterial const * mat,
                          ScatterIO & io) {
    double ir                       = mat->refractive_index;
    *io.attenuation                 = color_vector(1.0, 1.0, 1.0);
    double ir_ratio                 = rec.front_face ? (1.0 / ir) : ir;
    direction_vector unit_direction = unit_vector(r_in.dir);
    double cos_theta                = std::min(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta                = std::sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract             = ir_ratio * sin_theta > 1.0;
    direction_vector direction;
    if (cannot_refract) {
      direction = unit_direction - 2.0 * dot(unit_direction, rec.normal) * rec.normal;
    } else {
      direction_vector r_out_perp     = ir_ratio * (unit_direction + cos_theta * rec.normal);
      double discriminant             = 1.0 - r_out_perp.magnitude_squared();
      direction_vector r_out_parallel = -std::sqrt(std::abs(discriminant)) * rec.normal;
      direction                       = r_out_perp + r_out_parallel;
    }
    *io.scattered = ray(rec.intersect, direction);
    return true;
  }

  // elección de dispersión
  // selecciona la función de scatter adecuada en función del tipo de material almacenado en
  // 'rec.mat_pointer'
  bool scatter(ray const & r_in, hit_record const & rec, ScatterIO & io) {
    MaterialBase const * base_mat = rec.mat_pointer;
    if (base_mat == nullptr) {
      return false;
    }
    switch (base_mat->type) {
      case MATTE_TYPE: return scatter_matte(rec, dynamic_cast<MatteMaterial const *>(base_mat), io);
      case METAL_TYPE:
        return scatter_metal(r_in, rec, dynamic_cast<MetalMaterial const *>(base_mat), io);
      case REFRACTIVE_TYPE:
        return scatter_refractive(r_in, rec, dynamic_cast<RefractiveMaterial const *>(base_mat),
                                  io);
      default: return false;
    }
  }

}  // namespace render
