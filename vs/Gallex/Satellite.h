#ifndef __SATELLITE_H__
#define __SATELLITE_H__

#pragma once

#include "GL/glew.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/vec3.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "GxTexture.h"
#include "GxSphere.h"
#include "Planet.h"
#include "GxBrackets.h"
#include "Physics.h"

class Satellite
{
public:
	Satellite();
	~Satellite();
	void BuildSatellite();
	void Draw();
	void SimpleDraw();
	void Update(double time_now);
	void SetObliquity(double degrees);
	double GetObliquity();
	void SetRotation(double s);
	double GetRotation();
	void SetSiderealPeriod(double s);
	double GetSiderealPeriod();
	void SetOrbitalInclination(double degrees);
	double GetOrbitalInclination();
	void SetMeanRadius(double km);
	double GetMeanRadius();
	void SetName(string name);
	string GetName();
	void SetEccentricity(double eccentricity);
	double GetEccentricity();
	void SetSemiMajorAxis(double axis);
	double GetSemiMajorAxis();
	void SetParent(Planet*& parent);
	Planet* GetParent();
	void SetTexture(const char* filename);
	string GetTextureName();
	void SetSelected(bool isSelected);
	bool GetSelected();
	glm::vec3 GetPosition();	
	void SetScale(double scale);
	void SetIsDead(bool isDead);
	bool GetIsDead();
	void SetColorId(uint32_t colorId);
	vec3b GetColor();
	void SetEulerAngles(glm::vec3 eAngles);
	double GetExpDist();

private:
	void DeleteLists();

	GxTexture texture;
	GLuint satelliteList;
	GLuint selectList;
	bool isSelected;
	string name;
	double mean_radius;
	double aphelion;
	double perihelion;
	double eccentricity;
	glm::vec3 position;
	double rotation;
	double obliquity;
	double sidereal_period;
	double orbital_inclination;
	double semimajor_axis;
	double rotation_angle;
	Planet* parent;
	bool isDead;
	uint32_t colorId;
	vec3b color;
	glm::vec3 eAngles;
};

#endif // !__SATELLITE_H__