#ifdef OMP
#include "/opt/homebrew/opt/libomp/include/omp.h"
#endif

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <cstdlib>
#include <iostream>
#include <memory>

#define NUM_THREADS 10

color ray_color(const ray &r, const hittable &world, int depth) {
   // If we exceed the ray bounce limit, we return and no more light is
   // gathered
   if (depth <= 0)
      return color(0.0, 0.0, 0.0);

   hit_record rec;
   if (world.hit(r, 0.001, infinity, rec)) {
      // We can mix between these two methods of performing diffuse lighting.
      // The latter is the more correct. The first is more of a hack.
      // The more correct method is much faster than the hack.

      /* point3 target = rec.p + rec.normal + random_unit_vector(); */
      point3 target = rec.p + random_in_hemisphere(rec.normal);
      return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
   }

   vec3 unit_direction = unit_vector(r.direction());
   float t = 0.5 * (unit_direction.y() + 1.0);
   return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char **argv) {
   // image
   const float aspect_ratio 	= 16.0 / 9.0;
   const int image_width 	= atoi(argv[1]);
   const int image_height 	= static_cast<int>(image_width / aspect_ratio);
   const int samples_per_pixel 	= 100;
   const int max_depth 		= 50;

   // world attr
   hittable_list world;
   world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
   world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

   // camera
   camera cam;

   // render image
   printf("P3\n%d %d\n255\n", image_width, image_height);

   for (int i = image_height - 1; i >= 0; i--) {
      std::cerr << "\nScanlines remaining: " << i  << ' ' << std::flush;

#ifdef OMP
      omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for \
      shared(image_width, image_height, samples_per_pixel, i, world, cam) \
      schedule(static)
#endif
      for (int j = 0; j < image_width; j++) {
	 color pixel_color(0, 0, 0);

	 for (int k = 0; k < samples_per_pixel; k++) {
	    float u = (float(j) + random_float()) / (image_width - 1);
	    float v = (float(i) + random_float()) / (image_height - 1);
	    pixel_color += ray_color(cam.get_ray(u, v), world, max_depth);
	 }

#ifdef OMP
#pragma omp critical
#endif
	 write_color(std::cout, pixel_color, samples_per_pixel);
      }
   }

   std::cerr << "\nDone.\n";
   return 0;
}
