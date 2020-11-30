#pragma once
#include <cmath> 
#include <iostream>

using std::sqrt; 
using std::fabs;

class vec3
{
public: 
	// Constructors
	vec3() : e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) : e{ e0,e1,e2 } {} 

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[] (int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }
	
	vec3& operator += (const vec3 &inVec) {
		e[0] += inVec.e[0];
		e[1] += inVec.e[1];
		e[2] += inVec.e[2];
		return *this;
	}

	vec3& operator -= (const vec3& inVec) {
		e[0] -= inVec.e[0];
		e[1] -= inVec.e[1];
		e[2] -= inVec.e[2];
		return *this;
	}

	vec3& operator *= (const double scalar) {
		e[0] *= scalar;
		e[1] *= scalar;
		e[2] *= scalar;
		return *this;
	}

	vec3& operator /= (const double scalar) {
		return *this *= 1 / scalar;
	}

	bool operator ==(const vec3& inVec) const
	{
		if (e[0] == inVec.e[0] &&
			e[1] == inVec.e[1] &&
			e[2] == inVec.e[2])
			return true;
		else
			return false;
	}

	bool operator !=(const vec3& inVec) const
	{
		if (*this == inVec)
			return false;
		else
			return true;
	}

	/// <summary>
	/// Length of a vector.
	/// aka. magnitude  
	/// </summary>
	/// <returns></returns>
	double length() const {
		return sqrt(length_squared()); // Is this really the best way to get the length of vector instance?
	}

	/// <summary>
	/// Length of a vector squared
	/// </summary>
	/// <returns></returns>
	double length_squared() const {
		return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions. 
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s && fabs(e[2] < s));
	}

// Consider putting above previous public section
// Also, give variable 'e' a more descriptive name
// Or even consider not have it as an array, just be three double variables x,y,z
public: 
	double e[3]; // declares instance variable which contains vector coordinates
};

// Type aliases for vec3 
using point3 = vec3;	// 3D point 
using color = vec3;		// RGB color


// vec 3 Utility Functions 
//------------------------------------------------------------------------

inline std::ostream& operator << (std::ostream& out, const vec3& inVec) {
	return out << inVec.e[0] << ' ' << inVec.e[1] << ' ' << inVec.e[2];
}

inline vec3 operator +(const vec3& inVec01, const vec3& inVec02) {
	return vec3(inVec01[0] + inVec02[0], inVec01[1] + inVec02[1], inVec01[2] + inVec02[2]);
}

inline vec3 operator -(const vec3& inVec01, const vec3& inVec02){
	return vec3(inVec01[0] - inVec02[0], inVec01[1] - inVec02[1], inVec01[2] - inVec02[2]);
}

inline vec3 operator *(const vec3& inVec01, const vec3& inVec02) {
	return vec3(inVec01[0] * inVec02[0], inVec01[1] * inVec02[1], inVec01[2] * inVec02[2]);
}

inline vec3 operator *(double scalar, const vec3& inVec) {
	return vec3(scalar * inVec[0], scalar * inVec[1], scalar * inVec[2]);
}

inline vec3 operator *(const vec3& inVec, double scalar) {
	return scalar * inVec;
}

inline vec3 operator /(const vec3& inVec, double scalar) {
	return (1 / scalar) * inVec;
}

// consider moving dot, cross, unit_vector methods into vec3 class
// and making dot and cross methods static
/// <summary>
/// Calculate dot product. 
/// Smaller value larger the angle between vectors. 
/// value of zero is equal to 90 degree angle
/// </summary>
/// <param name="inVec01"></param>
/// <param name="inVec02"></param>
/// <returns></returns>
inline double dot(const vec3& inVec01, const vec3& inVec02 ) {
	return inVec01.e[0] * inVec02.e[0]
		 + inVec01.e[1] * inVec02.e[1]
		 + inVec01.e[2] * inVec02.e[2];
}

/// <summary>
/// Calculate cross product.
/// Returns vector that is perpendicular to two input vectors
/// </summary>
/// <param name="inVec01"></param>
/// <param name="inVec02"></param>
/// <returns></returns>
inline vec3 cross(const vec3& inVec01, const vec3& inVec02) {
	return vec3(inVec01.e[1] * inVec02.e[2] - inVec01.e[2] * inVec02.e[1],
				inVec01.e[2] * inVec02.e[0] - inVec01.e[0] * inVec02.e[2],
				inVec01.e[0] * inVec02.e[1] - inVec01.e[1] * inVec02.e[0]);
}

/// <summary>
/// Converts vector to unit vector.
/// Meaning vector magnitude is equal to 1.
/// </summary>
/// <param name="inVec"></param>
/// <returns></returns>
inline vec3 unit_vector(vec3 inVec) {
	return inVec / inVec.length();
}



// Diffuse scatter methods
// --------------------------
/// <summary>
/// Diffuse scatter method.
/// Generates random vector inside of unit sphere
/// whose center is a point that is the intersection point + normal (P + n).
/// Scatter relation to normal cos^3(X)
/// </summary>
/// <returns></returns>
inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1.0, 1.0);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}

/// <summary>
/// Diffuse scatter method.
/// Generates random vector along surface of unit sphere 
/// whose center is a point that is the intersection point + normal (P + n).
/// Scatter relation to normal cos(X)
/// </summary>
/// <returns></returns>
inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

/// <summary>
/// Diffuse scatter method.
/// Generates uniform scatter in same hemisphere as normal.
/// Scatter has no relation to the normal.
/// </summary>
/// <param name="normal"></param>
/// <returns></returns>
inline vec3 random_in_hemisphere(const vec3& normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0)	// In the same hemisphere as the normal. ie angle less than 90 degrees
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}
// --------------------------

vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0); 
		if (p.length_squared() >= 1)
			continue; 
		return p;
	}
}

/// <summary>
/// Caculates reflected rays
/// </summary>
/// <param name="v"></param>
/// <param name="n"></param>
/// <returns></returns>
inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

/// <summary>
/// Calculates refracted rays
/// </summary>
/// <param name="uv"></param>
/// <param name="n"></param>
/// <param name="etai_over_etat"></param>
/// <returns></returns>
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0); 
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n); 
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}
