#ifndef RENDER_OBJECT_BASE_HPP
#define RENDER_OBJECT_BASE_HPP

#include <string>

namespace render {

  // ENUM QUE IDENTIFICA EL TIPO DE OBJETO (necesario para el dispatching switch)
  enum ObjectType { SPHERE_TYPE, CYLINDER_TYPE };

  // ESTRUCTURA QUE DEFINE EL TIPO DE OBJETO SEGÚN EL ENUM
  struct ObjectBase {
    virtual ~ObjectBase() = default;
    ObjectType type;
  };

}  // namespace render

#endif
