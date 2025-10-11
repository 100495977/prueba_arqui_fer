#ifndef SCENE_PARSER_HPP
#define SCENE_PARSER_HPP

#include <vector>
#include <string>
#include "materials.hpp"
#include "objects.hpp"


bool parse_scene(const std::string &filename, 
                  std::vector<MatteMaterial> &matte_materials,
                  std::vector<MetalMaterial> &metal_materials,
                  std::vector<RefractiveMaterial> &refractive_materials,
                  std::vector<Sphere> &spheres,
                  std::vector<Cylinder> &cylinders);


#endif // SCENE_PARSER_HPP


//Falta implementar comprobación que los objetos referencian materiales ya definidos
