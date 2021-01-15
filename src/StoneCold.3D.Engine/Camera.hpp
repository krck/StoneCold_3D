
#ifndef STONECOLD_CAMERA_H
#define STONECOLD_CAMERA_H

#include "math.h"
#include "Enums.hpp"

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	class Camera {
	public:
		// OpenGL Coordinate System
		// ------------------------------
		//              +Y
		//               | -Z
		//               | /
		//        -X____ |/_____ +X
		//               /
		//              /|
		//            +Z |
		//              -Y
		// ------------------------------
		// +Y = UP       |  -Y = DOWN
		// +Z = BACKWARD |  -Z = FORWARD
		// +X = RIGHT    |  -X = LEFT    
		// ------------------------------
		const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3 WORLD_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3 WORLD_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
		// ---------------------------------------------------------------------------------------------------------
		// This can be confusing because in any usual 3D Model "space" (e.g. Blender) the Y and Z Axis are switched,
		// meaning that Z = UP/DOWN Axis and Y = FORWARD/BACKWARD Axis. Also +/- on that Y Axis are switched as well
		// ---------------------------------------------------------------------------------------------------------

	public:
		Camera(const glm::vec3& pos);
		Camera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& front, const glm::vec3& right);

		//
		// Process input received from the Keyboard and Mouse movements
		//
		void ChangePosition(MoveDirection dir, float deltaTime);
		void ChangeLineOfSight(const int xpos, const int ypos, bool constrain);

		inline void SetPosition(const glm::vec3& position) { _position = position; }
		inline void SetPosition(glm::vec3&& position) noexcept { _position = std::move(position); }
		inline void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
		inline void SetMouseSensitivity(float mouseSensitivity) { _mouseSens = mouseSensitivity; }

		inline glm::vec3 GetPosition() const { return _position; }
		inline float GetMoveSpeed() const { return _moveSpeed; }
		inline float GetMouseSensitivity() const { return _mouseSens; }

		inline glm::vec3 GetLeft() const { return glm::normalize(glm::cross(_up, _front)); }
		inline glm::vec3 GetRight() const { return glm::normalize(glm::cross(_front, _up)); }
		inline glm::mat4 GetViewMatrix() const { return glm::lookAt(_position, (_position + _front), _up); }

	private:
		inline float DegreeToRadians(float degrees) const { return (degrees * 0.0174533f); }

		void UpdateCameraVecs();

	private:
		glm::vec3 _position;
		glm::vec3 _up, _front, _right;
		float _pitch, _yaw;
		float _lastX, _lastY;
		float _mouseSens, _moveSpeed;
	};

}

#endif
