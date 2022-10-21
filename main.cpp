#include <iostream>

int main(int argc, char **argv) {
  const int image_width = 256;
  const int image_height = 256;

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  /* printf("P3\n%d %d\n255\n", image_width, image_height); */

  for (int i = image_height - 1; i >= 0; i--) {
	for (int j = 0; j < image_width; j++) {
	  auto r = double(j) / (image_width - 1);
	  auto g = double(i) / (image_height - 1);
	  auto b = 0.25;

	  int ir = static_cast<int>(255.999 * r);
	  int ig = static_cast<int>(255.999 * g);
	  int ib = static_cast<int>(255.999 * b);

	  std::cout << ir << ' ' << ig << ' ' << ib << std::endl;
	}
  }
}
