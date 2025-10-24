#include <algorithm>  // Para std::clamp
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

// --- Includes de AOS ---
#include "aos_camera.hpp"  // Tu cámara AOS independiente
#include "aos_image.hpp"
#include "aos_ray.hpp"     // En lugar de ray.hpp
#include "aos_vector.hpp"  // En lugar de vector.hpp

// --- Includes de Common ---
#include "config.hpp"
#include "config_parser.hpp"
#include "hittable.hpp"
#include "material_base.hpp"
#include "material_logic.hpp"
#include "materials.hpp"
#include "math_utilities.hpp"
#include "objects.hpp"
#include "scene_parser.hpp"

// --- Constantes ---
constexpr double T_MIN = 0.001;
constexpr double T_MAX = std::numeric_limits<double>::infinity();

// --- Función Ray-Color (Lógica principal de renderizado) ---

/**
 * @brief Calcula el color de fondo (cielo)
 */
aos::ColorVector background_color(aos::Ray const & r, ConfigParams const & config) {
  aos::DirectionVector unit_direction = aos::unit_vector(r.get_direction());
  // Mapea el componente 'y' de -1.0 a 1.0 -> 0.0 a 1.0
  auto t = 0.5 * (unit_direction.get_y() + 1.0);

  // Colores base del cielo
  aos::ColorVector light(config.background_light_color_r, config.background_light_color_g,
                         config.background_light_color_b);
  aos::ColorVector dark(config.background_dark_color_r, config.background_dark_color_g,
                        config.background_dark_color_b);

  // Interpolación lineal (lerp) entre oscuro y claro
  return (1.0 - t) * dark + t * light;
}

/**
 * @brief Función recursiva que calcula el color de un rayo
 */
aos::ColorVector ray_color(aos::Ray const & r, render::hittable const & world,
                           ConfigParams const & config, render::RNG & material_rng, int depth) {
  // Si alcanzamos el límite de rebotes, no más luz
  if (depth <= 0) {
    return aos::ColorVector(0.0, 0.0, 0.0);
  }

  render::hit_record rec;

  // Convertir aos::Ray a render::ray para compatibilidad con common
  render::ray common_ray(r.get_origin(), r.get_direction());

  // Comprobamos la colisión los objetos de la escena
  if (world.hit(common_ray, T_MIN, T_MAX, rec)) {
    render::ray scattered;
    render::color_vector attenuation;

    // Preparamos la estructura de E/S para la lógica de materiales
    render::ScatterIO scatter_io{};
    scatter_io.attenuation = &attenuation;
    scatter_io.scattered   = &scattered;
    scatter_io.rng         = &material_rng;

    // Llamamos a la lógica de dispersión
    if (render::scatter(common_ray, rec, scatter_io)) {
      // El rayo rebotó. Convertir de vuelta a AOS y continuar
      aos::Ray scattered_aos(scattered.orig, scattered.dir);
      aos::ColorVector atten_color(attenuation.r(), attenuation.g(), attenuation.b());
      return atten_color * ray_color(scattered_aos, world, config, material_rng, depth - 1);
    }

    // El material absorbió el rayo
    return aos::ColorVector(0.0, 0.0, 0.0);
  }

  // Si no hay colisión, devolvemos el color del cielo
  return background_color(r, config);
}

// --- Función Principal ---

