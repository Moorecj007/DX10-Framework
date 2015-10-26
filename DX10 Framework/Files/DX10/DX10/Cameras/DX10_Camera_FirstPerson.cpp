/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Camera_FirstPerson.h
* Description : First Person Camera for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Local Includes
#include "DX10_Camera_FirstPerson.h"

DX10_Camera_FirstPerson::DX10_Camera_FirstPerson()
{
}

DX10_Camera_FirstPerson::~DX10_Camera_FirstPerson()
{
	ReleasePtr(m_pDirectInput)
}

bool DX10_Camera_FirstPerson::Initialise(DX10_Renderer* _pRenderer, HINSTANCE _hInstance, HWND _hWnd)
{
	m_pRenderer = _pRenderer;
	m_pDirectInput = new DirectInput();
	VALIDATE(m_pDirectInput->Initialise(_hInstance, _hWnd));

	m_position = D3DXVECTOR3(0.0f, 30.0f, -80.0f);
	m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_forward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_defaultForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_defaultRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_moveStrafe = 0.0f;
	m_moveForwards = 0.0f;
	m_moveFly = 0.0f;
	m_yaw = 0.0f;
	m_pitch = DegreesToRadians(30.0f);
	m_yawChange = 0.0f;
	m_pitchChange = 0.0f;

	m_speed = 20.0f;
	m_rotSpeed = DegreesToRadians(90.0f);

	m_maxRotation = DegreesToRadians(89.0f);
	m_minRotation = DegreesToRadians(-89.0f);

	Process(0.016f);

	return true;
}

void DX10_Camera_FirstPerson::Process(float _dt)
{
	// Get mouse input
	//m_pDirectInput->DetectMouseInput(&m_yawChange, &m_pitchChange);
	m_yaw += m_yawChange * _dt;
	m_pitch += m_pitchChange * _dt;

	// Prevent Gimbal Lock
	if (m_pitch > m_maxRotation)
	{
		m_pitch = m_maxRotation;
	}
	if (m_pitch < m_minRotation)
	{
		m_pitch = m_minRotation;
	}

	// Create a full rotation matrix
	D3DXMatrixRotationYawPitchRoll(&m_rotationMatrix, m_yaw, m_pitch, 0);
	D3DXVec3TransformCoord(&m_target, &m_defaultForward, &m_rotationMatrix);
	D3DXVec3Normalize(&m_target, &m_target);

	// Calculate a Yaw rotation matrix
	D3DXMATRIX RotateYTempMatrix;
	D3DXMatrixRotationY(&RotateYTempMatrix, m_yaw);

	// Update the Local camera Axis around the Y axis
	D3DXVec3TransformNormal(&m_right, &m_defaultRight, &RotateYTempMatrix);
	D3DXVec3TransformNormal(&m_up, &m_up, &RotateYTempMatrix);
	D3DXVec3TransformNormal(&m_forward, &m_defaultForward, &RotateYTempMatrix);

	// Adjust the position
	m_position += m_moveStrafe * m_right * _dt;
	m_position += m_moveForwards * m_forward * _dt;
	m_position += m_moveFly * m_up * _dt;

	// reset the the movement values
	m_moveStrafe = 0.0f;
	m_moveForwards = 0.0f;
	m_moveFly = 0.0f;
	m_yawChange = 0.0f;
	m_pitchChange = 0.0f;

	// Pick a target in front of the camera
	m_target = m_position + m_target;

	// Create the View matrix
	D3DXMatrixLookAtLH(&m_matView, &m_position, &m_target, &m_up);

	m_pRenderer->SetViewMatrix(m_matView);
	m_pRenderer->SetEyePosition(m_position);
}

void DX10_Camera_FirstPerson::MoveForwards(float _dir)
{
	if (_dir > 0)
	{
		// Movement Forwards along look at axis		(positive)
		m_moveForwards += m_speed;
	}
	else
	{
		// Movement Backwards along look at axis	(negative)
		m_moveForwards -= m_speed;
	}
}

void DX10_Camera_FirstPerson::Strafe(float _dir)
{
	if (_dir > 0)
	{
		// Movement Right along right axis	(positive)
		m_moveStrafe += m_speed;
	}
	else
	{
		// Movement Left along right axis	(negative)
		m_moveStrafe -= m_speed;
	}
}

void DX10_Camera_FirstPerson::Fly(float _dir)
{
	if (_dir > 0)
	{
		// Movement Up along Up axis	(positive)
		m_moveFly += m_speed;
	}
	else
	{
		// Movement Down along Up axis	(negative)
		m_moveFly -= m_speed;
	}
}

void DX10_Camera_FirstPerson::RotateYaw(float _dir)
{
	if (_dir > 0)
	{
		// Rotate right on Yaw axis
		m_yawChange += m_rotSpeed;
	}
	else
	{
		// Rotate left on Yaw axis
		m_yawChange -= m_rotSpeed;
	}
}

void DX10_Camera_FirstPerson::RotatePitch(float _dir)
{
	if (_dir > 0)
	{
		// Rotate Up on Pitch axis
		m_pitchChange += m_rotSpeed;
	}
	else
	{
		// Rotate Down on Pitch axis
		m_pitchChange -= m_rotSpeed;
	}
}

void DX10_Camera_FirstPerson::GetRay(POINT _mousePos, v3float* _pOriginPos, v3float* _pDirection)
{
	// Move the Mouse Coordinates into the -1 to +1 range.
	float pointX = ((2.0f * (float)_mousePos.x) / (float)m_pRenderer->GetWidth()) - 1.0f;
	float pointY = (((2.0f * (float)_mousePos.y) / (float)m_pRenderer->GetHeight()) - 1.0f) * -1.0f;

	// Adjust the points using the Projection Matrix to Account for the Aspect Ratio of the Viewport
	D3DXMATRIX projectionMatrix;
	projectionMatrix = *(m_pRenderer->GetProjMatrix());
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	D3DXMATRIX invViewMatrix;
	D3DXMatrixInverse(&invViewMatrix, NULL, &m_matView);

	// Calculate the Direction of the Ray in View Space.
	_pDirection->x = (pointX * invViewMatrix._11) + (pointY * invViewMatrix._21) + invViewMatrix._31;
	_pDirection->y = (pointX * invViewMatrix._12) + (pointY * invViewMatrix._22) + invViewMatrix._32;
	_pDirection->z = (pointX * invViewMatrix._13) + (pointY * invViewMatrix._23) + invViewMatrix._33;

	// Set the Origin Position of the Ray which is the Position of the Camera.
	*_pOriginPos = { m_position.x, m_position.y, m_position.z };
}