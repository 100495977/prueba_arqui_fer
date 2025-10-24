#ifndef AOS_RAY_HPP
#define AOS_RAY_HPP

#include "aos_vector.hpp"

namespace aos {

  class Ray {
  public:
    // Constructores
    Ray();
    Ray(PointVector const & origin, DirectionVector const & direction);

    // Getters
    [[nodiscard]] PointVector get_origin() const;
    [[nodiscard]] DirectionVector get_direction() const;

    // Funciones
    [[nodiscard]] PointVector at(double t) const;

  private:
    PointVector orig;
    DirectionVector dir;
  };

}  // namespace aos

#endif  // AOS_RAY_HPP
