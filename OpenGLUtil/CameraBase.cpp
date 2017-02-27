#include "stdafx.h"
#include "CameraBase.h"

namespace shared{

	CameraBase::CameraBase() :
		position(0.0f, 0.0f, 0.0f),
		worldUp(0.0f, 1.0f, 0.0f),
		front(0.0f, 0.0f, -1.0f),
		minZoom(1.0f),
		maxZoom(30.0f),
		zoomLevel(1.0f),
		movementSpeed(3.0f)
	{
		update();
	}

	CameraBase::CameraBase(glm::vec3 position, glm::vec3 up, glm::vec3 front) :
		position(position),
		worldUp(up),
		front(front),
		minZoom(1.0f),
		maxZoom(30.0f),
		zoomLevel(1.0f),
		movementSpeed(3.0f)
	{
		update();
	}

	void CameraBase::setMovementSpeed(GLfloat speed)
	{
		if (speed >= 0.0f){
			movementSpeed = speed;
		}
	}

	void CameraBase::setMinZoom(GLfloat newZoom)
	{
		if (newZoom >= 0.0f && newZoom < maxZoom){
			minZoom = newZoom;
		}
	}


	void CameraBase::setMaxZoom(GLfloat newZoom)
	{
		if (newZoom > minZoom){
			maxZoom = newZoom;
		}
	}

	glm::mat4 CameraBase::view() const
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

#pragma message(Reminder "Change the implementation to allow the camera to move in any direction vector")
	void CameraBase::move(CameraMovement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;
		if (direction == FORWARD)
			this->position += this->front * velocity;
		if (direction == BACKWARD)
			this->position -= this->front * velocity;
		if (direction == LEFT)
			this->position -= this->right * velocity;
		if (direction == RIGHT)
			this->position += this->right * velocity;
	}

	void CameraBase::rotate(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset)
	{

	}

	void CameraBase::zoom(GLfloat offset)
	{
		if (this->zoomLevel >= minZoom && this->zoomLevel <= maxZoom)
			this->zoomLevel -= offset;
		if (this->zoomLevel <= minZoom)
			this->zoomLevel = minZoom;
		if (this->zoomLevel >= maxZoom)
			this->zoomLevel = maxZoom;
	}

	void CameraBase::update()
	{
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
}