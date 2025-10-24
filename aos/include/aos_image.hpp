#ifndef AOS_IMAGE_HPP
#define AOS_IMAGE_HPP

#include "aos_vector.hpp"
#include <vector>

namespace aos {

  struct Pixel {
    unsigned char r, g, b;
    Pixel();
    Pixel(unsigned char red, unsigned char green, unsigned char blue);
  };

  class AOSImage {
  public:
    AOSImage(int width, int height);

    // Funciones principales
    void set_pixel(int x, int y, ColorVector const & color);
    void write_ppm(std::ostream & out) const;

    // Getters
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] Pixel const & get_pixel(int x, int y) const;

  private:
    int width_;
    int height_;
    std::vector<Pixel> pixels_;
  };

}  // namespace aos

#endif  // AOS_IMAGE_HPP
