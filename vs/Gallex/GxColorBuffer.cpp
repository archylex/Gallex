#include "GxColorBuffer.h"

bool CheckColor(const vec3b& one, const vec3b& two)
{
	if (one.r == two.r && one.g == two.g && one.b == two.b)
		return true;
	return false;
}

void ColorLineToBytes(const uint32_t& src, vec3b& dist)
{
	dist.b = (src & 0xFF) >> 0;
	dist.g = (src & 0xFF00) >> 8;
	dist.r = (src & 0xFF0000) >> 16;
}

vec3b GetSpectralColor(const char* spectral)
{
	vec3b color = {0, 0, 0};

	switch (spectral[0])
	{
	case 'O':
		color.r = 100;
		color.g = 111;
		color.b = 253;
		break;
	case 'B':
		color.r = 138;
		color.g = 150;
		color.b = 252;
		break;
	case 'A':
		color.r = 202;
		color.g = 205;
		color.b = 254;
		break;
	case 'F':
		color.r = 254;
		color.g = 254;
		color.b = 254;
		break;
	case 'G':
		color.r = 254;
		color.g = 246;
		color.b = 99;
		break;
	case 'K':
		color.r = 254;
		color.g = 169;
		color.b = 82;
		break;
	case 'M':
		color.r = 254;
		color.g = 100;
		color.b = 66;
		break;
	}

	return color;
}