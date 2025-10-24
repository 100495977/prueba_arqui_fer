#include "../include/soa_camera.hpp"
#include <cmath>

namespace {

  inline void norm(double & x, double & y, double & z) {
    double l = std::sqrt(x * x + y * y + z * z);
    if (l > 0.0) {
      double inv = 1.0 / l;
      x *= inv;
      y *= inv;
      z *= inv;
    }
  }

}  // anonymous namespace

namespace soa {

  CameraSOA::CameraSOA(ConfigParams const & c)
      : OX(c.camera_x), OY(c.camera_y), OZ(c.camera_z), FOV(c.field_of_view * M_PI / 180.0) {
    // Origen y vectores base

    double dx = OX - c.target_x, dy = OY - c.target_y, dz = OZ - c.target_z;
    DF = std::sqrt(dx * dx + dy * dy + dz * dz);

    // w = unit(lookfrom - lookat)
    WX = dx;
    WY = dy;
    WZ = dz;
    norm(WX, WY, WZ);

    // u = unit(cross(vup, w))
    double cx = c.north_y * WZ - c.north_z * WY;
    double cy = c.north_z * WX - c.north_x * WZ;
    double cz = c.north_x * WY - c.north_y * WX;
    UX        = cx;
    UY        = cy;
    UZ        = cz;
    norm(UX, UY, UZ);

    // v = cross(w, u)
    VX = WY * UZ - WZ * UY;
    VY = WZ * UX - WX * UZ;
    VZ = WX * UY - WY * UX;
  }

  void CameraSOA::generate_primary_rays(std::size_t w, std::size_t h, std::size_t spp) {
    if (w == 0 or h == 0 or spp == 0) {
      origins_x.clear(), origins_y.clear(), origins_z.clear(), dirs_x.clear(), dirs_y.clear(),
          dirs_z.clear();
      return;
    }
    double hp     = 2.0 * std::tan(FOV * 0.5) * DF;
    double aspect = static_cast<double>(w) / static_cast<double>(h);
    double wp     = hp * aspect;
    double phx = wp * UX, phy = wp * UY, phz = wp * UZ;
    double pvx = hp * VX, pvy = hp * VY, pvz = hp * VZ;
    double p0x  = OX - 0.5 * phx - 0.5 * pvx - DF * WX;
    double p0y  = OY - 0.5 * phy - 0.5 * pvy - DF * WY;
    double p0z  = OZ - 0.5 * phz - 0.5 * pvz - DF * WZ;
    double dx_x = phx / double(w), dx_y = phy / double(w), dx_z = phz / double(w);
    double dy_x = pvx / double(h), dy_y = pvy / double(h), dy_z = pvz / double(h);
    std::size_t total = w * h * spp;
    origins_x.assign(total, OX);
    origins_y.assign(total, OY);
    origins_z.assign(total, OZ);
    dirs_x.resize(total);
    dirs_y.resize(total);
    dirs_z.resize(total);
    for (std::size_t j = 0; j < h; ++j) {
      double ry = double(j) + 0.5;
      for (std::size_t i = 0; i < w; ++i) {
        double rx = double(i) + 0.5;
        double px = p0x + rx * dx_x + ry * dy_x;
        double py = p0y + rx * dx_y + ry * dy_y;
        double pz = p0z + rx * dx_z + ry * dy_z;
        double dx = px - OX, dy = py - OY, dz = pz - OZ;
        norm(dx, dy, dz);
        std::size_t base = ((j * w) + i) * spp;
        for (std::size_t s = 0; s < spp; ++s) {
          dirs_x[base + s] = dx;
          dirs_y[base + s] = dy;
          dirs_z[base + s] = dz;
        }
      }
    }
  }

}  // namespace soa
