#include "GxBackground.h"

GxBackground::GxBackground()
{
	texture.LoadTexture("data\\textures\\system\\background.jpg");
}

GxBackground::~GxBackground()
{
	texture.Remove();
}

void GxBackground::Draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex2f(1, 0.0f);
	glTexCoord2f(1, 0);
	glVertex2f(1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(0.0f, 1);
	glEnd();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}