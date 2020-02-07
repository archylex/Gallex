#include "Planet.h"

Planet::Planet() 
{
	texture.LoadTexture("Earth.jpg");
		
	name = "Planet";
	mean_radius = 1;
	obliquity = 0;
	rotation = 0;
	orbital_inclination = 0;
	sidereal_period = 0;
	semimajor_axis = 50;
	eccentricity = 0.3;	
	isSelected = false; 
}

Planet::~Planet()
{
	DeleteLists();
	DeleteTextures();
}

void Planet::SetTexture(const char* filename)
{
	texture.LoadTexture(filename);
}

string Planet::GetTextureName()
{
	return texture.GetTextureName();
}

string Planet::GetRingTextureName()
{
	return ring_texture.GetTextureName();
}

void Planet::Draw()
{	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotated(rotation_angle, (double) sin(obliquity), (double) cos(obliquity), 0);		
	glColor3f(1.0f, 1.0f, 1.0f);	
	glBindTexture(GL_TEXTURE_2D, texture);
	glCallList(planetList);
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

	if (ring_texture.GetTextureName().size() > 0)
	{
		double radius = mean_radius * 3;
		
		glEnable(GL_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		
		glPushMatrix();
		glTranslated(position.x, position.y, position.z);
		glRotated(-90, 1, 0, 0);
		glColor3d(1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, ring_texture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1);
		glVertex2d(-radius, -radius);
		glTexCoord2d(1, 1);
		glVertex2d(radius, -radius);
		glTexCoord2d(1, 0);
		glVertex2d(radius, radius);
		glTexCoord2d(0, 0);
		glVertex2d(-radius, radius);
		glEnd();		
		glRotated(180, 1, 0, 0);				
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1);
		glVertex2d(-radius, -radius);
		glTexCoord2d(1, 1);
		glVertex2d(radius, -radius);
		glTexCoord2d(1, 0);
		glVertex2d(radius, radius);
		glTexCoord2d(0, 0);
		glVertex2d(-radius, radius);
		glEnd();
		glPopMatrix();

		glDisable(GL_BLEND);
		glDisable(GL_ALPHA);
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void Planet::SimpleDraw()
{
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(rotation_angle, (double)sin(obliquity), (double)cos(obliquity), 0);	
	glColor3ub(color.r, color.g, color.b);
	glCallList(planetList);
	glPopMatrix();
}

void Planet::SetEulerAngles(glm::vec3 eAngles)
{
	this->eAngles = eAngles;
}

void Planet::Update(double time_now)
{
	rotation_angle = (rotation > 0) ? 360.0f / rotation * time_now : 0;

	double true_anomaly = GetTrueAnomaly(eccentricity, sidereal_period, time_now);
	vec2d point = Get2DCoord(semimajor_axis, eccentricity, true_anomaly);

	position.x = parent->GetPosition().x + point.x * cos(orbital_inclination);
	position.y = parent->GetPosition().y + point.x * sin(orbital_inclination);
	position.z = parent->GetPosition().z + point.y;
}

void Planet::SetObliquity(double degrees)
{
	this->obliquity = glm::radians(degrees);
}

double Planet::GetObliquity()
{
	return glm::degrees(obliquity);
}

void Planet::SetRotation(double s)
{
	this->rotation = s * 3600;
}

double Planet::GetRotation()
{
	return rotation / 3600;
}

void Planet::SetOrbitalInclination(double degrees)
{
	this->orbital_inclination = glm::radians(degrees);
}

double Planet::GetOrbitalInclination()
{
	return glm::degrees(orbital_inclination);
}

void Planet::SetSiderealPeriod(double s)
{
	this->sidereal_period = s * 3600;
}

double Planet::GetSiderealPeriod()
{
	return sidereal_period / 3600;
}

void Planet::SetMeanRadius(double km)
{
	this->mean_radius = km;
}

double Planet::GetMeanRadius()
{
	return mean_radius;
}

void Planet::SetName(string name)
{
	this->name = name;
}

string Planet::GetName()
{
	return this->name;
}

void Planet::SetEccentricity(double eccentricity)
{
	this->eccentricity = eccentricity;
}

double Planet::GetEccentricity()
{
	return eccentricity;
}

void Planet::SetSemiMajorAxis(double axis)
{
	this->semimajor_axis = axis;
}

double Planet::GetSemiMajorAxis()
{
	return semimajor_axis;
}

void Planet::SetParent(Star* parent)
{
	this->parent = parent;
}

Star* Planet::GetParent()
{
	return parent;
}

glm::vec3 Planet::GetPosition()
{
	return position;
}

void Planet::SetSelected(bool isSelected)
{
	this->isSelected = isSelected;
}

bool Planet::GetSelected()
{
	return isSelected;
}

void Planet::SetIsDead(bool isDead)
{
	this->isDead = isDead;
}

bool Planet::GetIsDead()
{
	return isDead;
}

void Planet::SetColorId(uint32_t colorId)
{
	this->colorId = colorId;
	ColorLineToBytes(colorId, color);
}

vec3b Planet::GetColor()
{
	return color;
}

void Planet::SetRingTexture(const char* filename)
{
	ring_texture.LoadTexture(filename);
}

void Planet::BuildPlanet()
{
	DeleteLists();

	planetList = glGenLists(1);
	glNewList(planetList, GL_COMPILE);	
	GxSphere(mean_radius, 32);
	glEndList();

	selectList = glGenLists(1);
	glNewList(selectList, GL_COMPILE);
	GxBrackets(mean_radius, 1.2f, 5.0f);
	glEndList();	
}

void Planet::DeleteLists()
{
	if (planetList)
		glDeleteLists(planetList, 1);

	if (selectList)
		glDeleteLists(selectList, 1);
}

void Planet::DeleteTextures()
{
	texture.Remove();
	ring_texture.Remove();
}

void Planet::SetScale(double scale)
{
	mean_radius *= scale;
	semimajor_axis *= scale;

	BuildPlanet();
}

double Planet::GetExpDist()
{
	return mean_radius * 5;
}