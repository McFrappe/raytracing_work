#ifndef COLOR_H
#define COLOR_H

#include "tools.h"
#include "vec3.h"
#include <cstring>

// recall that color is vec3 type
inline char *get_pixel(color pixel_color, int samples_per_pixel) {
   float r = pixel_color.x();
   float g = pixel_color.y();
   float b = pixel_color.z();

   // Divide the color by the number of samples
   float scale = 1.0 / samples_per_pixel;
   r = sqrt(scale * r);
   g = sqrt(scale * g);
   b = sqrt(scale * b);

   // write translated [0, 255] value of each color component

   std::string red = std::to_string(static_cast<int>(256 * clamp(r, 0.0, 0.999)));
   std::string green = std::to_string(static_cast<int>(256 * clamp(g, 0.0, 0.999)));
   std::string blue = std::to_string(static_cast<int>(256 * clamp(b, 0.0, 0.999)));
   std::string pixel = red + ' ' + green + ' ' + blue + '\n' + '\0';

   char *cstr = new char[pixel.length() + 2];
   std::strcpy(cstr, pixel.c_str());

   return cstr;
}

inline void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
   float r = pixel_color.x();
   float g = pixel_color.y();
   float b = pixel_color.z();

   // Divide the color by the number of samples
   float scale = 1.0 / samples_per_pixel;
   r = sqrt(scale * r);
   g = sqrt(scale * g);
   b = sqrt(scale * b);

   // write translated [0, 255] value of each color component
   out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif
