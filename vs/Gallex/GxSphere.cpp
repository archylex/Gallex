#include "GxSphere.h"

void GxSphere(const double radius, const unsigned int sections)
{
	for (unsigned int h_edge = 0; h_edge < sections; h_edge++)
	{
		double texCoordY0 = (double)h_edge / (double)sections;
		double texCoordY1 = (double)(h_edge + 1) / (double)sections;
		double coordSinY0 = (double)sin((double)(texCoordY0 * M_PI));
		double coordSinY1 = (double)sin((double)(texCoordY1 * M_PI));
		double coordCosY0 = (double)cos((double)(texCoordY0 * M_PI));
		double coordCosY1 = (double)cos((double)(texCoordY1 * M_PI));

		for (unsigned int v_edge = 0; v_edge < sections; v_edge++)
		{
			double texCoordX0 = (double)v_edge / (double)sections;
			double texCoordX1 = (double)(v_edge + 1) / (double)sections;
			double coordSinX0 = (double)sin((double)(texCoordX0 * 2 * M_PI));
			double coordSinX1 = (double)sin((double)(texCoordX1 * 2 * M_PI));
			double coordCosX0 = (double)cos((double)(texCoordX0 * 2 * M_PI));
			double coordCosX1 = (double)cos((double)(texCoordX1 * 2 * M_PI));

			double point[4][3];

			point[0][0] = radius * coordSinX0 * coordSinY0;
			point[0][1] = -radius * coordCosY0;
			point[0][2] = radius * coordCosX0 * coordSinY0;
			point[1][0] = radius * coordSinX1 * coordSinY0;
			point[1][1] = -radius * coordCosY0;
			point[1][2] = radius * coordCosX1 * coordSinY0;
			point[2][0] = radius * coordSinX1 * coordSinY1;
			point[2][1] = -radius * coordCosY1;
			point[2][2] = radius * coordCosX1 * coordSinY1;
			point[3][0] = radius * coordSinX0 * coordSinY1;
			point[3][1] = -radius * coordCosY1;
			point[3][2] = radius * coordCosX0 * coordSinY1;

			double normal[4][3];

			for (int i = 0; i < 4; i++)
			{
				double absolut = (double)sqrt(point[i][0] * point[i][0] + point[i][1] * point[i][1] + point[i][2] * point[i][2]);

				if (absolut > 0)
				{
					normal[i][0] = point[i][0] / absolut;
					normal[i][1] = point[i][1] / absolut;
					normal[i][2] = point[i][2] / absolut;
				}
			}

			glBegin(GL_TRIANGLE_STRIP);
			glNormal3d(normal[0][0], normal[0][1], normal[0][2]);
			glTexCoord2d(texCoordX0, texCoordY0);
			glVertex3d(point[0][0], point[0][1], point[0][2]);
			glNormal3d(normal[1][0], normal[1][1], normal[1][2]);
			glTexCoord2d(texCoordX1, texCoordY0);
			glVertex3d(point[1][0], point[1][1], point[1][2]);
			glNormal3d(normal[3][0], normal[3][1], normal[3][2]);
			glTexCoord2d(texCoordX0, texCoordY1);
			glVertex3d(point[3][0], point[3][1], point[3][2]);
			glNormal3d(normal[2][0], normal[2][1], normal[2][2]);
			glTexCoord2d(texCoordX1, texCoordY1);
			glVertex3d(point[2][0], point[2][1], point[2][2]);
			glEnd();
		}
	}
}