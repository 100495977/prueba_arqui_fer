#include "scene_parser.hpp"
#include "parser_utils.hpp"
#include <fstream>
#include <iostream>
#include <unordered_set>

bool parse_scene_file(const std::string& filename, 
                    std::vector<MateMaterial>& mate_materials,
                    std::vector<MetalMaterial>& metal_materials,
                    std::vector<RefractiveMaterial>& refractive_materials,
                    std::vector<Sphere>& spheres,
                    std::vector<Cylinder>& cylinders) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        print_error("Cannot open scene file: " + filename);
        return false;
    }

    std::string line;
    std::unordered_set<std::string> material_names; //Detectamos duplicados

    while (std::getline(file, line)) {
        if (is_empty_line(line)) {
            continue;
        }

        auto tokens = split_line(line);
        if (tokens.empty()) continue;

        std::string type_with_colon = tokens[0];
        if (type_with_colon.back() != ":") {
            print_error("Unknown scene entity " + type_with_colon.substr(0, type_with_colon.find(':')));
            return false;
        }

        std::string type = type_with_colon.substr(0, type_with_colon.length() - 1);

        try {
            auto check_excess = [&](const std::string& entity_type, size_t expected_size) {
                if (tokens.size() > expected_size) {
                    std::string extra;
                    for (size_t i = expected_size; i < tokens.size(); ++i) {
                        extra += tokens[i];
                        if (i < tokens.size() - 1) extra += " ";
                    }
                    print_error("Extra data after configuration value for key " + entity_type + "\nExtra " + extra + "\nLine: " + line);
                    return false;
                }

                return true;
            };

            //Materiales
            if (type == "matte") {
                if (tokens.size() < 5) {
                    print_error("Invalid matte material parameters\nLine: " + line);
                    return false;
                }
                if (!check_excess("matte", 5)) return false;

                std::string name = tokens[1];
                if (material_names.count(name)) {
                    print_error("Material with name " + name + "already exists\nLine: " + line);
                    return false;
                }

                double r = std::stod(tokens[2]);
                double g = std::stod(tokens[3]);
                double b = std::stod(tokens[4]);

                if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
                    print_error("Invalid matte material parameters\nLine: " + line);
                    return false;
                }

                mate_materials.push_back({name, r, g, b});
                material_names.insert(name);

            }

            else if (type == "metal") {
                if (tokens.size() < 6) {
                    print_error("Invalid metal material parameters\nLine: " + line);
                    return false;
                }
                if (!check_excess("metal", 6)) return false;

                std::string name = tokens[1];
                if (material_names.count(name)) {
                    print_error("Material with name " + name + "already exists\nLine: " + line);
                    return false;
                }

                double r = std::stod(tokens[2]);
                double g = std::stod(tokens[3]);
                double b = std::stod(tokens[4]);
                double diffusion = std::stod(tokens[5]);

                if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1 || diffusion <= 0) {
                    print_error("Invalid metal material parameters\nLine: " + line);
                    return false;
                }

                metal_materials.push_back({name, r, g, g, diffusion});
                material_names.insert(name);
            }

            else if (type == "refractive") {
                if (tokens.size() < 3){
                    print_error("Invalid refractive material parameters\nLine: " + line);
                    return false;
                }
                if (!check_excess("refractive", 3)) return false;

                std::string name = tokens[1];
                if (material_names.count(name)) {
                    print_error("Material with name " + name + "already exists\nLine: " + line);
                    return false;
                }

                double refraction = std::stod(tokens[2]);
                if (refraction <= 0) {
                    print_error("Invalid refractive material parameters\nLine: " + line);
                    return false;
                }

                refractive_materials.push_back({name, refraction});
                material_names.insert(name);
            }

            else if (type == "sphere") {
                if (tokens.size() < 6) {
                    print_error("Invalid sphere parameters\nLine: " + line);
                    return false;
                }

                if (!check_excess("sphere", 6)) return false;

                double x = std::stod(tokens[1]);
                double y = std::stod(tokens[2]);
                double z = std::stod(tokens[3]);
                double radius = std::stod(tokens[4]);
                std::string material = tokens[5];

                if (radius <= 0) {
                    print_error("Invalid sphere parameters\nLine: " + line);
                    return false;
                }

                if (!material_names.count(material)) {
                    print_error("Material not found " + material + "\nLine: " + line);
                    return false;
                }

                spheres.push_back({x, y, z, radius, material});
            }

            else if (type == "cylinder") {
                if (tokens.size() < 9) {
                    print_error("Invalid cylinder parameters\nLine: " + line);
                    return false;
                }
                if (!check_excess("cylinder", 9)) return false;

                double x = std::stod(tokens[1]);
                double y = std::stod(tokens[2]);
                double z = std::stod(tokens[3]);
                double radius = std::stod(tokens[4]);
                double ax = std::stod(tokens[5]);
                double ay = std::stod(tokens[6]);
                double az = std::stod(tokens[7]);
                std::string material = tokens[8];

                if (radius <= 0) {
                    print_error("Invalid cylinder parameters\nLine: " + line);
                    return false;
                }

                if (!material_names.count(material)) {
                    print_error("Material not found " + material + "\nLine: " + line);
                    return false;
                }

                cylinders.push_back({x, y, z, radius, ax, ay, az, material});
                
            }

            else {
                print_error("Unknown scene entity " + type);
                return false;
            }
        }

        catch (const std::exception& e) {
            if (type == "matte") {
                print_error("Invalid matter material parameters\nLine: " + line);
            } else if (type == "metal") {
                print_error("Invalid metal material parameters\nLine: " + line);
            } else if (type == "refractive") {
                print_error("Invalid refractive material parameters\nLine: " + line);
            } else if (type == "sphere") {
                print_error("Invalid sphere parameters\nLine: " + line);
            } else if (type == "cylinder") {
                print_error("Invalid cylinder parameters\nLine: " + line);
            } else {
                print_error("Invalid parameters\nLine: " + line);
            }
            return false;
        }
    }

    return true;

}