#ifndef VEC3_H
#define VEC3_H

#include "tools.h"

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
   public:
      vec3() : e{0, 0, 0} {}
      vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

      float x() const { return e[0]; }
      float y() const { return e[1]; }
      float z() const { return e[2]; }

      vec3 operator-() const {
	 return vec3(-e[0], -e[1], -e[2]);
      }
      float operator[](int i) const {
	 return e[i];
      }
      float &operator[](int i) {
	 return e[i];
      }

      vec3 &operator+=(const vec3 &v) {
	 e[0] += v.e[0];
	 e[1] += v.e[1];
	 e[2] += v.e[2];

	 return *this;
      }

      vec3 &operator*=(const float t) {
	 e[0] *= t;
	 e[1] *= t;
	 e[2] *= t;

	 return *this;
      }

      vec3 &operator/=(const float t) {
	 return *this *= (1 / t);
      }

      float length_squared() const {
	 return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
      }

      float length() const {
	 return sqrt(length_squared());
      }

      bool near_zero() const {
	 // return true if the vector is close to zero;
	 const float s = 1e-8;
	 return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
      }

      inline static vec3 random() {
	 return vec3(
	       random_float(),
	       random_float(),
	       random_float());
      }

      inline static vec3 random(float min, float max) {
	 return vec3(
	       random_float(min, max),
	       random_float(min, max),
	       random_float(min, max));
      }

   public:
      float e[3];
};

// We can use aliases to denote different things in code for clarity
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// Piping results from a vector into a variable out
inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
   return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// define overloading for addition of vectors
inline vec3 operator-(const vec3 &u, const vec3 &v) {
   return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// define overloading for addition of vectors
inline vec3 operator+(const vec3 &u, const vec3 &v) {
   return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// overloading for multiplication of vectors
inline vec3 operator*(const vec3 &u, const vec3 &v) {
   return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// overloading for multiplication of scalar to vectors
inline vec3 operator*(float t, const vec3 &v) {
   return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// Whats the difference between this and the one before?
inline vec3 operator*(const vec3 &v, float t) {
   return t * v;
}

// overloading for division of vector with scalar
inline vec3 operator/(vec3 v, float t) {
   return (1/t) * v;
}

// dot product between two vectors
inline float dot(const vec3 &u, const vec3 &v) {
   return u.e[0] * v.e[0]
      + u.e[1] * v.e[1]
      + u.e[2] * v.e[2];
}

// cross product between two vectors
inline vec3 cross(const vec3 &u, const vec3 &v) {
   return vec3(
	 u.e[1] * v.e[2] - u.e[2] * v.e[1],
	 u.e[2] * v.e[0] - u.e[0] * v.e[2],
	 u.e[0] * v.e[1] - u.e[1] * v.e[0]
	 );
}

// get the unit vector from a given vector (the normal)
inline vec3 unit_vector(vec3 v) {
   return v / v.length();
}

inline vec3 random_in_unit_sphere() {
   while (true) {
      auto point = vec3::random(-1, 1);
      if (point.length_squared() >= 1) continue;

      return point;
   }
}

inline vec3 random_unit_vector() {
   return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3 &normal) {
   vec3 in_unit_sphere = random_in_unit_sphere();
   if (dot(in_unit_sphere, normal) > 0.0) // The normal in the hemisphere
      return in_unit_sphere;

   return -in_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
   return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3 &uv, const vec3 &n, float etai_over_etat) {
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;

  return r_out_perp + r_out_parallel;
}

#endif


