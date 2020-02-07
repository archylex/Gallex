#ifndef __DLG_BOX_H__
#define __DLG_BOX_H__

#pragma once

#include "PreLoader.h"
#include "GxText.h"
#include <vector>
#include <string>
#include <functional>

class DlgBox
{
public:
	DlgBox(string title, string text, function<void(int)> callback, int mode);
	void Draw();
	void Finish();

private:
	PreLoader* preloader;
	SIZEBOX textEditBox;
	GxText* titleText;
	GxText* mainText;
	double border;
	double title_size;
	double edit_size;
	unsigned int edit_length;
	function<void(int)> callback;
	int mode;
};

#endif // !__DLG_BOX_H__