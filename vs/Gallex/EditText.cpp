#include "EditText.h"

EditText::EditText(string title, function<void()> callback)
{
	this->title = title;
	this->callback = callback;
	this->preloader = PreLoader::GetInstance();

	title_size = 32;
	edit_size = 32;
	edit_length = 32;

	cText = new GxText();
	cText->SetText(title);
	cText->SetSize(title_size);

	double minSize = (preloader->GetScreen().width > preloader->GetScreen().height) ? preloader->GetScreen().height : preloader->GetScreen().width;
	border = minSize * 0.005f;

	double h = 32 + 24 + 5 * border;

	textEditBox.left = preloader->GetScreen().width * 3 / 8;
	textEditBox.right = preloader->GetScreen().width * 5 / 8;
	textEditBox.bottom = (preloader->GetScreen().height - h) / 2;
	textEditBox.top = textEditBox.bottom + h;
}

void EditText::Draw()
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
	glBegin(GL_QUADS);
	glColor4f(0.5f, 0.5f, 0.5f, 0.75f);
	glVertex2d(textEditBox.left + border, textEditBox.bottom + border);
	glVertex2d(textEditBox.right - border, textEditBox.bottom + border);
	glVertex2d(textEditBox.right - border, textEditBox.bottom + edit_size + border);
	glVertex2d(textEditBox.left + border, textEditBox.bottom + edit_size + border);
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

	cText->SetText(title);
	cText->SetSize(title_size);
	cText->SetPosition((int)round((textEditBox.left + textEditBox.right - cText->GetLength()) / 2), (int)round(textEditBox.top - cText->GetHeight() - border));
	cText->Print();
	
	cText->SetText(text);
	cText->SetSize(edit_size);
	cText->SetPosition((int)round(textEditBox.left + 2 * border), (int)round(textEditBox.bottom + border));
	cText->Print();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void EditText::SetText(string text)
{
	if (text.size() > edit_length)
	{
		string new_text = text.substr(text.size() - edit_length, text.size());
		this->text = new_text;
	}
	else {
		this->text = text;
	}
}

void EditText::Finish()
{
	callback();
}