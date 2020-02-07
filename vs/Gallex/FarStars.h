#ifndef __FAR_STARS_H__
#define __FAR_STARS_H__

#pragma once

#include "FileUtils.h"
#include "GL/glew.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GxColorBuffer.h"
#include "GxTexture.h"

struct FARSTAR {
	double angle_x;
	double angle_y;
	vec3b color;
	double size;
};

class FarStars
{
public:
	FarStars();
	~FarStars();
	void Draw();
	
private:
	void LoadStars();
	void BuildStarsList();

	GLuint starslist;
	vector<FARSTAR> stars;
	GxTexture texture;
};

#endif // !__FAR_STARS_H__