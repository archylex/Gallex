#ifndef __GX_BACKGROUND_H__
#define __GX_BACKGROUND_H__

#pragma once

#include "GxTexture.h"

class GxBackground
{
public:
	GxBackground();
	~GxBackground();
	void Draw();

private:
	GxTexture texture;
};

#endif // !__GX_BACKGROUND_H__