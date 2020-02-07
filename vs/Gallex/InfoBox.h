#ifndef __INFO_BOX_H__
#define __INFO_BOX_H__

#pragma once

#include "PreLoader.h"
#include "GxText.h"
#include "GxColorBuffer.h"
#include <vector>
#include <string>

class InfoBox
{
public:
	InfoBox(const vector<string> lines);
	void Draw();
	void SetInfoBoxSize(double x, double y, double w, double h);
	void SetTitleSize(int title_size);
	void SetLineSize(int main_size);
	void SetTitleColor(uint32_t color);
	void SetLineColor(uint32_t color);
	int GetBoxHeight();
	bool CheckIntersection(int x, int y);

private:
	PreLoader* preloader;
	SIZEBOX infBox;
	GxText* titleText;
	GxText* mainText;
	double border;
	double title_size;
	double main_size;
	vector<string> lines;
	vec3b title_color;
	vec3b main_color;
};

#endif // !__INFO_BOX_H__