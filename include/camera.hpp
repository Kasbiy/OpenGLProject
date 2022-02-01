#ifndef CAMERA_HPP
#define CAMERA_HPP

#undef near
#undef far

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

#include "Visualizer.hpp"

BEGIN_VISUALIZER_NAMESPACE

class Camera
{
public:
    Camera(uint32_t windowWidth,
		   uint32_t windowHeight,
		   glm::vec3 position = glm::vec3(0.0f),
		   float horizontalAngle = glm::pi<float>(),
		   float verticalAngle = glm::pi<float>(),
		   float speed = 1.0f,
		   float mouseSpeed = 0.005f,
		   float FOV = 70.0f,
		   float near = 0.01f,
		   float far = 100.f);

	void VerticalMovement(int);
	void HorizontalMovement(int);

	void MoveForward(float dt);
	void MoveBackward(float dt);
	void MoveLeft(float dt);
	void MoveRight(float dt);

	void ComputeProjection(uint32_t, uint32_t);
	
	inline const glm::mat4& GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

    inline const glm::mat4& GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

	inline const glm::mat4& GetViewProjectionMatrix() const
	{
		return m_ViewProjectionMatrix;
	}

    inline const glm::vec3& GetPosition() const
    {
        return m_Position;
    }

	inline const glm::vec3& GetDirection() const
	{
		return m_Direction;
	}

	inline const glm::vec3& GetRight() const
	{
		return m_Right;
	}

	inline const glm::vec3& GetUp() const
	{
		return m_Up;
	}

	inline void SetMovementSpeed(float movementSpeed)
	{
		m_MovementSpeed = movementSpeed;
	}

    inline void SetMouseMovementSpeed(float mouseMovementSpeed)
    {
		m_MouseMovementSpeed = mouseMovementSpeed;
    }

	inline float GetFOV() const
	{
		return m_FOV;
	}

	inline void SetNear(float near)
	{
		m_Near = near;
	}

    inline void SetFar(float far)
    {
        m_Far = far;
    }

	void SetFov(float FOV);

private:
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewProjectionMatrix;
	int m_WindowWidth;
	int m_WindowHeight;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	float m_HorizontalAngle;
	float m_VerticalAngle;
	float m_MovementSpeed;
	float m_MouseMovementSpeed;
	float m_FOV;
	float m_Near;
	float m_Far;
    static constexpr float s_MinFov = 30.f;
    static constexpr float s_MaxFov = 180.f;
};

END_VISUALIZER_NAMESPACE

#endif // !CAMERA_HPP
