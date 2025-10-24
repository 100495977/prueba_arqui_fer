#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "object_base.hpp"
#include <string>

// Estructura de una esfera: Centro (x, y, z), radio y material[2]
struct Sphere : public render::ObjectBase {
  render::ObjectType type = render::SPHERE_TYPE;
  double center_x, center_y, center_z;
  double radius;
  std::string material_name;
};

// Estructura de un cilindro: Centro (x, y, z), radio, eje y material[2]
struct Cylinder : public render::ObjectBase {
  render::ObjectType type = render::CYLINDER_TYPE;
  double center_x, center_y, center_z;
  double radius;
  double axis_x, axis_y, axis_z;
  double height = 2.0;  // altura total del cilindro (valor por defecto 2.0 => half-height 1.0)
  std::string material_name;
};

#endif  // OBJECTS_HPP
