#include "stdafx.h"
#include "Camera.h"

namespace shared{

	Camera::Camera() :
		rotationSensitivity(0.25f),
		maxPitch(80.f),
		yaw(-90.0f),
		pitch(0.0f),
		roll(0.0f)
	{
		update();
	}

	Camera::Camera(glm::vec3 position,
		glm::vec3 up,
		glm::vec3 front,
		GLfloat yaw,
		GLfloat pitch,
		GLfloat roll) :
		CameraBase(position, up, front),
		rotationSensitivity(0.25f),
		maxPitch(80.f),
		yaw(yaw),
		pitch(pitch),
		roll(0.0f)
	{
		update();
	}

	void Camera::setMaxPitch(GLfloat newPitch){
		if (newPitch >= 0.0f && newPitch < 90.f){
			maxPitch = newPitch;
		}
	}

#pragma message(Reminder "Fix roll.")
	void Camera::rotate(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset)
	{
		xoffset *= this->rotationSensitivity;
		yoffset *= this->rotationSensitivity;
		zoffset *= this->rotationSensitivity;
		this->yaw = glm::mod(xoffset + this->yaw, 360.0f);
		this->pitch = yoffset + this->pitch;
		this->roll = glm::mod(zoffset + this->roll, 360.0f);
		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (this->pitch > maxPitch)
			this->pitch = maxPitch;
		if (this->pitch < -maxPitch)
			this->pitch = -maxPitch;

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->update();
	}


	void Camera::update()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));// *cos(glm::radians(this->roll));
		front.y = sin(glm::radians(this->pitch));// *sin(glm::radians(this->roll));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

}
