#include "../include/aos_ray.hpp"

namespace aos {

  // Constructores
  Ray::Ray() : orig(0, 0, 0), dir(0, 0, 0) { }

  Ray::Ray(PointVector const & origin, DirectionVector const & direction)
      : orig(origin), dir(direction) { }

  // Getters
  PointVector Ray::get_origin() const {
    return orig;
  }

  DirectionVector Ray::get_direction() const {
    return dir;
  }

  // Funciones
  PointVector Ray::at(double t) const {
    return orig + t * dir;
  }

}  // namespace aos
