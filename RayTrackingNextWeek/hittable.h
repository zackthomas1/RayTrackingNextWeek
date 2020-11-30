#pragma once
#include "rtweekend.h"
#include "aabb.h"

// Foward declaration
class material;

struct hit_record {
	point3 p = point3(0, 0, 0); // consider renaming 'pos', 'position', or 'hitPosition'
	vec3 normal = vec3(0, 0, 0);
	shared_ptr<material> mat_ptr;
	double t = 0.0;
	double u = 0.0; 
	double v = 0.0;
	bool front_face = NULL; 

	/// <summary>
	/// Determines if ray is inside or outside hittable object.
	/// Then sets normal in opposite direction of inRay
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="outward_normal"></param>
	inline void set_face_normal(const ray& inRay, const vec3& outward_normal) {
		front_face = dot(inRay.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public: 
	/// <summary>
	/// Updates hitRecord with data about closest ray-hittable object intersection 
	/// to ray origin(ie. camera)
	/// </summary>
	/// <param name="r"></param>
	/// <param name="t_min"></param>
	/// <param name="t_max"></param>
	/// <param name="rec"></param>
	/// <returns></returns>
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0; 
};