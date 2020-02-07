#include "ListBox.h"

#define DEADLINE 8

ListBox::ListBox(vector<string>& items, function<void(string)> callback)
{
	preloader = PreLoader::GetInstance();

	font_size = 32;

	cText = new GxText();
	cText->SetSize(font_size);

	this->items = items;
	this->vec_pos = 0;
	this->list_pos = 0;
	this->start_range = 0;
	this->end_range = DEADLINE;
	this->callback = callback;

	CalculatePos();

	double minSize = (preloader->GetScreen().width > preloader->GetScreen().height) ? preloader->GetScreen().height : preloader->GetScreen().width;
	border = minSize * 0.005f;

	double len_text = preloader->GetScreen().width / 4;
	double window_height = font_size * 9 + 2 * border;

	this->listBox.left = (preloader->GetScreen().width - len_text) / 2;
	this->listBox.right = this->listBox.left + len_text;
	this->listBox.bottom = (preloader->GetScreen().height - window_height) / 2;
	this->listBox.top = this->listBox.bottom + window_height;

}

void ListBox::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, preloader->GetScreen().width, 0, preloader->GetScreen().height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex2d(listBox.left - border, listBox.bottom - border);
	glVertex2d(listBox.right + border, listBox.bottom - border);
	glVertex2d(listBox.right + border, listBox.top + border);
	glVertex2d(listBox.left - border, listBox.top + border);
	glEnd();
	glBegin(GL_QUADS);
	glColor4f(0.2f, 0.2f, 0.2f, 0.75f);
	glVertex2d(listBox.left, listBox.bottom);
	glVertex2d(listBox.right, listBox.bottom);
	glVertex2d(listBox.right, listBox.top);
	glVertex2d(listBox.left, listBox.top);
	glEnd();
	glBegin(GL_QUADS);
	glColor4f(0.5f, 0.5f, 0.5f, 0.75f);
	glVertex2d(listBox.left + border, listBox.top - font_size - font_size * (vec_pos - start_range) - border);
	glVertex2d(listBox.right - border, listBox.top - font_size - font_size * (vec_pos - start_range) - border);
	glVertex2d(listBox.right - border, listBox.top - font_size * (vec_pos - start_range) - border);
	glVertex2d(listBox.left + border, listBox.top - font_size * (vec_pos - start_range) - border);
	glEnd();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, preloader->GetScreen().width, 0, preloader->GetScreen().height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (items.size() > 0)
	{
		for (unsigned int i = start_range; i <= end_range; i++)
		{
			cText->SetText(items[i].c_str());
			cText->SetPosition((int)round(listBox.left + 2 * border), (int)round(listBox.top - font_size - (i - start_range) * font_size - border / 2));
			cText->Print();
		}
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void ListBox::CalculatePos()
{
	start_range = vec_pos - list_pos;
	end_range = (items.size() >= DEADLINE) ? start_range + DEADLINE : items.size() - 1;
}

void ListBox::SetNextPosition()
{
	this->vec_pos++;

	this->list_pos = (list_pos >= DEADLINE) ? DEADLINE : this->list_pos + 1;
		
	if ((size_t)list_pos >= items.size())
		list_pos = items.size() - 1;

	this->vec_pos = ((size_t)vec_pos >= items.size()) ? items.size() - 1 : vec_pos;

	CalculatePos();
}

void ListBox::SetPreviousPosition()
{
	this->vec_pos--;
	this->list_pos = (list_pos <= 0) ? 0 : list_pos - 1;
	this->vec_pos = (vec_pos < 0) ? 0 : vec_pos;
	CalculatePos();
}

void ListBox::EnterSelectedItem()
{
	callback(items[vec_pos]);
}

bool ListBox::CheckIntersection(int x, int y)
{
	for (unsigned int i = start_range; i <= end_range; i++)
	{
		int x1 = listBox.left + border; 
		int x2 = listBox.right - border;
		int y1 = preloader->GetScreen().height - (listBox.top - font_size - font_size * (i - start_range) - border);
		int y2 = preloader->GetScreen().height - (listBox.top - font_size * (i - start_range) - border);
		
		if (x > x1 && x < x2 && y > y2 && y < y1)
		{
			list_pos = i - start_range;
			vec_pos = i;
			return true;
		}
	}

	return false;
}