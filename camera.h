#ifndef CAMERA_H
#define CAMERA_H

#include "tools.h"
#include "vec3.h"

class camera {
  public:
    camera(
	point3 lookfrom,
	point3 lookat,
	vec3 vup,
	float vfov, // vertical fieldofview in degrees
	float aspect_ratio,
	float aperture,
	float focus_dist
	) {
      auto theta = degrees_to_radians(vfov);
      auto h = tan(theta / 2);
      auto viewpoint_height = 2.0 * h;
      auto viewpoint_width = aspect_ratio * viewpoint_height;
      auto focal_length = 1.0;

      vec3 w = unit_vector(lookfrom - lookat);
      vec3 u = unit_vector(cross(vup, w));
      vec3 v = cross(w, u);

      origin = lookfrom;
      horizontal = focus_dist * viewpoint_width * u;
      vertical = focus_dist * viewpoint_height * v;
      lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;
      lens_radius = aperture / 2;
    }

    ray get_ray(float s, float t) {
      vec3 rd = lens_radius * random_in_unit_sphere();
      vec3 offset = u * rd.x() + v * rd.y();

      return ray(
	  origin + offset,
	  lower_left_corner + s * horizontal + t * vertical - origin - offset
	  );
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif
