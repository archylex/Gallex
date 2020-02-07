#ifndef __GX_CAMERA_H__
#define __GX_CAMERA_H__

#pragma once

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

class GxCamera
{
public:
	GxCamera();
	void LookAt(const glm::vec3& position, const glm::vec3& reference, bool isRotation);
	void Move(const glm::vec3& movement);
	glm::vec3 OnEdges(const unsigned int edge, float refreshMills);
	void OnMouseMove(int dx, int dy);
	void OnMouseWheel(double zDelta);
	void SetViewMatrixPointer(float* viewMatrix, float* viewMatrixInverse = NULL);
	void SetSensitivity(float sensitivity);
	glm::mat4* GetViewMatrix();
	glm::mat4* GetInverseViewMatrix();	
	glm::vec3 GetEulerAngles();	

	glm::vec3 X;
	glm::vec3 Y;
	glm::vec3 Z;

private:
	void CalculateViewMatrix();
	glm::vec3 CalcEulerAngles();

	glm::mat4* viewMatrix;
	glm::mat4* viewMatrixInverse;
	float sensitivity;	
	glm::vec3 position;
	glm::vec3 reference;
	glm::vec3 eulerAngles;
};

#endif // !__GX_CAMERA_H__