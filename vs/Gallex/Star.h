#ifndef __STAR_H__
#define __STAR_H__

#pragma once

#include "GL/glew.h"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "PreLoader.h"
#include "GxTexture.h"
#include "GxColorBuffer.h"
#include "GxShader.h"
#include "GxBrackets.h"
#include "GxCamera.h"
#include "GxSphere.h"

#define TESTS 12
#define ANGLE_STEP 360.0f / TESTS

class Star
{
public:
	Star();
	~Star();
	void BuildStar();
	void Draw(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, GxCamera& camera);
	void SimpleDraw();
	void Update(double time_now);
	void Resize();
	void DeleteLists();
	void SetScale(double scale);
	void SetPosition(glm::vec3 pos);
	void SetPosition(double x, double y, double z);
	glm::vec3 GetPosition();
	void SetObliquity(double degrees);
	double GetObliquity();
	void SetRotation(double s);
	double GetRotation();
	void SetEquatorialRadius(double km);
	double GetEquatorialRadius();
	void SetName(string name);
	string GetName();
	void SetSpectral(string spectral);
	string GetSpectral();	
	void SetSelected(bool isSelected);
	bool GetSelected();	
	void SetIsDead(bool isDead);
	bool GetIsDead();
	void SetColorId(uint32_t colorId);
	vec3b GetColor();
	void SetEulerAngles(glm::vec3 eAngles);
	double GetExpDist();
		
private:
	PreLoader* preloader;
	string name;
	string spectral;
	double equatorial_radius;
	double obliquity; // to the ecliptic
	double rotation; // the rotation of star around its own axis (days)
	glm::vec3 position;
	vec3b color;
	vec3b colorId;
	GLuint FBO;
	GLuint DepthTexture;
	GLuint StarTextures[5];
	int starTextureWidth;
	int starTextureHeight;
	GLuint sphereList;
	GLuint selectList;
	bool isSelected;
	double rotation_angle;
	bool isDead;
	glm::vec3 eAngles;
};

#endif // !__STAR_H__