#ifndef __MENU_H__
#define __MENU_H__

#pragma once

#include "PreLoader.h"
#include "GxText.h"
#include <vector>
#include <functional>

class Menu
{
public:
	Menu(const vector<const char*>& items, const vector<function<void()>> funcs);
	void Draw();
	void SetNextPosition();
	void SetPreviousPosition();
	void Finish();
	bool CheckIntersection(int x, int y);

private:
	vector<const char*> items;
	vector<function<void()>> funcs;
	int menu_pos;
	SIZEBOX dlgBox;
	double border;
	PreLoader* preloader;
	GxText* cText;
	int font_size;
};

#endif // !__MENU__