#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Using 

using std::shared_ptr; 
using std::make_shared; 
using std::sqrt; 

// Constants 

const double infinity = std::numeric_limits<double>::infinity(); 
const double pi = 3.1415926535897932385; 

// Utility Functions 

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline bool DoubleEquality(double v01, double v02){
	const double EPSILON = 0.0001;
	if (abs(v01 - v02) < EPSILON)
		return true;
	else
		return false;
}

/// <summary>
/// Limits upper and lower value limits by 
/// setting values above and below min and max to min or max.
/// </summary>
/// <param name="x"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
inline double clamp(double x, double min, double max) {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

/// <summary>
/// Generates a random floating point number
/// </summary>
/// <returns></returns>
inline double random_double() {
	// Returns a random real in[0,1). 
	return rand() / (RAND_MAX + 1.0);
}

/// <summary>
/// Generates a random floating point number within given range
/// </summary>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
inline double random_double(double min, double max) {
	// Returns a random real in [min, max). 
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	// Returns a random integer in [min,max].
	return static_cast<int>(random_double(min, max + 1));
}

// Common Headers

#include "ray.h"
#include "vec3.h"