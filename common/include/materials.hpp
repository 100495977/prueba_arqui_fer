#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "material_base.hpp"
#include <string>

// Material mate - reflectancia RGB[2]
struct MatteMaterial : public MaterialBase {
  MaterialType type = MATTE_TYPE;
  std::string name;
  double reflectance_r, reflectance_g, reflectance_b;
};

// Material metálico - reflectancia + difusión [2]
struct MetalMaterial : public MaterialBase {
  MaterialType type = METAL_TYPE;
  std::string name;
  double reflectance_r, reflectance_g, reflectance_b;
  double diffusion;
};

// Material refractivo - índice de refracción[2]
struct RefractiveMaterial : public MaterialBase {
  MaterialType type = REFRACTIVE_TYPE;
  std::string name;
  double refractive_index;
};

#endif  // MATERIALS_HPP
