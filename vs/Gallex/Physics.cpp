#include "Physics.h"

double GetTrueAnomaly(const double eccentricity, const double orbital_period, const double time)
{
	double PsiDiff = 1.0f;
	double M = (orbital_period > 0) ? 2.0f * M_PI * time / orbital_period : 0;
	double PsiOld = M;
	double theta0old = 180.0f;

	while (PsiDiff > 1e-10) {
		double PsiNew = M + eccentricity * sin(PsiOld);
		PsiDiff = PsiNew - PsiOld;
		PsiOld = PsiNew;
		double theta0 = 2 * atan(sqrt((1 + eccentricity) / (1 - eccentricity)) * tan(PsiOld / 2.0f));
		return theta0;
	}
	
	return 0;
}

double GetDistance(double semimajor_axis, double eccentricity, double true_anomaly)
{
	return semimajor_axis * (1 - pow(eccentricity, 2)) / (1 + eccentricity * cos(true_anomaly));
}

vec2d Get2DCoord(double semimajor_axis, double eccentricity, double true_anomaly)
{
	vec2d point;
	double r_orbit = GetDistance(semimajor_axis, eccentricity, true_anomaly);
	point.x = r_orbit * cos(true_anomaly);
	point.y = r_orbit * sin(true_anomaly);
	return point;
}

bool CheckCollide(glm::vec3 posA, glm::vec3 posB, double radiusA, double radiusB)
{
	double distance = sqrt(pow(posB.x - posA.x, 2) + pow(posB.y - posA.y, 2) + pow(posB.z - posA.z, 2));

	if (distance <= (radiusA + radiusB))
		return true;
	else
		return false;
}