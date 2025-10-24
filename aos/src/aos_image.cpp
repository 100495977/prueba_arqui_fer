#include <cstddef>

#include "../include/aos_image.hpp"

namespace aos {

  // Pixel
  Pixel::Pixel() : r(0), g(0), b(0) { }

  Pixel::Pixel(unsigned char red, unsigned char green, unsigned char blue)
      : r(red), g(green), b(blue) { }

  // AOSImage
  AOSImage::AOSImage(int width, int height)
      : width_(width), height_(height), pixels_(static_cast<size_t>(width * height)) { }

  void AOSImage::set_pixel(int x, int y, ColorVector const & color) {
    int index        = y * width_ + x;
    pixels_[index].r = static_cast<unsigned char>(color.r() * 255);
    pixels_[index].g = static_cast<unsigned char>(color.g() * 255);
    pixels_[index].b = static_cast<unsigned char>(color.b() * 255);
  }

  void AOSImage::write_ppm(std::ostream & out) const {
    // Cabecera PPM
    out << "P3\n" << width_ << ' ' << height_ << "\n255\n";

    // Datos de píxeles
    for (int y = height_ - 1; y >= 0; --y) {
      for (int x = 0; x < width_; ++x) {
        int index = y * width_ + x;
        out << static_cast<int>(pixels_[index].r) << ' ' << static_cast<int>(pixels_[index].g)
            << ' ' << static_cast<int>(pixels_[index].b) << '\n';
      }
    }
  }

  int AOSImage::width() const {
    return width_;
  }

  int AOSImage::height() const {
    return height_;
  }

  Pixel const & AOSImage::get_pixel(int x, int y) const {
    // Use size_t for the index calculation to avoid narrowing from int to size_type
    std::size_t index = static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) +
                        static_cast<std::size_t>(x);
    return pixels_[index];
  }

}  // namespace aos
