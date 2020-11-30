#pragma once
#include "hittable.h"
#include "vec3.h"
#include "material.h"

class sphere : public hittable {
public: 

	sphere() { 
		center = point3(0, 0, 0); radius = 1.0; 
	}
	sphere(point3 cen, double r, shared_ptr<material>m = make_shared<lambertian>(color(0.75, 0.75, 0.75))) : center(cen), radius(r), mat_ptr(m) {}


	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override; 
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

private: 
	static void get_sphere_uv(const point3& p, double& u, double& v) {
		// p: a given point on the sphere of radius one, centered at the origin.
		// u: returned value [0,1] of angle around the Y axis from X=-1.
		// v: returned value [0,1] of angle from Y=-1 to Y=+1.
		//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
		//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
		//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

		auto theta = acos(-p.y()); 
		auto phi = atan2(-p.z(), p.x()) + pi;

		u = phi / (2 * pi); 
		v = theta / pi;
	}


// Consider making attributes private
// and moving above constructor
public:
	point3 center; 
	double radius;
	shared_ptr<material> mat_ptr;

};

/// <summary>
/// Updates hitRecord with data about closest ray-sphere intersection 
/// to ray origin(ie. camera)
/// </summary>
/// <param name="inRay"></param>
/// <param name="t_min"></param>
/// <param name="t_max"></param>
/// <param name="hitRecord"></param>
/// <returns></returns>
bool::sphere::hit(const ray& inRay, double t_min, double t_max, hit_record& hitRecord) const
{
	vec3 oc = inRay.origin() - center;
	auto a = inRay.direction().length_squared();
	auto half_b = dot(oc, inRay.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) 
		return false;

	auto sqrtDiscriminant = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtDiscriminant) / a; 
	if (root < t_min || root > t_max) {
		root = (-half_b + sqrtDiscriminant) / a;
		if (root < t_min || root > t_max)
			return false;
	}

	// return hit information through out paramenter hitRecord
	hitRecord.t = root;	// t-value at hit position
	hitRecord.p = inRay.at(hitRecord.t); // Hit position as type point3
	vec3 outward_normal = (hitRecord.p - center) / radius ; // normal pointing to out from sphere center in direction of ray
	hitRecord.set_face_normal(inRay, outward_normal); // sets normal in opposite directon of incoming ray
	get_sphere_uv(outward_normal, hitRecord.u, hitRecord.v);
	hitRecord.mat_ptr = mat_ptr;

	return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
	output_box = aabb(center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}