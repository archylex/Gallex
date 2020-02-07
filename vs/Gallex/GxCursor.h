#ifndef __GX_CURSOR_H__
#define __GX_CURSOR_H__

#pragma once

#include "GL/glew.h"
#include "GxColorBuffer.h"
#include "GxTexture.h"
#include "PreLoader.h"

#define TOP_EDGE 2
#define LEFT_EDGE 3
#define RIGHT_EDGE 4
#define BOTTOM_EDGE 1

struct POS2D {
	int x;
	int y;
};

class GxCursor
{
public:
	GxCursor();
	~GxCursor();
	void Draw();
	void SetPosition(int x, int y);
	POS2D GetPosition();
	void SetSize(int size);
	int GetEdge();

private:
	GxTexture texture;
	POS2D position;
	int cursor_size;
	PreLoader* preloader;
	int edge;
};

#endif // !__GX_CURSOR_H__