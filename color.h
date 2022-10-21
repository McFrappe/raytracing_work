#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

// recall that color is vec3 type
inline void write_color(std::ostream &out, color pixel_color) {
	// write translated [0, 255] value of each color component
	out << static_cast<int>(255.99 * pixel_color.x()) << ' '
		<< static_cast<int>(255.99 * pixel_color.y()) << ' '
		<< static_cast<int>(255.99 * pixel_color.z()) << '\n';

}

#endif
