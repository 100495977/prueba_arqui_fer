#include "config_parser.hpp"
#include "parser_utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

bool parse_config_file(const std::string& filename, ConfigParams& config) {
    std::ifstream file(filename);
    if (!file.is_open()){
        print_error("Cannot open config file:" + filename);
        return false;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (is_empty_line(line)) {
            continue;
        }

        auto tokens = split_line(line);
        if (tokens.empty()) continue;

        //Verificamos que el primer token termina en ":"
        std::string key_with_colon tokens[0];
        if (key_with_colon.back() != ':') {
            print_error("Unknown configuration key" + key_with_colon);
            return false;
        }

        std::string key = key_with_colon.substr(0, key_with_colon.length() - 1);
        try {
            //Verificamos que no haya información excesiva antes de procesar
            auto check_excess = [&](size_t expected_size) {
                if (tokens_size() > expected_size) {
                    std:: string extra;
                    for (size_t i = expected_size; i < tokens.size(); ++1) {
                        extra += tokens[i];
                        if (i < tokens.size() - 1) extra += " ";
                    }

                    print_error("Extra data after configuration value for key" + key + "\nExtra" + extra);
                    return false;
                }
                return true;
            };

            //Ahora vamos a comprobar cada parámetro como indica el enunciado
            if (key == "aspect_ratio") {
                if (tokens.size() < 3) {
                    print_error("Invalid value for key aspectratio\nLine: " + line);
                    return false;
                }
                if (!check_excess(3)) return false;

                int w = std::stoi(tokens[1]);
                int h = std::stoi(tokens[2]);
                if (w <= 0 || h <= 0) {
                    print_error("Invalid value for key aspectario\nLine: " + line);
                    return false;
                }

                config.aspect_width = w;
                config.aspect_height = h;

            }

            else if (key == "imagewidth") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key imagewidth\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;
                
                int w = std::stoi(tokens[1]);
                if (w <= 0) {
                    print_error("Invalid value for key imagewidth\nLine: " + line);
                    return false;
                }
                config.image_width = w;
            }

            else if (key == "gamma") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key gamma\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;

                config.gamma = std::stod(tokens[1]);
            }

            else if (key == "cameraposition") {
                if (tokens.size() < 4) {
                    print_error("Invalid value for key cameraposition\nLine: " + line);
                    return false;
                }
                if (!check_excess(4)) return false;

                config.camera_x = std::stod(tokens[1]);
                config.camera_y = std::stod(tokens[2]);
                config.camera_z = std::stod(tokens[3]);
            }

            else if (key == "cameratarget") {
                if (tokens.size() < 4) {
                    print_error("Invalid value for key cameratarget\nLine: " + line);
                    return false;
                }
                if (!check_excess(4)) return false;

                config.target_x = std::stod(tokens[1]);
                config.target_y = std::stod(tokens[2]);
                config.target_z = std::stod(tokens[3]);
            }

            else if (key == "cameranorth") {
                if (tokens.size() < 4) {
                    print_error("Invalid value for key cameranorth\nLine: " + line);
                    return false;
                }
                if (!check_excess(4)) return false;

                config.north_x = std::stod(tokens[1]);
                config.north_y = std::stod(tokens[2]);
                config.north_z = std::stod(tokens[3]);
            }

            else if (key == "fieldofview") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key fieldofview\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;

                double fov = std::stod(tokens[1]);
                if (fov <= 0 || fov >= 180) {
                    print_error("Invalid value for key fieldofview\nLine: " + line);
                    return false;
                }
                config.field_of_view = fov;
            }

            else if (key == "samplesperpixel") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key samplesperpixel\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;

                int samples = std::stoi(tokens[1]);
                if (samples <= 0) {
                    print_error("Invalid value for key samplesperpixel\nLine: " + line);
                    return false;
                }
                config.samples_per_pixel = samples;
            }

            else if (key == "maxdepth") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key maxdepth\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;

                int depth = std::stoi(tokens[1]);
                if (depth <= 0) {
                    print_error("Invalid value for key maxdepth\nLine: " + line);
                    return false;
                }

                config.max_depth = depth;
            }

            else if (key == "materialrngseed") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key materialrngseed\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;

                int seed = std::stoi(tokens[1]);
                if (seed <= 0) {
                    print_error("Invalid value for key materialrngseed\nLine: " + line);
                    return false;
                }
                config.material_rng_seed = seed;
            }

            else if (key == "rayrngseed") {
                if (tokens.size() < 2) {
                    print_error("Invalid value for key rayrngseed\nLine: " + line);
                    return false;
                }
                if (!check_excess(2)) return false;

                int seed = std::stoi(tokens[1]);
                if (seed <= 0) {
                    print_error("Invalid value for key rayrngseed\nLine: " + line);
                    return false;
                }
                config.ray_rng_seed = seed;
            }

            else if (key == "backgrounddarkcolor") {
                if (tokens.size() < 4) {
                    print_error("Invalid value for key backgrounddarkcolor\nLine" + line);
                    return false;
                }
                if (!check_excess(4)) return false;

                double r = std::stod(tokens[1]);
                double g = std::stod(tokens[2]);
                double b = std::stod(tokens[3]);

                if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
                    print_error("Invalid value for key backgrounddarkcolor\nLine" + line);
                    return false;
                }

                config.bg_dark_r = r;
                config.bg_dark_g = g;
                config.bg_dark_b = b;
            }

            else if (key == "backgroundlightcolor") {
                if (tokens.size() < 4) {
                    print_error("Invalid value for key backgroundlightcolor\nLine" + line);
                    return false;
                }
                if (!check_excess(4)) return false;

                double r = std::stod(tokens[1]);
                double g = std::stod(tokens[2]);
                double b = std::stod(tokens[3]);

                if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
                    print_error("Invalid value for key backgroundlightcolor\nLine" + line);
                    return false;
                }

                config.bg_dark_r = r;
                config.bg_dark_g = g;
                config.bg_dark_b = b;
            }

            else {
                print_error("Unkown configuration key" + key_with_colon);
                return false;
            }
        }

        catch (const std::exception& e) {
            print_error("Invalid value for key " + key + "\nLine: " + line);
            return false;
        }
    }

    return true;
}

