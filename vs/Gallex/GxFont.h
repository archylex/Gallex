#ifndef __GX_FONT_H__
#define __GX_FONT_H__

#pragma once

#include "GL/glew.h"
#include "FileUtils.h"
#include "StringUtils.h"
#include "GxTexture.h"
#include <string>

struct FONTCHAR {
	double width;
	double height;
	double id;
};

void CreateGLFont(const string& name, GLuint& list, GxTexture& texture, vector<FONTCHAR>& fontChar);
void RemoveGLFont(GLuint& list);

#endif // !__GX_FONT_H__