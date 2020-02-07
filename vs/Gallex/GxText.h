#ifndef __GX_TEXT_H__
#define __GX_TEXT_H__

#pragma once

#include "PreLoader.h"
#include "GxFont.h"

struct SIZEBOX {
	double left;
	double right;
	double bottom;
	double top;
};

class GxText
{
public:
	GxText();
	GLvoid SetText(string text);
	GLvoid SetPosition(int x, int y);
	GLvoid SetSize(double size);
	GLvoid Print();
	double GetLength();
	double GetHeight();

private:
	int x;
	int y;
	string text;
	double scale;
	PreLoader* preloader;
};

#endif // !__GX_TEXT_H__