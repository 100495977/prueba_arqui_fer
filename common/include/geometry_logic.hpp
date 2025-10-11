#ifndef RENDER_GEOMETRY_LOGIC_HPP
#define RENDER_GEOMETRY_LOGIC_HPP

#include "hit_record.hpp"
#include "math_utilities.hpp"
#include "objects.hpp"
#include "ray.hpp"

namespace render {

  // FUNCIONES DE COLISIÓN ESPECÍFICAS

  // para esfera
  inline bool hit_sphere(ray const & r, double t_min, double t_max, hit_record & rec,
                         Sphere const * sph) {
    // la fórmula de la esfera es: t^2*dot(D,D) + 2t*dot(D, O-C) + dot(O-C, O-C) - R^2 = 0

    // vector O - C (origen del rayo - centro de la esfera)
    point_vector center_vec(sph->center_x, sph->center_y, sph->center_z);
    direction_vector oc = r.orig - center_vec;

    // componentes de la ecuación cuadrática Ax^2 + Bx + C = 0
    auto a      = r.dir.magnitude_squared();
    auto half_b = dot(oc, r.dir);
    auto c      = oc.magnitude_squared() - sph->radius * sph->radius;

    auto discriminant = half_b * half_b - a * c;

    // caso 1: no hay colisiones
    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    // usamos una variable local para encontrar el t más cercano
    double closest_t  = t_max;
    bool hit_anything = false;

    // probamos la primera raíz (la más cercana: -B - sqrtD)
    auto root1 = (-half_b - sqrtd) / a;
    if (root1 > t_min && root1 < closest_t) {
      closest_t    = root1;
      hit_anything = true;
    }

    // probamos la segunda raíz (la más lejana: -B + sqrtD)
    auto root2 = (-half_b + sqrtd) / a;
    // usamos 'closest_t' actualizado
    if (root2 > t_min && root2 < closest_t) {
      closest_t    = root2;
      hit_anything = true;
    }

    if (!hit_anything) {
      return false;
    }

    // si encontramos un hit válido:
    rec.t         = closest_t;
    rec.intersect = r.at(rec.t);
  }

  // para cilindro
  inline bool hit_cylinder(ray const & r, double t_min, double t_max, hit_record & rec,
                           Cylinder const * cyl) {
    // usamos una variable local para guardar la colisión más cercana del cilindro
    double current_t_max = t_max;
    bool hit_anything    = false;

    //
    point_vector center(cyl->center_x, cyl->center_y, cyl->center_z);
    direction_vector axis = unit_vector(direction_vector(cyl->axis_x, cyl->axis_y, cyl->axis_z));
    double radius         = cyl->radius;

    // ASUNCIÓN TEMPORAL
    double const HALF_HEIGHT = 1.0;

    // origen del rayo respecto al centro del cilindro
    direction_vector oc = r.orig - center;

    // proyecciones del rayo y origen en el eje del cilindro
    double a_dot_d  = dot(axis, r.dir);
    double a_dot_oc = dot(axis, oc);

    // INTERSECCIÓN CON SUPERFICIE LATERAL

    // componentes de la ecuación cuadrática A*t^2 + B*t + C = 0
    auto A = r.dir.magnitude_squared() - a_dot_d * a_dot_d;
    auto B = 2.0 * (dot(r.dir, oc) - a_dot_d * a_dot_oc);
    auto C = oc.magnitude_squared() - a_dot_oc * a_dot_oc - radius * radius;

    auto discriminant = B * B - 4.0 * A * C;

    if (discriminant >= 0) {
      auto sqrtd = std::sqrt(discriminant);

      // probamos ambas raíces (la más pequeña primero)
      double t_solutions[2] = {(-B - sqrtd) / (2.0 * A), (-B + sqrtd) / (2.0 * A)};

      for (double t_side : t_solutions) {
        if (t_side > t_min && t_side < current_t_max) {
          point_vector hit_point  = r.at(t_side);
          direction_vector hit_oc = hit_point - center;

          // verificar si el punto de impacto está dentro de la altura del cilindro
          // altura axial
          double hit_height = dot(hit_oc, axis);

          // colisión lateral válida y es la más cercana hasta ahora
          if (std::fabs(hit_height) <= HALF_HEIGHT) {
            // actualizar el límite de búsqueda
            current_t_max = t_side;
            hit_anything  = true;

            // registrar el impacto
            rec.t         = t_side;
            rec.intersect = hit_point;

            // normal del cilindro lateral: (P - C) - (h * A) / R
            direction_vector p_minus_c           = rec.intersect - center;
            direction_vector normal_unnormalized = p_minus_c - (hit_height * axis);
            direction_vector outward_normal      = unit_vector(normal_unnormalized);

            // configura normal y front_face
            rec.set_face_normal(r, outward_normal);
          }
        }
      }
    }

    // INTERSECCIÓN CON LAS TAPAS

    // los planos de las tapas son perpendiculares al eje (N=axis o N=-axis)
    // la fórmula de colisión rayo-plano: t = dot(P_cap - O, N) / dot(D, N)

    std::pair<point_vector, direction_vector> caps[2] = {
      // tapa superior
      {center + (HALF_HEIGHT * axis),  axis},
      // tapa inferior
      {center - (HALF_HEIGHT * axis), -axis}
    };

    for (auto const & cap : caps) {
      point_vector P_cap     = cap.first;
      direction_vector N_cap = cap.second;

      double d_dot_n = dot(r.dir, N_cap);

      // si el rayo es paralelo al plano (d_dot_n es ~0), saltar
      if (std::fabs(d_dot_n) < 1e-8) {
        continue;
      }

      double t_cap = dot(P_cap - r.orig, N_cap) / d_dot_n;

      // si el hit está en el rango [t_min, current_t_max]
      if (t_cap > t_min && t_cap < current_t_max) {
        point_vector hit_point = r.at(t_cap);

        // verificar si el punto de impacto está dentro del radio de la tapa (radio test)
        direction_vector vec_to_center = hit_point - P_cap;

        // colisión válida con la tapa, y es la más cercana hasta ahora
        if (vec_to_center.magnitude_squared() <= radius * radius) {
          // actualizar el límite de búsqueda
          current_t_max = t_cap;
          hit_anything  = true;

          // registrar el impacto
          rec.t         = t_cap;
          rec.intersect = hit_point;

          // configura normal y front_face
          rec.set_face_normal(r, N_cap);
        }
      }
    }

    return hit_anything;
  }

  inline bool hit_object(ray const & r, double t_min, double t_max, hit_record & rec,
                         ObjectBase const * obj) {
    // selección de la función lógica a través de un switch según el tipo
    switch (obj->type) {
      case SPHERE_TYPE: return hit_sphere(r, t_min, t_max, rec, dynamic_cast<Sphere const *>(obj));
      case CYLINDER_TYPE:
        return hit_cylinder(r, t_min, t_max, rec, dynamic_cast<Cylinder const *>(obj));
        return false;
      default: return false;
    }
  }

}  // namespace render

#endif  // RENDER_GEOMETRY_LOGIC_HPP
