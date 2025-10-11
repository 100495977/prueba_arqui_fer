#ifndef RENDER_MATH_UTIL_HPP
#define RENDER_MATH_UTIL_HPP

#include <random>
#include <cmath>
#include <limits>
#include <iostream>
#include "vector.hpp"
#include "config.hpp"

namespace render {
    
    // CLASE QUE DEFINE LA GENERACIÓN DE NÚMEROS ALEATORIOS
    class RNG {
    private:
        // usamos el generador Mersenne-Twister (exigido por el enunciado)
        std::mt19937_64 generator; 

    public:
        RNG(uint64_t seed) : generator(seed) {}

        [[nodiscard]] double random_double() {
            // genera [0.0, 1.0)
            return std::generate_canonical<double, 10>(generator); 
        }

        [[nodiscard]] double random_double(double min, double max) {
            // genera [min, max) a raiz del random_double base antes definido
            return min + (max - min) * random_double();
        }
    };

    // vector aleatorio con componentes en el rango [min, max]
    inline direction_vector random_vec(RNG& rng, double min = -1.0, double max = 1.0) {return direction_vector(rng.random_double(min, max), rng.random_double(min, max), rng.random_double(min, max));}

    // PROCESADO DEL COLOR (3.2)
    inline void write_color(std::ostream& out, color_vector pixel_color, int samples_per_pixel, double gamma_value) {
        
        // promedio (anti-aliasing, escalar por el número de muestras)
        double scale = 1.0 / samples_per_pixel;
        double r = pixel_color.r() * scale;
        double g = pixel_color.g() * scale;
        double b = pixel_color.b() * scale;

        // corrección Gamma (r' = r^(1/gamma))
        double gamma_inv = 1.0 / gamma_value;
        r = std::pow(r, gamma_inv); 
        g = std::pow(g, gamma_inv); 
        b = std::pow(b, gamma_inv);

        // clamping simple a [0, 1] (necesario para la conversión PPM a [0, 255])
        auto clamp_to_one = [](double val) {
            if (val < 0.0) return 0.0;
            if (val > 1.0) return 1.0;
            return val;
        };
        
        // escalado a [0, 255] (requisito de salida PPM)
        int r_out = static_cast<int>(256.0 * clamp_to_one(r));
        int g_out = static_cast<int>(256.0 * clamp_to_one(g));
        int b_out = static_cast<int>(256.0 * clamp_to_one(b));

        // R, G y B salen en el output stream separados con espacios (formato P3)
        out << r_out << ' ' << g_out << ' ' << b_out << '\n';
    }

} // namespace render

#endif