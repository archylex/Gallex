#ifndef __RENDERING_H__
#define __RENDERING_H__

#pragma once

#include "GUI.h"
#include "GL/glut.h"
#include "PreLoader.h"
#include "GxColorBuffer.h"
#include "SpaceScene.h"

#define SELECT_BUFFER_SIZE 512
#define NONE_BTN_MOUSE 0
#define LEFT_BTN_MOUSE 1
#define RIGHT_BTN_MOUSE 2
#define MIDDLE_BTN_MOUSE 3

class Rendering
{
public:
	Rendering();
	void Display();
	void Destroy();
	void Render(GLuint mode);
	void Resize(const int width, const int height);
	void OnKeyPressed(int key);
	void OnMouseMove(int x, int y);	
	void ProcessColorBuffer(int x, int y);
	void SetTimeScale(const int ms);
	void OnMouseButtonDown(int x, int y, int btn);
	void OnMouseButtonUp(int x, int y, int btn);

private:
	PreLoader* preloader;
	SpaceScene* scene;		
	int lastX;
	int lastY;
	int mouseButton;
	GxCursor* gxCursor;
	GUI* gui;
	int frameCount;
	double startTime;
};

#endif // !__RENDERING_H__