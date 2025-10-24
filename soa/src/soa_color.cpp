#include "../include/soa_color.hpp"
// #include <algorithm>
#include <cmath>
#include <cstdint>

namespace color {

  Color background_color(double dir_y, Color const & light_color, Color const & dark_color) {
    double m = (dir_y + 1.0) / 2.0;

    // color = (1-m)*light_color + m*dark_color
    return (1.0 - m) * light_color + m * dark_color;
  }

  uint8_t to_byte(double value, double gamma) {
    double clamped   = clamp(value, 0.0, 1.0);
    double corrected = std::pow(clamped, 1.0 / gamma);

    return static_cast<uint8_t>(corrected * 255.0);
  }

}  // namespace color
