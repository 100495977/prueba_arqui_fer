#ifndef AOS_CAMERA_HPP
#define AOS_CAMERA_HPP

// Minimal ConfigParams to avoid requiring external config.hpp; adjust fields as needed.
namespace aos {

  struct ConfigParams {
    double camera_x, camera_y, camera_z;
    double target_x, target_y, target_z;
    double north_x, north_y, north_z;
    double field_of_view;  // degrees
    int aspect_width;
    int aspect_height;
  };

}  // namespace aos

#include "aos_ray.hpp"
#include "aos_vector.hpp"
#include <cmath>

namespace aos {

  // Constante para convertir grados a radianes
  constexpr double DEGREES_TO_RADIANS = M_PI / 180.0;

  class Camera {
  public:
    // Constructor que se inicializa a partir de los parámetros de configuración
    Camera(ConfigParams const & config) {
      // Posición y orientación de la cámara
      auto lookfrom = PointVector(config.camera_x, config.camera_y, config.camera_z);
      auto lookat   = PointVector(config.target_x, config.target_y, config.target_z);
      auto vup      = DirectionVector(config.north_x, config.north_y, config.north_z);

      // Parámetros de la imagen
      auto vfov         = config.field_of_view * DEGREES_TO_RADIANS;
      auto h            = std::tan(vfov / 2.0);
      auto viewport_h   = 2.0 * h;
      auto aspect_ratio = static_cast<double>(config.aspect_width) / config.aspect_height;
      auto viewport_w   = aspect_ratio * viewport_h;

      // Cálculo de la base ortonormal de la cámara (ejes W, U, V)
      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);

      // Configuración final
      origin            = lookfrom;
      horizontal        = viewport_w * u;
      vertical          = viewport_h * v;
      lower_left_corner = origin - (horizontal / 2.0) - (vertical / 2.0) - w;
    }

    // Genera un rayo para un píxel (s, t) en la pantalla
    [[nodiscard]] Ray get_ray(double s, double t) const {
      DirectionVector dir = lower_left_corner + s * horizontal + t * vertical - origin;
      return {origin, unit_vector(dir)};
    }

  private:
    PointVector origin;
    PointVector lower_left_corner;
    DirectionVector horizontal;
    DirectionVector vertical;
    DirectionVector u, v, w;  // Base ortonormal de la cámara
  };

}  // namespace aos

#endif  // AOS_CAMERA_HPP
