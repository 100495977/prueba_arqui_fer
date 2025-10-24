#ifndef SOA_CAMERA_HPP
#define SOA_CAMERA_HPP

#include "../../common/include/config.hpp"
#include <cstddef>
#include <vector>

namespace soa {

  class CameraSOA {
  public:
    explicit CameraSOA(ConfigParams const & cfg);
    void generate_primary_rays(std::size_t w, std::size_t h, std::size_t spp);

    std::vector<double> origins_x, origins_y, origins_z;
    std::vector<double> dirs_x, dirs_y, dirs_z;

  private:
    double OX{}, OY{}, OZ{};
    double UX{}, UY{}, UZ{};
    double VX{}, VY{}, VZ{};
    double WX{}, WY{}, WZ{};
    double DF{}, FOV{};
  };

}  // namespace soa

#endif
