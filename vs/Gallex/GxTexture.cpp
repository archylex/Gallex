#include "GxTexture.h"

GxTexture::GxTexture()
{
	texture = 0;
}

GxTexture::operator GLuint ()
{
	return texture;
}

bool GxTexture::LoadTexture(const string filename)
{
	size_t found = filename.find_last_of("\\");
	string strname = filename.substr(found + 1);
	filename_string = strname;
	
	Remove();

	int BPP;
	string DirectoryFileName = GetModuleDirectory() + filename;
	FIBITMAP* dib = GetBitmap((char*)DirectoryFileName.c_str(), BPP);

	if (dib == NULL)
		return false;

	GLenum format = 0;

	if (BPP == 32) format = GL_BGRA;
	if (BPP == 24) format = GL_BGR;

	if (format == 0)
	{
		FreeImage_Unload(dib);
		return false;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, FreeImage_GetBits(dib));
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(dib);

	return true;
}

void GxTexture::Remove()
{
	glDeleteTextures(1, &texture);
	texture = 0;
}

FIBITMAP* GxTexture::GetBitmap(const string filename, int& BPP)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());

	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename.c_str());

	if (fif == FIF_UNKNOWN)
		return NULL;

	FIBITMAP* dib = NULL;

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename.c_str());

	if (dib != NULL)
	{
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);

		if (width == 0 || height == 0)
		{
			FreeImage_Unload(dib);
			return NULL;
		}

		BPP = FreeImage_GetBPP(dib);

	}

	return dib;
}

int GxTexture::GetWidth()
{
	return width;
}

int GxTexture::GetHeight()
{
	return height;
}

string GxTexture::GetTextureName()
{
	return filename_string;
}