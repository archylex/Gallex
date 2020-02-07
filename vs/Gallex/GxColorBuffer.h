#ifndef __GX_COLOR_BUFFER_H__
#define __GX_COLOR_BUFFER_H__

#pragma once

#include "GL/glew.h"
#include <cstdint>

#define COLOR_BUFFER_SIZE   32

using namespace std;

struct vec3b
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

bool CheckColor(const vec3b& one, const vec3b& two);
void ColorLineToBytes(const uint32_t& src, vec3b& dist);
vec3b GetSpectralColor(const char* spectral);

#endif // !__GX_COLOR_BUFFER_H__