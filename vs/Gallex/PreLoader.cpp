#include "PreLoader.h"

PreLoader* PreLoader::instance = 0;

PreLoader* PreLoader::GetInstance()
{
	if (instance == 0)
		instance = new PreLoader();

	return instance;
}

PreLoader::PreLoader()
{	
	refreshMills = 25;
}

PreLoader::~PreLoader()
{
	UnloadShaders();
	UnloadTextures();
	RemoveFont();
}

void PreLoader::LoadConfig(string fn)
{
	string content = ReadFile("data\\" + fn);
	istringstream stream{ content };
	string line;

	while (getline(stream, line)) {
		char* value;

		strtok_s((char*)line.c_str(), "=", &value);

		if (strcmp(line.c_str(), "width") == 0)
			screen.width = atoi(value);

		if (strcmp(line.c_str(), "height") == 0)
			screen.height = atoi(value);

		if (strcmp(line.c_str(), "depth") == 0)
			screen.depth = atoi(value);

		if (strcmp(line.c_str(), "freq") == 0)
			screen.frequency = atoi(value);
	}
}

void PreLoader::SetScreen(int width, int height, int depth, int freq)
{
	screen.width = width;
	screen.height = height;
	screen.depth = depth;
	screen.frequency = freq;
}

GXSCREEN PreLoader::GetScreen()
{
	return screen;
}

void  PreLoader::LoadTextures()
{
	bokeh_texture.LoadTexture("data\\textures\\system\\bokeh.jpg");
}

void PreLoader::UnloadTextures()
{
	bokeh_texture.Remove();
}

void PreLoader::LoadShaders()
{
	starDepthTest.Load("star_depth_test.vs", "star_depth_test.fs");
	perPixel.Load("per_pixel_lighting.vs", "per_pixel_lighting.fs");
	blurHorizontal.Load("blur.vs", "blur_horizontal.fs");
	blurVertical.Load("blur.vs", "blur_vertical.fs");
	starFX.Load("star_fx.vs", "star_fx.fs");
}

void PreLoader::UnloadShaders()
{
	starDepthTest.Destroy();
	perPixel.Destroy();
	blurHorizontal.Destroy();
	blurVertical.Destroy();
	starFX.Destroy();
}

vector<FONTCHAR> PreLoader::GetFontChars()
{
	return fontChars;
}

GLuint& PreLoader::GetFontGLList()
{
	return font_list;
}

GLvoid PreLoader::RemoveFont()
{
	RemoveGLFont(font_list);
}

GxShader& PreLoader::GetPerPixelShader()
{
	return perPixel;
}

GxShader& PreLoader::GetBlurHorizontalShader()
{
	return blurHorizontal;
}

GxShader& PreLoader::GetBlurVerticalShader()
{
	return blurVertical;
}

GxShader& PreLoader::GetStarDepthTestShader()
{
	return starDepthTest;
}

GxShader& PreLoader::GetStarFXShader()
{
	return starFX;
}

void PreLoader::LoadFont(string name)
{
	LoadTextures();
	LoadShaders();
	CreateGLFont(name, font_list, font_texture, fontChars);
}

GxTexture& PreLoader::GetBokehTexture()
{
	return bokeh_texture;
}

GxTexture& PreLoader::GetFontTexture()
{
	return font_texture;
}

int PreLoader::GetRefreshMills()
{
	return refreshMills;
}

void PreLoader::SetRefreshMills(int refreshMills)
{
	this->refreshMills = (refreshMills > 0) ? refreshMills : 1;
}