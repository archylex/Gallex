#pragma once

#include <vector>
#include <sstream>
#include <string>
#include "FileUtils.h"
#include "StringUtils.h"
#include "GxTexture.h"
#include "GxShader.h"
#include "GxFont.h"
#include "GL/glew.h"
#include <gl/GL.h>

using namespace std;

struct GXSCREEN {
	int width = 1024;
	int height = 768;
	int depth = 32;
	int frequency = 60;
};

class PreLoader
{
public:
	~PreLoader();
	static PreLoader* GetInstance();
	void SetScreen(int width, int height, int depth, int freq);
	GXSCREEN GetScreen();
	GxTexture& GetFontTexture();
	GxTexture& GetBokehTexture();
	void LoadFont(string name);
	vector<FONTCHAR> GetFontChars();
	GLuint& GetFontGLList();
	GLvoid RemoveFont();
	void  LoadTextures();
	void UnloadTextures();
	void LoadShaders();
	GxShader& GetPerPixelShader();
	GxShader& GetBlurHorizontalShader();
	GxShader& GetBlurVerticalShader();
	GxShader& GetStarDepthTestShader();
	GxShader& GetStarFXShader();	
	void UnloadShaders();
	void LoadConfig(string fn);
	int GetRefreshMills();
	void SetRefreshMills(int refreshMills);

private:
	PreLoader();

	static PreLoader* instance;
	GXSCREEN screen;
	vector<FONTCHAR> fontChars;
	GLuint font_list;
	GxTexture font_texture;
	GxTexture bokeh_texture;
	GxShader perPixel;
	GxShader starDepthTest;
	GxShader starFX;
	GxShader blurHorizontal;
	GxShader blurVertical;
	int refreshMills;
};