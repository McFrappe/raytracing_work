#include <iostream>
#include "vec3.h"
#include "color.h"

#define _DIM 256

int main(int argc, char **argv) {
	const int image_width = _DIM;
	const int image_height = _DIM;

	printf("P3\n%d %d\n255\n", image_width, image_height);

	for (int i = image_height - 1; i >= 0; i--) {
		std::cerr << "\nScanlines remaining: " << i  << ' ' << std::flush;
		for (int j = 0; j < image_width; j++) {
			color pixel_color(
					float(j) / (image_width - 1),
					float(i) / (image_height - 1),
					0.25);

			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
	return 0;
}
