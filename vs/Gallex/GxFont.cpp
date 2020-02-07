#include "GxFont.h"

void CreateGLFont(const string& name, GLuint& list, GxTexture& texture, vector<FONTCHAR>& fontChar)
{
	string texture_filename = "data\\fonts\\" + name + ".jpg";
	string xml_filename = "data\\fonts\\" + name + ".xml";
	vector<string> charLines = GetXMLDataWithTag(ReadFile(xml_filename.c_str()), "char");
	vector<string> spacingLines = GetXMLDataWithTag(ReadFile(xml_filename.c_str()), "spacing");

	texture.LoadTexture(texture_filename);

	list = glGenLists(128);

	glBindTexture(GL_TEXTURE_2D, texture);

	for (unsigned int a = 0; a < charLines.size(); a++)
	{
		vector<string> v = GetValuesFromXMLData(charLines[a]);
		vector<string> sv = GetValuesFromXMLData(spacingLines[a]);

		double texture_left_x = stod(v[1].c_str()) / texture.GetWidth();
		double texture_bottom_y = 1 - (stod(v[2].c_str()) + stod(v[4].c_str())) / texture.GetHeight();
		double texture_right_x = (stod(v[1].c_str()) + stod(sv[2].c_str())) / texture.GetWidth();
		double texture_top_y = 1 - stod(v[2].c_str()) / texture.GetHeight();
		double left_spacing = stod(sv[1].c_str());
		double right_spacing = stod(sv[2].c_str()) + stod(sv[3].c_str());

		FONTCHAR symbol;
		symbol.width = left_spacing + right_spacing;
		symbol.height = stod(v[4].c_str());
		symbol.id = stod(v[0].c_str());
		fontChar.push_back(symbol);

		glNewList(list + (GLuint)atoi(v[0].c_str()), GL_COMPILE);
		glTranslated(left_spacing, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2d(texture_left_x, texture_bottom_y);
		glVertex2d(0, 0);
		glTexCoord2d(texture_right_x, texture_bottom_y);
		glVertex2d(stod(sv[2].c_str()), 0);
		glTexCoord2d(texture_right_x, texture_top_y);
		glVertex2d(stod(sv[2].c_str()), stod(v[4].c_str()));
		glTexCoord2d(texture_left_x, texture_top_y);
		glVertex2d(0, stod(v[4].c_str()));
		glEnd();
		glTranslated(right_spacing, 0, 0);
		glEndList();
	}
}

void RemoveGLFont(GLuint& list)
{
	glDeleteLists(list, 128);
}