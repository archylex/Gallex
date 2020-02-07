#include "FarStars.h"

FarStars::FarStars()
{
	LoadStars();
	BuildStarsList();
	texture.LoadTexture("data\\textures\\system\\farstar.jpg");
}

FarStars::~FarStars()
{
	if (starslist)
		glDeleteLists(starslist, 1);
}

void FarStars::Draw()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glCallList(starslist);
	glEnable(GL_LIGHTING);
	glDisable(GL_ALPHA);
	glDisable(GL_BLEND);
}

void FarStars::LoadStars()
{
	vector<STARINFO> star_info = GetValuesFromStarFile("data\\stars.xml");
	
	for (STARINFO info : star_info)
	{
		FARSTAR star;
		star.size = info.magnitude;
		star.angle_x = info.declination;
		star.angle_y = info.right_ascension * 360 / 24;
		star.color = GetSpectralColor(info.spectral_class.c_str());
		stars.push_back(star);
	}
}

void FarStars::BuildStarsList()
{
	double PIover180 = M_PI / 180.0f;
	double radius = 1024.0f;
	double pt[4][3], v[3], a[3], b[3];

	if (starslist)
		glDeleteLists(starslist, 1);
	
	starslist = glGenLists(1);

	glNewList(starslist, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (FARSTAR star : stars)
	{
		double r = radius * (double)cos(star.angle_x * PIover180);
		a[0] = r * (double)cos((90 - star.angle_y) * PIover180);
		a[1] = radius * (double)sin(star.angle_x * PIover180);
		a[2] = r * (double)sin((90 - star.angle_y) * PIover180);
		v[0] = r * (double)cos((90 - star.angle_y - star.size) * PIover180);
		v[1] = radius * (double)sin(star.angle_x * PIover180);
		v[2] = r * (double)sin((90 - star.angle_y - star.size) * PIover180);
		b[0] = v[0] - a[0];
		b[1] = v[1] - a[1];
		b[2] = v[2] - a[2];
		double absolut1 = (double)sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
		if (absolut1 > 0)
		{
			b[0] = b[0] / absolut1;
			b[1] = b[1] / absolut1;
			b[2] = b[2] / absolut1;
		}
		v[0] = a[1] * b[2] - a[2] * b[1];
		v[1] = a[2] * b[0] - a[0] * b[2];
		v[2] = a[0] * b[1] - a[1] * b[0];
		double absolut = (double)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		if (absolut > 0)
		{
			v[0] = v[0] / absolut;
			v[1] = v[1] / absolut;
			v[2] = v[2] / absolut;
		}
		b[0] = b[0] * star.size / 2.0f;
		b[1] = b[1] * star.size / 2.0f;
		b[2] = b[2] * star.size / 2.0f;
		v[0] = v[0] * star.size / 2.0f;
		v[1] = v[1] * star.size / 2.0f;
		v[2] = v[2] * star.size / 2.0f;
		pt[0][0] = a[0] - b[0] - v[0];
		pt[0][1] = a[1] - b[1] - v[1];
		pt[0][2] = a[2] - b[2] - v[2];
		pt[1][0] = a[0] + b[0] - v[0];
		pt[1][1] = a[1] + b[1] - v[1];
		pt[1][2] = a[2] + b[2] - v[2];
		pt[2][0] = a[0] + b[0] + v[0];
		pt[2][1] = a[1] + b[1] + v[1];
		pt[2][2] = a[2] + b[2] + v[2];
		pt[3][0] = a[0] - b[0] + v[0];
		pt[3][1] = a[1] - b[1] + v[1];
		pt[3][2] = a[2] - b[2] + v[2];
		glColor3ub(star.color.r, star.color.g, star.color.b);
		glTexCoord2d(0, 0);
		glVertex3d(pt[0][0], pt[0][1], pt[0][2]);
		glTexCoord2d(1, 0);
		glVertex3d(pt[1][0], pt[1][1], pt[1][2]);
		glTexCoord2d(0, 1);
		glVertex3d(pt[3][0], pt[3][1], pt[3][2]);
		glTexCoord2d(1, 0);
		glVertex3d(pt[1][0], pt[1][1], pt[1][2]);
		glTexCoord2d(1, 1);
		glVertex3d(pt[2][0], pt[2][1], pt[2][2]);
		glTexCoord2d(0, 1);
		glVertex3d(pt[3][0], pt[3][1], pt[3][2]);
	}
	glEnd();
	glEndList();
}