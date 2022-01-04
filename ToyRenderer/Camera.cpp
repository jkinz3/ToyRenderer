#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_FOV = 70.f;
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

void Camera::UpdateView()
{
	m_ViewMatrix = Matrix::CreateLookAt(m_Position, Vector3::Zero, Vector3::UnitY);
}

void Camera::UpdateProjection()
{
	float AspectRatio = float(m_ViewWidth) / float(m_ViewHeight);
	
	m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(m_FOV), AspectRatio, .01f, 100.f);
}
