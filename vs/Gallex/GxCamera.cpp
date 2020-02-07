#include "GxCamera.h"

GxCamera::GxCamera()
{
	viewMatrix = NULL;
	viewMatrixInverse = NULL;

	X = glm::vec3(1, 0, 0);
	Y = glm::vec3(0, 1, 0);
	Z = glm::vec3(0, 0, 1);

	position = glm::vec3(0.0f, 0.0f, 7.0f);
	reference = glm::vec3(0.0f, 0.0f, 0.0f);

	eulerAngles = CalcEulerAngles();
}

void GxCamera::LookAt(const glm::vec3& position, const glm::vec3& reference, bool isRotation)
{
	this->position = position;
	this->reference = reference;

	Z = glm::normalize(position - reference);
	X = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), Z));
	Y = glm::cross(Z, X);

	if (!isRotation)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void GxCamera::Move(const glm::vec3& movement)
{
	position += movement;
	reference += movement;

	CalculateViewMatrix();
}

glm::vec3 GxCamera::OnEdges(const unsigned int edge, float refreshMills)
{
	float Speed = 5.0f;
	float Distance = Speed * refreshMills;

	glm::vec3 Up(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = X;
	glm::vec3 Forward = cross(Up, Right);

	Up *= Distance;
	Right *= Distance;
	Forward *= Distance;

	glm::vec3 Movement(0, 0, 0);
		
	if (edge == 3) Movement -= Right;
	if (edge == 4) Movement += Right;

	if (edge == 1)
	{
		Movement -= Forward * sin(eulerAngles.x);
		Movement += Up * cos(eulerAngles.x);
	}

	if (edge == 2)
	{
		Movement += Forward * sin(eulerAngles.x);
		Movement -= Up * cos(eulerAngles.x);
	}

	return Movement;
}

void GxCamera::OnMouseMove(int dx, int dy)
{
	position -= reference;

	if (dx != 0)
	{
		float deltaX = (float)dx * sensitivity;

		X = glm::rotate(X, glm::radians(deltaX), glm::vec3(0.0f, 1.0f, 0.0f));
		Y = glm::rotate(Y, glm::radians(deltaX), glm::vec3(0.0f, 1.0f, 0.0f));
		Z = glm::rotate(Z, glm::radians(deltaX), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float deltaY = (float)dy * sensitivity;

		Y = rotate(Y, glm::radians(deltaY), X);
		Z = rotate(Z, glm::radians(deltaY), X);

		if (Y.y < 0.0f)
		{
			Z = glm::vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	position = reference + Z * length(position);

	CalculateViewMatrix();

	eulerAngles = CalcEulerAngles();
}

void GxCamera::OnMouseWheel(double zDelta)
{
	position -= reference;

	if (zDelta < 0 && length(position) < 1000.0f)
		position += position * sensitivity * 0.1f;

	if (zDelta > 0 && length(position) > 0.1f)
		position -= position * sensitivity * 0.1f;

	position += reference;

	CalculateViewMatrix();
}

void GxCamera::SetSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}

void GxCamera::SetViewMatrixPointer(float* viewMatrix, float* viewMatrixInverse)
{
	this->viewMatrix = (glm::mat4*)viewMatrix;
	this->viewMatrixInverse = (glm::mat4*)viewMatrixInverse;

	CalculateViewMatrix();
}

void GxCamera::CalculateViewMatrix()
{
	if (viewMatrix != NULL)
	{
		*viewMatrix = glm::mat4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, position), -dot(Y, position), -dot(Z, position), 1.0f);

		if (viewMatrixInverse != NULL)
		{
			*viewMatrixInverse = inverse(*viewMatrix);
		}
	}
}

glm::mat4* GxCamera::GetViewMatrix()
{
	return viewMatrix;
}

glm::mat4* GxCamera::GetInverseViewMatrix()
{
	return viewMatrixInverse;
}

glm::vec3 GxCamera::CalcEulerAngles()
{
	glm::vec3 res;
	res.x = atan2(-Z.y, Z.z);
	res.y = asin(Z.x);
	res.z = atan2(-Y.x, X.x);
	return res;
}

glm::vec3 GxCamera::GetEulerAngles()
{
	return eulerAngles;
}