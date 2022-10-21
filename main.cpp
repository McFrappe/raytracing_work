#include <iostream>

#define _DIM 256

int main(int argc, char **argv) {
	const int image_width = _DIM;
	const int image_height = _DIM;

	printf("P3\n%d %d\n255\n", image_width, image_height);

	for (int i = image_height - 1; i >= 0; i--) {
		std::cerr << "\nScanlines remaining: " << i  << ' ' << std::flush;
		for (int j = 0; j < image_width; j++) {
			auto r = double(j) / (image_width - 1);
			auto g = double(i) / (image_height - 1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			printf("%d %d %d", ir, ig, ib);
		}
	}

	std::cerr << "\nDone.\n";
	return 0;
}
