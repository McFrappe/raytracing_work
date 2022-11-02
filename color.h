#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include "tools.h"

// recall that color is vec3 type
inline void write_color(
    std::ostream &out,
    color pixel_color,
    int samples_per_pixel
    ) {
  float r = pixel_color.x();
  float g = pixel_color.y();
  float b = pixel_color.z();

  // Divide the color by the number of samples
  float scale = 1.0 / samples_per_pixel;

  r *= scale;
  g *= scale;
  b *= scale;

  // write translated [0, 255] value of each color component
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif
