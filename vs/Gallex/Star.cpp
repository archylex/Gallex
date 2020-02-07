#include "Star.h"

Star::Star()
{
	preloader = PreLoader::GetInstance();
	
	name = "Star";
	equatorial_radius = 7.0f;
	obliquity = 0.0f;
	rotation = 0.0f;
	position = { 0, 0, -30 };
	isSelected = false;
	
	SetSpectral("G2V");	
}

Star::~Star()
{
	DeleteLists();

	glDeleteTextures(1, &DepthTexture);
	glDeleteTextures(5, StarTextures);
		
	if (GLEW_EXT_framebuffer_object)	
		glDeleteFramebuffersEXT(1, &FBO);	
}

void Star::Resize()
{
	glBindTexture(GL_TEXTURE_2D, DepthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, preloader ->GetScreen().width, preloader->GetScreen().height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	starTextureWidth = preloader->GetScreen().width / 2;
	starTextureHeight = preloader->GetScreen().height / 2;

	for (unsigned int i = 0; i < 4; i++)
	{
		glBindTexture(GL_TEXTURE_2D, StarTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, starTextureWidth, starTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindTexture(GL_TEXTURE_2D, StarTextures[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, starTextureWidth, starTextureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(preloader->GetBlurHorizontalShader());
	glUniform1f(glGetUniformLocation(preloader->GetBlurHorizontalShader(), "TextureWidth"), 1.0f / (float)starTextureWidth);
	glUseProgram(preloader->GetBlurVerticalShader());
	glUniform1f(glGetUniformLocation(preloader->GetBlurVerticalShader(), "TextureHeight"), 1.0f / (float)starTextureHeight);
	glUseProgram(0);
}

void Star::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, GxCamera &camera)
{
	if (isSelected)
	{
		glDisable(GL_TEXTURE_2D);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(viewMatrix));
		glPushMatrix();
		glColor3f(0.7f, 0.85f, 1.0f);
		glTranslatef(position.x, position.y, position.z);
		glRotatef(glm::degrees(eAngles.x), 1, 0, 0);
		glRotatef(glm::degrees(eAngles.y), 0, 1, 0);
		glRotatef(glm::degrees(eAngles.z), 0, 0, 1);
		glCallList(selectList);
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
	}

	glm::mat4 biasMatrix = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
	glm::mat4 VPB = biasMatrix * projectionMatrix * viewMatrix;

	glBindTexture(GL_TEXTURE_2D, DepthTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, preloader->GetScreen().width, preloader->GetScreen().height);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Check visible star (optimization)
	bool isCalcStarFX = false;
	float angle = 0.0f;

	for (unsigned int test = 0; test < TESTS && !isCalcStarFX; test++)
	{
		glm::vec4 starPosProjection = VPB * glm::vec4(position + rotate(camera.X, angle, camera.Z) * (float)equatorial_radius, 1.0f);
		starPosProjection /= starPosProjection.w;

		isCalcStarFX |= (starPosProjection.x >= 0.0f && starPosProjection.x <= 1.0f && starPosProjection.y >= 0.0f && starPosProjection.y <= 1.0f && starPosProjection.z >= 0.0f && starPosProjection.z <= 1.0f);

		angle += ANGLE_STEP;
	}

	// Calculate FX while star is visible only (optimization)
	if (isCalcStarFX)
	{
		glm::vec4 starPosProjection = VPB * glm::vec4(position, 1.0f);
		starPosProjection /= starPosProjection.w;

		glViewport(0, 0, starTextureWidth, starTextureHeight);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[1], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, StarTextures[4], 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(viewMatrix));
		glPushMatrix();
		glTranslated(position.x, position.y, position.z);
		glRotated(rotation_angle, (GLfloat) cos(obliquity), (GLfloat) sin(obliquity), 0.0f);
		glColor3ub(color.r, color.g, color.b);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glUseProgram(preloader->GetPerPixelShader());
		glCallList(sphereList);
		glUseProgram(0);
		glPopMatrix();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[0], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, StarTextures[1]);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, StarTextures[4]);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, DepthTexture);
		glUseProgram(preloader->GetStarDepthTestShader());
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();
		glUseProgram(0);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[2], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		glBindTexture(GL_TEXTURE_2D, StarTextures[0]);
		glUseProgram(preloader->GetBlurHorizontalShader());
		glUniform1i(glGetUniformLocation(preloader->GetBlurHorizontalShader(), "Width"), 1);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[1], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		glBindTexture(GL_TEXTURE_2D, StarTextures[2]);
		glUseProgram(preloader->GetBlurVerticalShader());
		glUniform1i(glGetUniformLocation(preloader->GetBlurVerticalShader(), "Width"), 1);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[3], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		glBindTexture(GL_TEXTURE_2D, StarTextures[0]);
		glUseProgram(preloader->GetBlurHorizontalShader());
		glUniform1i(glGetUniformLocation(preloader->GetBlurHorizontalShader(), "Width"), 10);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[2], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		glBindTexture(GL_TEXTURE_2D, StarTextures[3]);
		glUseProgram(preloader->GetBlurVerticalShader());
		glUniform1i(glGetUniformLocation(preloader->GetBlurVerticalShader(), "Width"), 10);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, StarTextures[3], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, StarTextures[1]);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, StarTextures[2]);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, preloader->GetBokehTexture());
		glUseProgram(preloader->GetStarFXShader());
		glUniform2fv(glGetUniformLocation(preloader->GetStarFXShader(), "StarPosProjection"), 1, glm::value_ptr(starPosProjection));
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		glEnd();
		glUseProgram(0);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glViewport(0, 0, preloader->GetScreen().width, preloader->GetScreen().height);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f)));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (isCalcStarFX)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, StarTextures[3]);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1.0f);
		glEnd();
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

void Star::SimpleDraw()
{
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	glRotated(rotation_angle, (GLfloat)cos(obliquity), (GLfloat)sin(obliquity), 0.0f);
	glColor3ub(colorId.r, colorId.g, colorId.b);
	glCallList(sphereList);
	glPopMatrix();
}

void Star::Update(double time_now)
{
	rotation_angle = (rotation > 0) ? 360.0f / rotation * time_now : 0;
}

void Star::SetEulerAngles(glm::vec3 eAngles)
{
	this->eAngles = eAngles;
}

void Star::SetPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Star::SetPosition(double x, double y, double z)
{
	this->position = glm::vec3(x, y, z);
}

glm::vec3 Star::GetPosition()
{
	return position;
}

void Star::SetObliquity(double degrees)
{
	this->obliquity = glm::radians(degrees);
}

double Star::GetObliquity()
{
	return glm::degrees(obliquity);
}

void Star::SetRotation(double s)
{
	this->rotation = s * 3600;
}

double Star::GetRotation()
{
	return rotation / 3600;
}

void Star::SetEquatorialRadius(double km)
{
	this->equatorial_radius = km;
}

double Star::GetEquatorialRadius()
{
	return equatorial_radius;
}

void Star::SetName(string name)
{
	this->name = name;
}

string Star::GetName()
{
	return name;
}

void Star::SetSpectral(string spectral)
{
	this->spectral = spectral;
	this->color = GetSpectralColor(spectral.c_str());
}

string Star::GetSpectral()
{
	return spectral;
}

void Star::SetSelected(bool isSelected)
{
	this->isSelected = isSelected;
}

bool Star::GetSelected()
{
	return isSelected;
}

void Star::SetIsDead(bool isDead)
{
	this->isDead = isDead;
}

bool Star::GetIsDead()
{
	return isDead;
}

void Star::SetColorId(uint32_t colorId)
{
	vec3b colID;
	ColorLineToBytes(colorId, colID);
	this->colorId = colID;
}

vec3b Star::GetColor()
{
	return colorId;
}

void Star::BuildStar()
{
	DeleteLists();

	selectList = glGenLists(1);
	glNewList(selectList, GL_COMPILE);
	GxBrackets(equatorial_radius, 1.2f, 5.0f);
	glEndList();

	sphereList = glGenLists(1);
	glNewList(sphereList, GL_COMPILE);
	GxSphere(equatorial_radius, 12);
	glEndList();

	glUseProgram(preloader->GetStarDepthTestShader());
	glUniform1i(glGetUniformLocation(preloader->GetStarDepthTestShader(), "StarTexture"), 0);
	glUniform1i(glGetUniformLocation(preloader->GetStarDepthTestShader(), "StarDepthTexture"), 1);
	glUniform1i(glGetUniformLocation(preloader->GetStarDepthTestShader(), "DepthTexture"), 2);
	glUseProgram(0);

	glUseProgram(preloader->GetStarFXShader());
	glUniform1i(glGetUniformLocation(preloader->GetStarFXShader(), "LowBlurStarTexture"), 0);
	glUniform1i(glGetUniformLocation(preloader->GetStarFXShader(), "HighBlurStarTexture"), 1);
	glUniform1i(glGetUniformLocation(preloader->GetStarFXShader(), "BokehTexture"), 2);
	glUniform1f(glGetUniformLocation(preloader->GetStarFXShader(), "Dispersal"), 0.1875f);
	glUniform1f(glGetUniformLocation(preloader->GetStarFXShader(), "HaloWidth"), 0.435f);
	glUniform1f(glGetUniformLocation(preloader->GetStarFXShader(), "Intensity"), 2.7f);
	glUniform3f(glGetUniformLocation(preloader->GetStarFXShader(), "Distortion"), 0.94f, 0.97f, 1.00f);
	glUseProgram(0);

	glGenTextures(1, &DepthTexture);
	glGenTextures(5, StarTextures);

	glGenFramebuffersEXT(1, &FBO);
}

void Star::DeleteLists()
{
	if (selectList)
		glDeleteLists(selectList, 1);

	if (sphereList)
		glDeleteLists(sphereList, 1);
}

void Star::SetScale(double scale)
{
	equatorial_radius *= scale;
	BuildStar();
	Resize();
}

double Star::GetExpDist()
{
	return equatorial_radius * 5;
}