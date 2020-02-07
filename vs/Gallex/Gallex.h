#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <windows.h>
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLu.h>
#include "GL/glut.h"
#include "Rendering.h"
#include "PreLoader.h"

#pragma comment (lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "FreeImage.lib")

#define WHEEL_UP 3
#define WHEEL_DOWN 4

using namespace std;

void Display();
void Reshape(int width, int height);
void MouseAction(int button, int state, int x, int y);
void MouseMotion(int x, int y);

PreLoader* preloader;
Rendering* rendering;