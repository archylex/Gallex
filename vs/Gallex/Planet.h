#ifndef __PLANET_H__
#define __PLANET_H__

#pragma once

#include "GL/glew.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/ext/vector_double3.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "GxColorBuffer.h"
#include "Physics.h"
#include "GxTexture.h"
#include "GxSphere.h"
#include "Star.h"
#include "GxBrackets.h"

class Planet
{
public:
	Planet();
	~Planet();
	void BuildPlanet();
	void Draw();
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
	void SetParent(Star* parent);
	Star* GetParent();
	void SetTexture(const char* filename);
	string GetTextureName();
	void SetRingTexture(const char* filename);
	string GetRingTextureName();
	void SetSelected(bool isSelected);
	bool GetSelected();
	glm::vec3 GetPosition();
	void SetIsDead(bool isDead);
	bool GetIsDead();
	void SimpleDraw();
	void SetColorId(uint32_t colorId);
	vec3b GetColor();
	void SetEulerAngles(glm::vec3 eAngles);	
	void SetScale(double scale);
	double GetExpDist();
			
private:		
	void DeleteLists();
	void DeleteTextures();

	GxTexture texture;
	GxTexture ring_texture;
	GLuint planetList;
	GLuint selectList;
	bool isSelected;
	string name;
	double mean_radius;
	double eccentricity;
	glm::vec3 position;
	double rotation; // the rotation of planet around its own axis (hours)
	double obliquity; // the angle between an object's rotational axis and its orbital axis (degrees)
	double sidereal_period; // the time it takes for a planet to complete one orbit (hours)
	double orbital_inclination; // the angle between the ecliptic plane and the celestial equator (degrees)
	double semimajor_axis;
	double rotation_angle;
	Star* parent;
	bool isDead;	
	uint32_t colorId;
	vec3b color;			
	glm::vec3 eAngles;
};

#endif // !__PLANET_H__