#include "../include/aos_vector.hpp"

namespace aos {

  // Constructores
  Vector::Vector() : x{0}, y{0}, z{0} { }

  Vector::Vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} { }

  // Getters
  double Vector::get_x() const {
    return x;
  }

  double Vector::get_y() const {
    return y;
  }

  double Vector::get_z() const {
    return z;
  }

  double Vector::r() const {
    return x;
  }

  double Vector::g() const {
    return y;
  }

  double Vector::b() const {
    return z;
  }

  // Operadores
  Vector Vector::operator-() const {
    return {-x, -y, -z};
  }

  double Vector::operator[](int i) const {
    switch (i) {
      case 0:  return x;
      case 1:  return y;
      case 2:  return z;
      default: return 0.0;
    }
  }

  double & Vector::operator[](int i) {
    switch (i) {
      case 0:  return x;
      case 1:  return y;
      case 2:  return z;
      default: return x;
    }
  }

  Vector & Vector::operator+=(Vector const & v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector & Vector::operator-=(Vector const & v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  Vector & Vector::operator*=(double t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  Vector & Vector::operator/=(double t) {
    x /= t;
    y /= t;
    z /= t;
    return *this;
  }

  // Magnitudes
  double Vector::magnitude_squared() const {
    return x * x + y * y + z * z;
  }

  double Vector::magnitude() const {
    return std::sqrt(magnitude_squared());
  }

  bool Vector::near_zero() const {
    auto const s = 1e-8;
    return (std::abs(x) < s) and (std::abs(y) < s) and (std::abs(z) < s);
  }

  // Funciones globales (implementación)
  std::ostream & operator<<(std::ostream & out, Vector const & v) {
    return out << v.get_x() << ' ' << v.get_y() << ' ' << v.get_z();
  }

  Vector operator+(Vector const & u, Vector const & v) {
    return {u.get_x() + v.get_x(), u.get_y() + v.get_y(), u.get_z() + v.get_z()};
  }

  Vector operator-(Vector const & u, Vector const & v) {
    return {u.get_x() - v.get_x(), u.get_y() - v.get_y(), u.get_z() - v.get_z()};
  }

  Vector operator*(Vector const & u, Vector const & v) {
    return {u.get_x() * v.get_x(), u.get_y() * v.get_y(), u.get_z() * v.get_z()};
  }

  Vector operator*(double t, Vector const & v) {
    return {t * v.get_x(), t * v.get_y(), t * v.get_z()};
  }

  Vector operator*(Vector const & v, double t) {
    return t * v;
  }

  Vector operator/(Vector const & v, double t) {
    return (1 / t) * v;
  }

  double dot(Vector const & u, Vector const & v) {
    return u.get_x() * v.get_x() + u.get_y() * v.get_y() + u.get_z() * v.get_z();
  }

  Vector cross(Vector const & u, Vector const & v) {
    return {u.get_y() * v.get_z() - u.get_z() * v.get_y(),
            u.get_z() * v.get_x() - u.get_x() * v.get_z(),
            u.get_x() * v.get_y() - u.get_y() * v.get_x()};
  }

  Vector unit_vector(Vector const & v) {
    return v / v.magnitude();
  }

}  // namespace aos
