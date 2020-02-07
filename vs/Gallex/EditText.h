#ifndef _EDIT_TEXT_H_
#define _EDIT_TEXT_H_

#pragma once

#include "PreLoader.h"
#include "GxText.h"
#include <vector>
#include <string>
#include <functional>

class EditText
{
public:
	EditText(string title, function<void()> callback);
	void Draw();
	void SetText(string text);
	void Finish();

private:
	PreLoader* preloader;
	SIZEBOX textEditBox;
	GxText* cText;
	string text;
	string title;
	function<void()> callback;
	double border;
	double title_size;
	double edit_size;
	unsigned int edit_length;
};

#endif // !_EDIT_TEXT_H_