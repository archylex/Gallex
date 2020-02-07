#include "GxText.h"

GxText::GxText()
{
	this->x = 0;
	this->y = 0;
	this->text = "";
	this->scale = 1;
	preloader = PreLoader::GetInstance();
}

GLvoid GxText::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

GLvoid GxText::SetText(string text)
{
	this->text = text;
}

GLvoid GxText::SetSize(double size)
{
	scale = size / preloader->GetFontChars()[0].height;
}

GLvoid GxText::Print()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, preloader->GetFontTexture());
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, preloader->GetScreen().width, 0, preloader->GetScreen().height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	glScaled(scale, scale, scale);
	glListBase(preloader->GetFontGLList());
	glCallLists((GLsizei)text.length(), GL_BYTE, (GLbyte*)text.c_str());
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

double GxText::GetLength()
{
	double length = 0;

	for (unsigned int i = 0; i < text.length(); i++)
	{
		for (FONTCHAR symbol : preloader->GetFontChars())
		{
			if (symbol.id == text[i])
			{
				length += symbol.width;
				break;
			}
		}
	}

	return length * scale;
}

double GxText::GetHeight()
{
	return preloader->GetFontChars()[0].height * scale;
}