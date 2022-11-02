#ifndef TOOLS_H
#define TOOLS_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
  return degrees * pi / 180.0;
}

inline float random_float() {
  // Return a random real within the bound [0, 1)
  return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max) {
  // Return a random real within specified bound [min, max)
  return min + (max - min) * random_float();
}

inline float clamp(float x, float min, float max) {
  if (x < min) return min;
  if (x > max) return max;

  return x;
}

#include "ray.h"
#include "vec3.h"

#endif
