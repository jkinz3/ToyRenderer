#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_FOV = 70.f;
	MouseSensitivity = 4.f;
	MovementSpeed = .1f;
}

Camera::~Camera()
{

}

bool Camera::Initialize(int width, int height, Vector3 Pos, float Yaw, float Pitch)
{
	m_ViewWidth = width;
	m_ViewHeight = height;
	m_Position = Pos;

	m_Yaw = Yaw;
	m_Pitch = Pitch;

	UpdateView();
	UpdateProjection();

	return true;
}

void Camera::Tick(float DeltaTime)
{
	constexpr float limit = XM_PIDIV2 - .01f;
	m_Pitch = std::max(-limit, m_Pitch);
	m_Pitch = std::min(+limit, m_Pitch);

	if(m_Yaw > XM_PI)
	{
		m_Yaw -= XM_2PI;
	}
	else if (m_Yaw < -XM_PI)
	{
		m_Yaw += XM_2PI;
	}



	UpdateView();
}

void Camera::SetViewSize(int width, int height)
{
	UpdateView();
	UpdateProjection();
}

Matrix Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

Matrix Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void Camera::MoveCamera(Vector3 DeltaLoc, Vector3 DeltaRot)
{
	DeltaLoc *= MovementSpeed;

	m_Position += DeltaLoc;
	
	m_Pitch -= DeltaRot.y;
	m_Yaw -= DeltaRot.x;
	UpdateView();
}

Quaternion Camera::GetOrientation() const
{
	return Quaternion::CreateFromYawPitchRoll(m_Yaw, m_Pitch, 0.f);
}

float Camera::GetMovementSpeed() const
{
	return MovementSpeed;
}

float Camera::GetMouseSensitivity() const
{
	return MouseSensitivity;
}

void Camera::ResetCamera()
{
	m_Position = Vector3(0.f, -1.5f, 0.f);
	m_Pitch = 0.f;
	m_Yaw = 0.f;
}

void Camera::UpdateView()
{
	float y = sinf(m_Pitch);
	float r = cosf(m_Pitch);
	float z = r * cosf(m_Yaw);
	float x = r * sinf(m_Yaw);

	XMVECTOR lookAt = m_Position + Vector3(x, y, z);

	m_ViewMatrix = Matrix::CreateLookAt(m_Position, lookAt, Vector3::Up);
}

void Camera::UpdateProjection()
{
	float AspectRatio = float(m_ViewWidth) / float(m_ViewHeight);
	
	m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(m_FOV), AspectRatio, .01f, 100.f);
}
