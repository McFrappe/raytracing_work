#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere() {}
    sphere(point3 center, float radius) : center(center), radius(radius) {};

    virtual bool hit(
	const ray &r,
	float t_min,
	float t_max,
	hit_record &rec)
      const override;

  public:
    point3 center;
    float radius;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  vec3 oc = r.origin() - center;
  float a = r.direction().length_squared();
  float half_b = dot(oc, r.direction());
  float c = oc.length_squared() - radius*radius;
  float discriminant = half_b*half_b - a*c;

  if (discriminant < 0)
    return false;

  float sqrt_d = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  float root = (-half_b - sqrt_d) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrt_d) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);

  return true;
}
#endif