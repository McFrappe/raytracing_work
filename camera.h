#ifndef CAMERA_H
#define CAMERA_H

#include "tools.h"

class camera {
  public:
    camera() {
      float aspect_ratio = 16.0 / 9.0;
      float viewpoint_height = 2.0;
      float viewpoint_width = aspect_ratio * viewpoint_height;
      float focal_length = 1.0;

      origin = point3(0, 0, 0);
      horizontal = vec3(viewpoint_width, 0.0, 0.0);
      vertical = vec3(0.0, viewpoint_height, 0.0);
      lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    }

    ray get_ray(float u, float v) {
      return ray(origin,
	  lower_left_corner + u * horizontal + v * vertical - origin
	  );
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

};

#endif