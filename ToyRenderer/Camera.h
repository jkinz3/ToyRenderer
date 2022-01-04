#pragma once

using namespace DirectX;
using namespace DirectX::SimpleMath;
class Camera
{
public:
	Camera();
	~Camera();

	bool Initialize(int width, int height, Vector3 Pos, float Yaw, float Pitch);

	void Tick(float DeltaTime);

	void SetViewSize(int width, int height);

	Matrix GetViewMatrix() const;
	Matrix GetProjectionMatrix() const;

	
	

private:


	void UpdateView();

	void UpdateProjection();

	float m_Yaw;
	float m_Pitch;
	Vector3 m_Position;

	float m_ViewWidth;
	float m_ViewHeight;

	float m_FOV;

	Matrix m_ViewMatrix;
	Matrix m_ProjectionMatrix;

};

