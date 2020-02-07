#ifndef __GX_SHADER_H__
#define __GX_SHADER_H__

#pragma once

#include <string>
#include "GL/glew.h"
#include "FileUtils.h"

using namespace std;

class GxShader
{
public:
	GxShader();
	operator GLuint ();
	bool Load(const char* vertexFilename, const char* fragmentFilename);
	void Destroy();

private:
	GLuint LoadShader(const string filename, GLenum _type);
	void SetDefaults();

	std::string modDir;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	GLuint* UniformLocations;
	GLuint* AttribLocations;
};

#endif // !__GX_SHADER_H__