int main(int argc, char * argv[]) {
  // --- Validamos los Argumentos ---
  if (argc != 4) {
    std::cerr << "Uso: " << argv[0] << " <config_file.cfg> <scene_file.txt> <output_file.ppm>"
              << '\n';
    return 1;
  }

  std::string config_filename = argv[1];
  std::string scene_filename  = argv[2];
  std::string output_filename = argv[3];

  // --- Parseamos los Archivos de Configuración y Escena ---
  ConfigParams config;
  if (!parse_config(config_filename, config)) {
    std::cerr << "Error: No se pudo parsear el archivo de configuración." << '\n';
    return 1;
  }

  std::vector<MatteMaterial> matte_materials;
  std::vector<MetalMaterial> metal_materials;
  std::vector<RefractiveMaterial> refractive_materials;
  std::vector<Sphere> spheres;
  std::vector<Cylinder> cylinders;

  SceneOutput scene_out{matte_materials, metal_materials, refractive_materials, spheres, cylinders};

  if (!parse_scene(scene_filename, scene_out)) {
    std::cerr << "Error: No se pudo parsear el archivo de escena." << '\n';
    return 1;
  }

  // --- Preparamos la Escena (hittable_list y Materiales) ---

  // Creamos un mapa para buscar punteros a materiales por su nombre
  std::unordered_map<std::string, render::MaterialBase const *> material_map;

  for (auto const & mat : matte_materials) {
    material_map[mat.name] = &mat;
  }
  for (auto const & mat : metal_materials) {
    material_map[mat.name] = &mat;
  }
  for (auto const & mat : refractive_materials) {
    material_map[mat.name] = &mat;
  }

  // Asignamos los punteros de material a los objetos
  for (auto & sph : spheres) {
    auto it = material_map.find(sph.material_name);
    if (it != material_map.end()) {
      sph.material_ptr = it->second;
    } else {
      std::cerr << "Error: Material '" << sph.material_name << "' no encontrado para una esfera.\n";
      return 1;
    }
  }
  for (auto & cyl : cylinders) {
    auto it = material_map.find(cyl.material_name);
    if (it != material_map.end()) {
      cyl.material_ptr = it->second;
    } else {
      std::cerr << "Error: Material '" << cyl.material_name
                << "' no encontrado para un cilindro.\n";
      return 1;
    }
  }

  // Añadimos todos los objetos a la lista 'world'
  render::hittable_list world;
  for (auto const & sph : spheres) {
    world.add(&sph);
  }
  for (auto const & cyl : cylinders) {
    world.add(&cyl);
  }

  // --- Configuramos la Cámara AOS y Generadores Aleatorios ---

  // Crear ConfigParams compatible con AOS Camera
  aos::ConfigParams aos_config;
  aos_config.camera_x      = config.camera_x;
  aos_config.camera_y      = config.camera_y;
  aos_config.camera_z      = config.camera_z;
  aos_config.target_x      = config.target_x;
  aos_config.target_y      = config.target_y;
  aos_config.target_z      = config.target_z;
  aos_config.north_x       = config.north_x;
  aos_config.north_y       = config.north_y;
  aos_config.north_z       = config.north_z;
  aos_config.field_of_view = config.field_of_view;
  aos_config.aspect_width  = config.aspect_width;
  aos_config.aspect_height = config.aspect_height;

  // Crear cámara AOS
  aos::Camera cam(aos_config);

  render::RNG material_rng(config.material_rng_seed);
  render::RNG ray_rng(config.ray_rng_seed);

  // --- Parámetros de renderizado ---
  int const image_width       = config.image_width;
  int const image_height      = config.get_image_height();
  int const samples_per_pixel = config.samples_per_pixel;
  int const max_depth         = config.max_depth;
  double const gamma          = config.gamma;

  // --- Creamos la imagen AOS ---
  aos::AOSImage image(image_width, image_height);

  std::cerr << "Renderizando AOS... (Ancho=" << image_width << ", Alto=" << image_height
            << ", Muestras=" << samples_per_pixel << ")\n";

  // --- Bucle principal de renderizado (de arriba abajo) ---
  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines restantes: " << j << ' ' << std::flush;

    // --- Bucle de píxeles (de izquierda a derecha) ---
    for (int i = 0; i < image_width; ++i) {
      aos::ColorVector pixel_color(0.0, 0.0, 0.0);

      // --- Bucle de Anti-Aliasing (múltiples muestras por píxel) ---
      for (int s = 0; s < samples_per_pixel; ++s) {
        // Coordenadas (u, v) del píxel actual, con un offset aleatorio
        auto u = (static_cast<double>(i) + ray_rng.random_double()) / (image_width - 1);
        auto v = (static_cast<double>(j) + ray_rng.random_double()) / (image_height - 1);

        // Obtenemos rayo de la cámara AOS
        aos::Ray r = cam.get_ray(u, v);

        // Acumulamos el color de esta muestra
        pixel_color += ray_color(r, world, config, material_rng, max_depth);
      }

      // --- Procesamiento final del color para AOS ---
      double scale = 1.0 / samples_per_pixel;

      // Aplicar promedio y corrección gamma
      double r_val = pixel_color.r() * scale;
      double g_val = pixel_color.g() * scale;
      double b_val = pixel_color.b() * scale;

      // Corrección gamma: elevar a 1/gamma
      r_val = std::pow(r_val, 1.0 / gamma);
      g_val = std::pow(g_val, 1.0 / gamma);
      b_val = std::pow(b_val, 1.0 / gamma);

      // Clamping a [0, 1]
      r_val = std::clamp(r_val, 0.0, 1.0);
      g_val = std::clamp(g_val, 0.0, 1.0);
      b_val = std::clamp(b_val, 0.0, 1.0);

      // Crear color final AOS
      aos::ColorVector final_color(r_val, g_val, b_val);

      // Guardar en imagen AOS (nota: coordenada Y invertida para almacenamiento)
      image.set_pixel(i, image_height - 1 - j, final_color);
    }
  }

  // --- Escribir la imagen al archivo PPM ---
  std::ofstream out_file(output_filename);
  if (!out_file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo de salida " << output_filename << '\n';
    return 1;
  }

  // Usar AOSImage para escribir el PPM
  image.write_ppm(out_file);
  out_file.close();

  std::cerr << "\n¡Renderizado AOS completado!\nImagen guardada en: " << output_filename << '\n';

  return 0;
}
