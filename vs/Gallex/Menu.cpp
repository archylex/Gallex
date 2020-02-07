#include "Menu.h"

Menu::Menu(const vector<const char*>& items, const vector<function<void()>> funcs)
{
	this->items = items;
	this->funcs = funcs;
	this->menu_pos = 0;

	preloader = PreLoader::GetInstance();

	double minSize = (preloader->GetScreen().width > preloader->GetScreen().height) ? preloader->GetScreen().height : preloader->GetScreen().width;
	border = minSize * 0.005f;

	font_size = 32;

	cText = new GxText();
	cText->SetSize(font_size);

	double max_len_text = 0;

	for (const char* s : items)
	{
		cText->SetText(s);
		double len = cText->GetLength();
		if (len > max_len_text)
			max_len_text = len;
	}

	max_len_text *= 3;

	double window_height = items.size() * font_size;
	dlgBox.left = (preloader->GetScreen().width - max_len_text) / 2 - border;
	dlgBox.right = dlgBox.left + max_len_text + border;
	dlgBox.bottom = (preloader->GetScreen().height - window_height) / 2 - border;
	dlgBox.top = dlgBox.bottom + window_height + border;
}

void Menu::Draw()
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
	glVertex2d(dlgBox.left - border, dlgBox.bottom - 2 * border);
	glVertex2d(dlgBox.right + border, dlgBox.bottom - 2 * border);
	glVertex2d(dlgBox.right + border, dlgBox.top + border);
	glVertex2d(dlgBox.left - border, dlgBox.top + border);
	glEnd();
	glBegin(GL_QUADS);
	glColor4f(0.2f, 0.2f, 0.2f, 0.75f);
	glVertex2d(dlgBox.left, dlgBox.bottom - border);
	glVertex2d(dlgBox.right, dlgBox.bottom - border);
	glVertex2d(dlgBox.right, dlgBox.top);
	glVertex2d(dlgBox.left, dlgBox.top);
	glEnd();
	glBegin(GL_QUADS);
	glColor4f(0.5f, 0.5f, 0.5f, 0.75f);
	glVertex2d(dlgBox.left + border, dlgBox.top - border - font_size * (menu_pos + 1));
	glVertex2d(dlgBox.right - border, dlgBox.top - border - font_size * (menu_pos + 1));
	glVertex2d(dlgBox.right - border, dlgBox.top - border - font_size * menu_pos);
	glVertex2d(dlgBox.left + border, dlgBox.top - border - font_size * menu_pos);
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

	for (unsigned int i = 0; i < items.size(); ++i)
	{
		cText->SetText(items[i]);
		cText->SetPosition((int)round((dlgBox.left + dlgBox.right - cText->GetLength()) / 2), (int)round(dlgBox.top - border - cText->GetHeight() - i * cText->GetHeight()));
		cText->Print();
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void Menu::SetNextPosition()
{
	this->menu_pos++;
	this->menu_pos = ((size_t)menu_pos >= items.size()) ? 0 : menu_pos;
}

void Menu::SetPreviousPosition()
{
	this->menu_pos--;
	this->menu_pos = (menu_pos < 0) ? items.size() - 1 : menu_pos;
}

void Menu::Finish()
{
	funcs[menu_pos]();
}

bool Menu::CheckIntersection(int x, int y)
{
	for (int i = 0; i < items.size(); i++)
	{
		int x1 = dlgBox.left + border;
		int y1 = preloader->GetScreen().height - (dlgBox.top - border - font_size * (i + 1));
		int x2 = dlgBox.right - border;
		int y2 = preloader->GetScreen().height - (dlgBox.top - border - font_size * i);
			   
		if (x > x1&& x < x2 && y > y2 && y < y1)
		{
			menu_pos = i;
			return true;
		}
	}

	return false;
}