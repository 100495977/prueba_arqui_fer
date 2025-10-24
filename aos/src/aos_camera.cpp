#include "../include/aos_camera.hpp"
#include <cmath>

namespace aos {

  // El constructor ya está implementado en el header, pero se podría mover aquí aquí:
  /*
  Camera::Camera(ConfigParams const & config) {
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

  Ray Camera::get_ray(double s, double t) const {
      DirectionVector dir = lower_left_corner + s * horizontal + t * vertical - origin;
      return {origin, unit_vector(dir)};
  }
  */

  // Por ahora este archivo puede estar vacío ya que todo está en el header
  // Pero lo creamos para mantener la estructura consistente

}  // namespace aos
