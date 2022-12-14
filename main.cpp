#ifdef OMP
#include "omp.h"
#endif

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "jpeglib.h"
#include <cstdlib>
#include <iostream>
#include <memory>

#define NUM_THREADS 10

GLOBAL(void) write_JPEG_file(
    char *filename,
    int image_width,
    int image_height,
    unsigned char *image_buffer,
    int quality) {
  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * (see the second half of this file for an example).  But here we just
   * take the easy way out and use the standard error handler, which will
   * print a message on stderr and call exit() if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE *outfile;                /* target file */
  JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */
  int row_stride;               /* physical row width in image buffer */

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
   * step fails.  (Unlikely, but it could happen if you are out of memory.)
   * This routine fills in the contents of struct jerr, and returns jerr's
   * address which we place into the link field in cinfo.
   */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = image_width;      /* image width and height, in pixels */
  cinfo.image_height = image_height;
  cinfo.input_components = 3;           /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB;       /* colorspace of input image */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  row_stride = image_width * 3; /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
    (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */
}

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

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_float();
      point3 center(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
	shared_ptr<material> sphere_material;

	if (choose_mat < 0.8) {
	  // diffuse
	  auto albedo = color::random() * color::random();
	  sphere_material = make_shared<lambertian>(albedo);
	  world.add(make_shared<sphere>(center, 0.2, sphere_material));
	} else if (choose_mat < 0.95) {
	  // metal
	  auto albedo = color::random(0.5, 1);
	  auto fuzz = random_float(0, 0.5);
	  sphere_material = make_shared<metal>(albedo, fuzz);
	  world.add(make_shared<sphere>(center, 0.2, sphere_material));
	} else {
	  // glass
	  sphere_material = make_shared<dialectric>(1.5);
	  world.add(make_shared<sphere>(center, 0.2, sphere_material));
	}
      }
    }
  }

  auto material1 = make_shared<dialectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

int main(int argc, char **argv) {
  // image
  const float aspect_ratio = 3.0 / 2.0;
  const int image_width = atoi(argv[1]);
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 500;
  const int max_depth = 50;
  unsigned char *image_content =
    (unsigned char *)calloc(sizeof(unsigned char), image_width * image_height * 3);

  // world attr
  hittable_list world = random_scene();

  // camera
  point3 lookfrom(13,2,3);
  point3 lookat(0,0,0);
  vec3 vup(0,1,0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;

  camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

  // render image
  printf("width: %d\nheight: %d\n", image_width, image_height);

#pragma omp parallel for shared(image_width, image_height, samples_per_pixel,  \
    world, cam) schedule(guided) collapse(2)
  for (int i = image_height - 1; i >= 0; i--) {
    for (int j = 0; j < image_width; j++) {
      color pixel_color(0, 0, 0);

      for (int k = 0; k < samples_per_pixel; k++) {
	float u = (float(j) + random_float()) / (image_width - 1);
	float v = (float(i) + random_float()) / (image_height - 1);
	pixel_color += ray_color(cam.get_ray(u, v), world, max_depth);
      }

      memcpy(
	  &image_content[3 * ((image_width * image_height) - ((i * image_width + j)))],
	  get_pixel(pixel_color, samples_per_pixel),
	  sizeof(unsigned char) * 3);
    }
  }

  write_JPEG_file((char *)"image.jpeg", image_width, image_height, image_content, 100);

  printf("Done.\n");
  return 0;
}
