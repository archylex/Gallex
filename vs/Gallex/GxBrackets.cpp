#include "GxBrackets.h"

void GxBrackets(const double radius, const double offset, const double part)
{
	double left = -radius * offset;
	double right = radius * offset;
	double bottom = -radius * offset;
	double top = radius * offset;
	double length_line = right / part;

	glBegin(GL_LINES);
	glVertex3d(left, bottom, 0.0f);
	glVertex3d(left + length_line, bottom, 0.0f);
	glVertex3d(left, bottom, 0.0f);
	glVertex3d(left, bottom + length_line, 0.0f);
	glVertex3d(left, top, 0.0f);
	glVertex3d(left + length_line, top, 0.0f);
	glVertex3d(left, top, 0.0f);
	glVertex3d(left, top - length_line, 0.0f);
	glVertex3d(right, top, 0.0f);
	glVertex3d(right, top - length_line, 0.0f);
	glVertex3d(right, top, 0.0f);
	glVertex3d(right - length_line, top, 0.0f);
	glVertex3d(right, bottom, 0.0f);
	glVertex3d(right - length_line, bottom, 0.0f);
	glVertex3d(right, bottom, 0.0f);
	glVertex3d(right, bottom + length_line, 0.0f);
	glEnd();
}