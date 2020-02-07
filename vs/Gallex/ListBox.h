#ifndef __LIST_BOX__
#define __LIST_BOX__

#pragma once

#include <vector>
#include <functional>
#include <string>
#include "PreLoader.h"
#include "GxText.h"

class ListBox
{
public:
	ListBox(vector<string>& items, function<void(string)> callback);
	void Draw();
	void SetNextPosition();
	void SetPreviousPosition();
	void EnterSelectedItem();
	void CalculatePos();
	bool CheckIntersection(int x, int y);

private:
	vector<string> items;
	int vec_pos;
	SIZEBOX listBox;
	double border;
	int list_pos;
	unsigned int start_range;
	unsigned int end_range;
	function <void(string)> callback;
	PreLoader* preloader;
	GxText* cText;
	int font_size;
};

#endif // !__SL_LIST_BOX__