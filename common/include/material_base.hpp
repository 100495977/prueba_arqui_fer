#ifndef RENDER_MATERIAL_BASE_HPP
#define RENDER_MATERIAL_BASE_HPP

#include <string>

namespace render {

  // ENUM QUE IDENTIFICA EL TIPO DE MATERIAL (necesario para el dispatching switch)
  enum MaterialType { MATTE_TYPE, METAL_TYPE, REFRACTIVE_TYPE };

  // ESTRUCTURA QUE DEFINE EL TIPO DE MATERIAL SEGÚN EL ENUM
  struct MaterialBase {
    virtual ~MaterialBase() = default;
    MaterialType type;
  };

}  // namespace render

#endif  // RENDER_MATERIAL_BASE_HPP
