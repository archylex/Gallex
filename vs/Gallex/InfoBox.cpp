#include "InfoBox.h"

InfoBox::InfoBox(const vector<string> lines)
{
	this->lines = lines;

	preloader = PreLoader::GetInstance();

	title_size = 18;
	main_size = 18;

	SetTitleColor(0xffffff);
	SetLineColor(0xffffff);

	titleText = new GxText();
	titleText->SetSize(title_size);

	mainText = new GxText();
	mainText->SetSize(main_size);

	double minSize = (preloader->GetScreen().width > preloader->GetScreen().height) ? preloader->GetScreen().height : preloader->GetScreen().width;
	border = minSize * 0.005f;

	infBox.left = preloader->GetScreen().width * 0.01f;
	infBox.right = infBox.left + preloader->GetScreen().width / 6;
	infBox.top = preloader->GetScreen().height * 0.99f;
	infBox.bottom = infBox.top - GetBoxHeight();
}

void InfoBox::Draw()
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
	glVertex2d(infBox.left - border, infBox.bottom - border);
	glVertex2d(infBox.right + border, infBox.bottom - border);
	glVertex2d(infBox.right + border, infBox.top + border);
	glVertex2d(infBox.left - border, infBox.top + border);
	glEnd();
	glBegin(GL_QUADS);
	glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
	glVertex2d(infBox.left, infBox.bottom);
	glVertex2d(infBox.right, infBox.bottom);
	glVertex2d(infBox.right, infBox.top);
	glVertex2d(infBox.left, infBox.top);
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
	
	glColor3ub(title_color.r, title_color.g, title_color.b);
	
	titleText->SetPosition((int)round(infBox.left + 2 * border), (int)round(infBox.top - border - title_size));
	titleText->SetText(lines[0]);
	titleText->Print();
	
	glColor3ub(main_color.r, main_color.g, main_color.b);

	for (unsigned int i = 1; i < lines.size(); i++)
	{		
		mainText->SetPosition((int)round(infBox.left + 2 * border), (int)round(infBox.top - border - title_size - i * main_size));
		mainText->SetText(lines[i]);
		mainText->Print();		
	}	

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void InfoBox::SetInfoBoxSize(double x, double y, double w, double h)
{
	infBox.left = x;
	infBox.right = w;
	infBox.top = y;
	infBox.bottom = h;
}

bool InfoBox::CheckIntersection(int x, int y)
{
	return (x > infBox.left&& x < infBox.right && y < (preloader->GetScreen().height - infBox.bottom) && y > (preloader->GetScreen().height - infBox.top));
}

void InfoBox::SetTitleSize(int title_size)
{
	this->title_size = title_size;
}

void InfoBox::SetLineSize(int main_size)
{
	this->main_size = main_size;
}

void InfoBox::SetTitleColor(uint32_t color)
{
	ColorLineToBytes(color, title_color);
}

void InfoBox::SetLineColor(uint32_t color)
{
	ColorLineToBytes(color, main_color);
}

int InfoBox::GetBoxHeight()
{
	return (int)round(title_size + (lines.size() - 1) * main_size + 2 * border);
}