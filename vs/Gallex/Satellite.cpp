#include "Satellite.h"

Satellite::Satellite()
{
	texture.LoadTexture("Moon.jpg");

	name = "Satellite";
	mean_radius = 0.1;
	obliquity = 0;
	rotation = 0;
	orbital_inclination = 0;
	sidereal_period = 0;
	aphelion = 0;
	perihelion = 0;
	semimajor_axis = 20;
	eccentricity = 0.7;
	isSelected = false;
}

Satellite::~Satellite()
{
	DeleteLists();
	texture.Remove();
}

void Satellite::SetTexture(const char* filename)
{
	texture.LoadTexture(filename);
}

string Satellite::GetTextureName()
{
	return texture.GetTextureName();
}

void Satellite::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(rotation_angle, (GLfloat)sin(obliquity), (GLfloat)cos(obliquity), 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glCallList(satelliteList);
	glPopMatrix();

	if (isSelected)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glPushMatrix();
		glColor3f(0.7f, 0.85f, 1.0f);
		glTranslated(position.x, position.y, position.z);
		glRotated(glm::degrees(eAngles.x), 1, 0, 0);
		glRotated(glm::degrees(eAngles.y), 0, 1, 0);
		glRotated(glm::degrees(eAngles.z), 0, 0, 1);
		glCallList(selectList);
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void Satellite::SimpleDraw()
{
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(rotation_angle, (GLfloat)sin(obliquity), (GLfloat)cos(obliquity), 0);
	glColor3ub(color.r, color.g, color.b);
	glCallList(satelliteList);
	glPopMatrix();
}

void Satellite::Update(double time_now)
{
	rotation_angle = (rotation > 0) ? 360.0f / rotation * time_now : 0;

	double true_anomaly = GetTrueAnomaly(eccentricity, sidereal_period, time_now);
	vec2d point = Get2DCoord(semimajor_axis, eccentricity, true_anomaly);
	
	position.x = parent->GetPosition().x + point.x * cos(orbital_inclination);
	position.y = parent->GetPosition().y + point.x * sin(orbital_inclination);
	position.z = parent->GetPosition().z + point.y;
}

void Satellite::SetEulerAngles(glm::vec3 eAngles)
{
	this->eAngles = eAngles;
}

void Satellite::SetObliquity(double degrees)
{
	this->obliquity = glm::radians(degrees);
}

double Satellite::GetObliquity()
{
	return glm::degrees(obliquity);
}

void Satellite::SetRotation(double s)
{
	this->rotation = s * 3600;
}

double Satellite::GetRotation()
{
	return rotation / 3600;
}

void Satellite::SetOrbitalInclination(double degrees)
{
	this->orbital_inclination = glm::radians(degrees);
}

double Satellite::GetOrbitalInclination()
{
	return glm::degrees(orbital_inclination);
}

void Satellite::SetSiderealPeriod(double s)
{
	this->sidereal_period = s * 3600;
}

double Satellite::GetSiderealPeriod()
{
	return sidereal_period / 3600;
}

void Satellite::SetMeanRadius(double km)
{
	this->mean_radius = km;
}

double Satellite::GetMeanRadius()
{
	return mean_radius;
}

void Satellite::SetName(string name)
{
	this->name = name;
}

string Satellite::GetName()
{
	return this->name;
}

void Satellite::SetEccentricity(double eccentricity)
{
	this->eccentricity = eccentricity;
}

double Satellite::GetEccentricity()
{
	return eccentricity;
}

void Satellite::SetSemiMajorAxis(double axis)
{
	this->semimajor_axis = axis;
}

double Satellite::GetSemiMajorAxis()
{
	return semimajor_axis;
}

void Satellite::SetParent(Planet*& parent)
{
	this->parent = parent;
}

Planet* Satellite::GetParent()
{
	return parent;
}

glm::vec3 Satellite::GetPosition()
{
	return position;
}

void Satellite::SetSelected(bool isSelected)
{
	this->isSelected = isSelected;
}

bool Satellite::GetSelected()
{
	return isSelected;
}

void Satellite::SetIsDead(bool isDead)
{
	this->isDead = isDead;
}

bool Satellite::GetIsDead()
{
	return isDead;
}

void Satellite::SetColorId(uint32_t colorId)
{
	this->colorId = colorId;

	color.b = (colorId & 0xFF) >> 0;
	color.g = (colorId & 0xFF00) >> 8;
	color.r = (colorId & 0xFF0000) >> 16;
}

vec3b Satellite::GetColor()
{
	return color;
}

void Satellite::SetScale(double scale)
{
	mean_radius *= scale;
	semimajor_axis *= scale;

	BuildSatellite();
}

void Satellite::BuildSatellite()
{
	DeleteLists();

	satelliteList = glGenLists(1);
	glNewList(satelliteList, GL_COMPILE);
	GxSphere(mean_radius, 32);
	glEndList();

	selectList = glGenLists(1);
	glNewList(selectList, GL_COMPILE);
	GxBrackets(mean_radius, 1.2f, 5.0f);
	glEndList();
}

void Satellite::DeleteLists()
{
	if (satelliteList)
		glDeleteLists(satelliteList, 1);

	if (selectList)
		glDeleteLists(selectList, 1);
}

double Satellite::GetExpDist()
{
	return mean_radius * 5;
}