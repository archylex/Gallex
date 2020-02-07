#ifndef __PHYSICS__
#define __PHYSICS__

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct vec2d {
	double x;
	double y;
};

double GetTrueAnomaly(const double eccentricity, const double orbital_period, const double time);
double GetDistance(double semimajor_axis, double eccentricity, double true_anomaly);
vec2d Get2DCoord(double semimajor_axis, double eccentricity, double true_anomaly);
bool CheckCollide(glm::vec3 posA, glm::vec3 posB, double radiusA, double radiusB);

#endif // !__PHYSICS__