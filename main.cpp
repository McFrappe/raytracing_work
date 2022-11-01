#include "tools.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <memory>

color ray_color(const ray &r, const hittable &world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec))
    return 0.5 * (rec.normal + color(1, 1, 1));

  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char **argv) {
  // image
  const float aspect_ratio 	= 16.0 / 9.0;
  const int image_width 	= 1200;
  const int image_height 	= static_cast<int>(image_width / aspect_ratio);

  // world attr
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  // camera viewpoint
  float viewpoint_height 	= 2.0;
  float viewpoint_width 	= aspect_ratio * viewpoint_height;
  float focal_length 		= 1.0;

  point3 origin 		= point3(0, 0, 0);
  vec3 horizontal 		= vec3(viewpoint_width, 0, 0);
  vec3 vertical 		= vec3(0, viewpoint_height, 0);
  vec3 lower_left_corner 	= origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

  // render image
  printf("P3\n%d %d\n255\n", image_width, image_height);

  for (int i = image_height - 1; i >= 0; i--) {
    std::cerr << "\nScanlines remaining: " << i  << ' ' << std::flush;
    for (int j = 0; j < image_width; j++) {
      float u = float(j) / (image_width - 1);
      float v = float(i) / (image_height - 1);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
      color pixel_color = ray_color(r, world);
      write_color(std::cout, pixel_color);
    }
  }

  std::cerr << "\nDone.\n";
  return 0;
}
