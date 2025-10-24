#ifndef AOS_VECTOR_HPP
#define AOS_VECTOR_HPP

#include <cmath>
#include <iostream>

namespace aos {

  class Vector {
  public:
    // Constructores
    Vector();
    Vector(double cx, double cy, double cz);

    // Getters
    [[nodiscard]] double get_x() const;
    [[nodiscard]] double get_y() const;
    [[nodiscard]] double get_z() const;
    [[nodiscard]] double r() const;
    [[nodiscard]] double g() const;
    [[nodiscard]] double b() const;

    // Operadores
    Vector operator-() const;
    double operator[](int i) const;
    double & operator[](int i);
    Vector & operator+=(Vector const & v);
    Vector & operator-=(Vector const & v);
    Vector & operator*=(double t);
    Vector & operator/=(double t);

    // Magnitudes
    [[nodiscard]] double magnitude_squared() const;
    [[nodiscard]] double magnitude() const;
    [[nodiscard]] bool near_zero() const;

  private:
    double x, y, z;
  };

  // Alias
  using PointVector     = Vector;
  using ColorVector     = Vector;
  using DirectionVector = Vector;
  using NormalVector    = Vector;

  // Declaraciones de funciones (implementación en .cpp)
  std::ostream & operator<<(std::ostream & out, Vector const & v);
  Vector operator+(Vector const & u, Vector const & v);
  Vector operator-(Vector const & u, Vector const & v);
  Vector operator*(Vector const & u, Vector const & v);
  Vector operator*(double t, Vector const & v);
  Vector operator*(Vector const & v, double t);
  Vector operator/(Vector const & v, double t);
  double dot(Vector const & u, Vector const & v);
  Vector cross(Vector const & u, Vector const & v);
  Vector unit_vector(Vector const & v);

}  // namespace aos

#endif  // AOS_VECTOR_HPP
