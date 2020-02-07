#include "GxCursor.h"

GxCursor::GxCursor()
{
	preloader = PreLoader::GetInstance();
	texture.LoadTexture("data\\textures\\system\\cursor.png");
	cursor_size = 30;
}

GxCursor::~GxCursor()
{
	texture.Remove();
}

void GxCursor::Draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, preloader->GetScreen().width, preloader->GetScreen().height, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslated(position.x, position.y, 0);
	glColor3d(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2d(0, 0);
	glTexCoord2f(1, 1);
	glVertex2d(cursor_size, 0);
	glTexCoord2f(1, 0);
	glVertex2d(cursor_size, cursor_size);
	glTexCoord2f(0, 0);
	glVertex2d(0, cursor_size);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA);
}

void GxCursor::SetSize(int size)
{
	cursor_size = size;
}

void GxCursor::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;
	
	edge = 0;
		
	if (y <= 10) edge = 1;
	if (y >= preloader->GetScreen().height - 10) edge = 2;
	if (x <= 10) edge = 3;
	if (x >= preloader->GetScreen().width - 10) edge = 4;
}

POS2D GxCursor::GetPosition()
{
	return position;
}

int GxCursor::GetEdge()
{
	return edge;
}