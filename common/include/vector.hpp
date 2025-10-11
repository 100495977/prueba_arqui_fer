#ifndef RENDER_VECTOR_HPP
#define RENDER_VECTOR_HPP

#include <iostream>
#include <cmath>

namespace render {

  // CLASE QUE DEFINE A UN VECTOR
  class vector {
  public:
    // constructores
    vector(): x{0}, y{0}, z{0} {}
    vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} {}

    // DEFINICION DE FUNCIONES (las dos devuelven x, y o z por valor pero diferenciamos por semántica)

    // devuelve posición
    [[nodiscard]] double get_x() const { return x; }
    [[nodiscard]] double get_y() const { return y; }
    [[nodiscard]] double get_z() const { return z; }
    
    // devuelve color
    [[nodiscard]] double r() const { return x; }
    [[nodiscard]] double g() const { return y; }
    [[nodiscard]] double b() const { return z; }

    // OPERADORES DE VECTORES

    // cálculo del inverso
    vector operator-() const { return vector(-x, -y, -z); }
    // lectura de una posición del vector
    double operator[](int i) const;
    // modificación de una posición del vector
    double& operator[](int i);

    // operadores de asignación compuesta
    // suma de vectores (componentes uno a uno)
    vector& operator+=(const vector& v);
    // resta de vectores (componentes uno a uno)
    vector& operator-=(const vector& v);
    // producto por un escalar t
    vector& operator*=(double t);
    // división por un escalar t
    vector& operator/=(double t);

    // MAGNITUDES

    // distancia cuadrado
    [[nodiscard]] double magnitude_squared() const {return x*x + y*y + z*z;}
    // distancia
    [[nodiscard]] double magnitude() const {return std::sqrt(magnitude_squared());}


     // UTILIDAD NUMÉRICA (para estabilidad, Sección 3.5.1)
    [[nodiscard]] bool near_zero() const {const auto s = 1e-8; return (std::abs(x) < s) && (std::abs(y) < s) && (std::abs(z) < s);}

  private:
    double x, y, z;
  };

  // DECLARACIÓN de alias

  // para posición
  using point_vector = vector;
  // para color
  using color_vector = vector;
  // para dirección
  using direction_vector = vector;
  // vector normal
  using normal_vector = vector;

  // UTILIDADES GLOBALES
  // están definidas como inline para que se inserten directamente en la llamada. No cambian el valor como las de asignación compuesto

  // imprime valores de un vector directamente al stream out separados por espacios
  inline std::ostream& operator<<(std::ostream& out, const vector& v) {return out << v.get_x() << ' ' << v.get_y() << ' ' << v.get_z();}
  // suma de dos vectores
  inline vector operator+(const vector& u, const vector& v) {return vector(u.get_x() + v.get_x(), u.get_y() + v.get_y(), u.get_z() + v.get_z());}
  // resta de dos vectores
  inline vector operator-(const vector& u, const vector& v) {return vector(u.get_x() - v.get_x(), u.get_y() - v.get_y(), u.get_z() - v.get_z());}
  // producto hadamard (elemento a elemento, usado para el color)
  inline vector operator*(const vector& u, const vector& v) {return vector(u.get_x() * v.get_x(), u.get_y() * v.get_y(), u.get_z() * v.get_z());}
  // producto por un escalar t (primero t*v luego v*t)
  inline vector operator*(double t, const vector& v) {return vector(t * v.get_x(), t * v.get_y(), t * v.get_z());}
  inline vector operator*(const vector& v, double t) {return t * v;}
  // división por un escalar t
  inline vector operator/(const vector& v, double t) {return (1/t) * v;}
  // dot product (multiplica cada posición del vector y suma)
  inline double dot(const vector& u, const vector& v) {
        return u.get_x() * v.get_x()
             + u.get_y() * v.get_y()
             + u.get_z() * v.get_z();}
  // cross product (devuelve el vector perpendicular a los dados)
  inline vector cross(const vector& u, const vector& v) {
        return vector(u.get_y() * v.get_z() - u.get_z() * v.get_y(),
                      u.get_z() * v.get_x() - u.get_x() * v.get_z(),
                      u.get_x() * v.get_y() - u.get_y() * v.get_x());}
  // normalización
  inline vector unit_vector(const vector& v) {
        return v / v.magnitude();
    }

}  // namespace render

#endif
