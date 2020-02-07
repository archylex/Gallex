#include "DlgBox.h"

DlgBox::DlgBox(string title, string text, function<void(int)> callback, int mode)
{
	this->callback = callback;
	this->mode = mode;
	this->preloader = PreLoader::GetInstance();
	
	title_size = 42;
	edit_size = 32;
	edit_length = 32;

	titleText = new GxText();
	titleText->SetText(title);
	titleText->SetSize(title_size);
	
	mainText = new GxText();
	mainText->SetText(text);
	mainText->SetSize(edit_size);
	
	double minSize = (preloader->GetScreen().width > preloader->GetScreen().height) ? preloader->GetScreen().height : preloader->GetScreen().width;
	border = minSize * 0.005f;

	double h = title_size + edit_size + 5 * border;

	textEditBox.left = (preloader->GetScreen().width - mainText->GetLength()) / 2 - 3 * border;
	textEditBox.right = (preloader->GetScreen().width + mainText->GetLength()) / 2 + 3 * border;
	textEditBox.bottom = (preloader->GetScreen().height - h) / 2;
	textEditBox.top = textEditBox.bottom + h;

	titleText->SetPosition((int)round((textEditBox.left + textEditBox.right - titleText->GetLength()) / 2), (int)round(textEditBox.top - titleText->GetHeight() - border));
	mainText->SetPosition((int)round((textEditBox.left + textEditBox.right - mainText->GetLength()) / 2), (int)round(textEditBox.bottom + 2 * border));
}

void DlgBox::Draw()
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
	glVertex2d(textEditBox.left - border, textEditBox.bottom - border);
	glVertex2d(textEditBox.right + border, textEditBox.bottom - border);
	glVertex2d(textEditBox.right + border, textEditBox.top + border);
	glVertex2d(textEditBox.left - border, textEditBox.top + border);
	glEnd();
	glBegin(GL_QUADS);
	glColor4f(0.2f, 0.2f, 0.2f, 0.75f);
	glVertex2d(textEditBox.left, textEditBox.bottom);
	glVertex2d(textEditBox.right, textEditBox.bottom);
	glVertex2d(textEditBox.right, textEditBox.top);
	glVertex2d(textEditBox.left, textEditBox.top);
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
	titleText->Print();
		
	glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
	mainText->Print();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void DlgBox::Finish()
{
	callback(mode);
}