#ifndef __GX_TEXTURE_H__
#define __GX_TEXTURE_H__

#pragma once

#include "GL/glew.h"
#include "FreeImage/FreeImage.h"
#include "FileUtils.h"

class GxTexture
{
public:
	GxTexture();
	operator GLuint ();
	bool LoadTexture(const string filename);
	void Remove();
	int GetWidth();
	int GetHeight();
	string GetTextureName();

private:
	FIBITMAP* GetBitmap(const string filename, int& BPP);

	GLuint texture;
	int width;
	int height;
	string filename_string;
};

#endif // !__GX_TEXTURE_H__