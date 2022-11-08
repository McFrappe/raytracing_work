#ifdef OMP
#include "/opt/homebrew/opt/libomp/include/omp.h"
#endif

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

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
      ray scattered;
      color attenuation;
      if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
	 return attenuation * ray_color(scattered, world, depth - 1);

      return color(0, 0, 0);
      /* We can mix between these two methods of performing diffuse lighting. */
      /* The latter is the more correct. The first is more of a hack. The more */
      /* correct method is much faster than the hack. */
      /* point3 target = rec.p + rec.normal + random_unit_vector(); */
      /* point3 target = rec.p + random_in_hemisphere(rec.normal); */
      /* return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1); */
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

#ifdef OMP
   char **image_content = (char**)calloc(sizeof(char*), image_width * image_height);
#endif

   // world attr
   hittable_list world;

   shared_ptr<lambertian> material_ground = make_shared<lambertian>(color(0.26, 0.29, 	0.81));
   shared_ptr<lambertian> material_center = make_shared<lambertian>(color(0.7, 	0.3, 	0.3));
   shared_ptr<metal> material_left   = make_shared<metal>(color(0.8, 	0.8, 	0.8), 0.3);
   shared_ptr<metal> material_right  = make_shared<metal>(color(0.81, 	0.26, 	0.31), 0.5);

   world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
   world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
   world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
   world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

   // camera
   camera cam;

   // render image
   printf("P3\n%d %d\n255\n", image_width, image_height);

#ifdef OMP
#pragma omp parallel for \
   shared(image_width, image_height, samples_per_pixel, world, cam) \
   schedule(static) collapse(2)
   for (int i = image_height - 1; i >= 0; i--) {
      for (int j = 0; j < image_width; j++) {
#else
   for (int i = image_height - 1; i >= 0; i--) {
      std::cerr << "\nScanlines remaining: " << i  << ' ' << std::flush;
      for (int j = 0; j < image_width; j++) {
#endif
	 color pixel_color(0, 0, 0);

	 for (int k = 0; k < samples_per_pixel; k++) {
	    float u = (float(j) + random_float()) / (image_width - 1);
	    float v = (float(i) + random_float()) / (image_height - 1);
	    pixel_color += ray_color(cam.get_ray(u, v), world, max_depth);
	 }

#ifdef OMP
	 image_content[i * image_height + j] = get_pixel(pixel_color, samples_per_pixel);
      }
   }
   // Write to cout so that we can pipe into the .ppm file
   // TODO: rewrite to create file directly instead
   for (int i = 0; i < image_width * image_height; i++)
	 std::cout << image_content[i];
#else
   write_color(std::cout, pixel_color, samples_per_pixel);
      }
   }
#endif
   std::cerr << "\nDone.\n";
   return 0;
}
