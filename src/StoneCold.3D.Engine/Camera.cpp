#include "Camera.hpp"

using namespace StoneCold::Base;
using namespace StoneCold::Engine;

Camera::Camera(const glm::vec3& pos) 
	: _position(pos) {
	_up = WORLD_UP;
	_front = WORLD_FRONT;
	_right = WORLD_RIGHT;
	_pitch = 0.0f;
	_yaw = -90.0f;
	_lastX = 0.0f;
	_lastY = 0.0f;
	_mouseSens = 0.5f;
	_moveSpeed = 3.0f;
}


Camera::Camera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& front, const glm::vec3& right) 
	: _position(pos), _up(up), _front(front), _right(right) {
	_pitch = 0.0f;
	_yaw = -90.0f;
	_lastX = 0.0f;
	_lastY = 0.0f;
	_mouseSens = 0.5f;
	_moveSpeed = 3.0f;
}


//
// Process input received from the Keyboard
//
void Camera::ChangePosition(MoveDirection dir, float deltaTime) {
	const float velocity = _moveSpeed * deltaTime;
	switch (dir) {
	case MoveDirection::UP:         _position += (_up * velocity);      break;
	case MoveDirection::DOWN:       _position -= (_up * velocity);      break;
	case MoveDirection::FORWARD:    _position += (_front * velocity);   break;
	case MoveDirection::BACKWARD:	_position -= (_front * velocity);   break;
	case MoveDirection::RIGHT:      _position += (_right * velocity);   break;
	case MoveDirection::LEFT:       _position -= (_right * velocity);   break;
	}
}


//
// Process input received from the Mouse movements
//
void Camera::ChangeLineOfSight(const int xpos, const int ypos, bool constrain) {
	const float xoffset = (xpos - _lastX) * _mouseSens;
	const float yoffset = (_lastY - ypos) * _mouseSens;
	_lastX = (float)xpos;
	_lastY = (float)ypos;
	_yaw += xoffset;
	_pitch += yoffset;
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrain) {
		if (_pitch > 89.0f) { _pitch = 89.0f; }
		if (_pitch < -89.0f) { _pitch = -89.0f; }
	}
	// Update Front, Right and Up Vectors using the updated Eular angles
	UpdateCameraVecs();
}


void Camera::UpdateCameraVecs() {
	glm::vec3 tmp;
	tmp.x = cos(DegreeToRadians(_yaw)) * cos(DegreeToRadians(_pitch));
	tmp.y = sin(DegreeToRadians(_pitch));
	tmp.z = sin(DegreeToRadians(_yaw)) * cos(DegreeToRadians(_pitch));
	_front = glm::normalize(tmp);
	_right = glm::normalize(glm::cross(_front, WORLD_UP));
	_up = glm::normalize(glm::cross(_right, _front));
}
