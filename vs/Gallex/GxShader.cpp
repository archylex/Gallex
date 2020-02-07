#include "GxShader.h"

GxShader::GxShader()
{
	SetDefaults();
	modDir = GetModuleDirectory();
}

GxShader::operator GLuint()
{
	return program;
}

bool GxShader::Load(const char* vertexFilename, const char* fragmentFilename)
{
	Destroy();

	vertexShader = LoadShader(vertexFilename, GL_VERTEX_SHADER);

	if (vertexShader == 0)
		return false;

	fragmentShader = LoadShader(fragmentFilename, GL_FRAGMENT_SHADER);

	if (fragmentShader == 0)
		return false;

	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int LinkStatus;

	glGetProgramiv(program, GL_LINK_STATUS, &LinkStatus);

	if (LinkStatus == GL_FALSE)
	{
		Destroy();
		return false;
	}

	return true;
}

void GxShader::Destroy()
{
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(program);

	delete[] UniformLocations;
	delete[] AttribLocations;

	SetDefaults();
}

GLuint GxShader::LoadShader(const string filename, GLenum _type)
{
	char* source = ReadFile("data\\shaders\\" + filename);

	GLuint shader = glCreateShader(_type);

	glShaderSource(shader, 1, (const char**)&source, NULL);

	glCompileShader(shader);

	delete[] source;

	int compileStatus;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		Destroy();
		return 0;
	}

	return shader;
}

void GxShader::SetDefaults()
{
	UniformLocations = NULL;
	AttribLocations = NULL;
	vertexShader = 0;
	fragmentShader = 0;
	program = 0;
}