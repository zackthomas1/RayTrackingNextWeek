#pragma once
#include "vec3.h"

class ray {
public:
	// Constructors
	ray() {
		orig = point3(0, 0, 0);
		dir = vec3(1, 0, 0);
	}
	ray(const point3& origin, const vec3& direction, double time = 0.0)
		: orig(origin), dir(direction), tm(time)
	{}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }

	/// <summary>
	/// Returns a 3d point cooridinate of a ray given a time value (t)
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	point3 at(double t) const {
		return orig + t * dir;
	}

	// Consider making instance variables private 
	// since origin() and direction() methods return variables
private:
	point3 orig;
	vec3 dir;
	double tm;
};