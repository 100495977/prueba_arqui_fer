#include "../include/render_soa.hpp"

#include "../../common/include/vector.hpp"
#include "../include/soa_color.hpp"
#include "../include/soa_ray.hpp"

#include <algorithm>
#include <cmath>

namespace soa {

  namespace {

    // Colorea por normal en [0,1]
    inline color::Color normal_to_color(render::vector const & n) {
      return color::Color{0.5 * (n.get_x() + 1.0), 0.5 * (n.get_y() + 1.0),
                          0.5 * (n.get_z() + 1.0)};
    }

    inline color::Color background_for_dir(double dy, ConfigParams const & cfg) {
      color::Color light{cfg.background_light_color_r, cfg.background_light_color_g,
                         cfg.background_light_color_b};
      color::Color dark{cfg.background_dark_color_r, cfg.background_dark_color_g,
                        cfg.background_dark_color_b};
      return color::background_color(dy, light, dark);
    }

    struct HitResult {
      bool hit{false};
      double t{0};
      render::vector normal;
    };

    // Intersecta con todas las esferas y cilindros y devuelve el más cercano
    HitResult closest_hit(ray::Ray const & r, SceneOutput const & scene) {
      HitResult result{};
      ray::IntersectionParams params{ray::MIN_DISTANCE, 1e18};

      // Esferas
      for (auto const & sph : scene.spheres.get()) {
        auto center = render::vector{sph.center_x, sph.center_y, sph.center_z};
        auto rec    = ray::hit_sphere(r, center, sph.radius, params);
        if (rec and (!result.hit or rec->t < result.t)) {
          result.hit    = true;
          result.t      = rec->t;
          result.normal = rec->normal;
          params.t_max  = rec->t;  // acotamos para siguientes
        }
      }

      // Cilindros
      for (auto const & cyl : scene.cylinders.get()) {
        ray::CylinderParams cp{};
        cp.center = render::vector{cyl.center_x, cyl.center_y, cyl.center_z};
        cp.radius = cyl.radius;
        cp.axis   = render::vector{cyl.axis_x, cyl.axis_y, cyl.axis_z};
        cp.height = cyl.height;
        auto rec  = ray::hit_cylinder(r, cp, params);
        if (rec and (!result.hit or rec->t < result.t)) {
          result.hit    = true;
          result.t      = rec->t;
          result.normal = rec->normal;
          params.t_max  = rec->t;
        }
      }

      return result;
    }

  }  // namespace

  void render_scene(ConfigParams const & cfg, SceneOutput const & scene, CameraSOA & camera,
                    SOAImage & image) {
    int w   = image.width();
    int h   = image.height();
    int spp = std::max(1, cfg.samples_per_pixel);
    camera.generate_primary_rays(static_cast<std::size_t>(w), static_cast<std::size_t>(h),
                                 static_cast<std::size_t>(spp));
    for (int j = 0; j < h; ++j) {
      for (int i = 0; i < w; ++i) {
        color::Color accum{0.0, 0.0, 0.0};
        std::size_t base = (static_cast<std::size_t>(j) * static_cast<std::size_t>(w) +
                            static_cast<std::size_t>(i)) *
                           static_cast<std::size_t>(spp);
        for (int s = 0; s < spp; ++s) {
          std::size_t si  = static_cast<std::size_t>(s);
          std::size_t idx = base + si;
          auto ox = camera.origins_x[idx], oy = camera.origins_y[idx], oz = camera.origins_z[idx];
          auto dx = camera.dirs_x[idx], dy = camera.dirs_y[idx], dz = camera.dirs_z[idx];
          ray::Ray r{
            render::vector{ox, oy, oz},
            render::vector{dx, dy, dz}
          };
          auto hit = closest_hit(r, scene);
          color::Color c{};
          if (hit.hit) {
            c = normal_to_color(hit.normal);
          } else {
            c = background_for_dir(dy, cfg);
          }
          accum = accum + c;
        }

        // Promedio por muestras
        double inv_spp = 1.0 / static_cast<double>(spp);
        RGBColor out_color{accum.r * inv_spp, accum.g * inv_spp, accum.b * inv_spp};
        image.setPixel(j, i, out_color, cfg.gamma);
      }
    }
  }

}  // namespace soa
