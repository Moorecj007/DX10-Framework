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

// Inclusion Guards
#pragma once
#ifndef __DX10_CAMERA_FIRSTPERSON_H__
#define __DX10_CAMERA_FIRSTPERSON_H__

// Local Includes
#include "../DX10_Renderer.h"
#include "../../../Utility/Utilities.h"
#include "../../../Utility/DirectInput.h"

struct TCameraRay
{
	// TO DO CAL - Rename and comment
	v3float Origin;
	v3float Direction;

	/***********************
	* CameraRay: Constructor for the CameraRay struct
	* @author: Jc Fowles
	* @parameter: _origin: The Origin of the Ray
	* @parameter: _direction: The Direction of the Ray
	********************/
	TCameraRay()
	{
		Origin = { 0.0f, 0.0f, 0.0f };
		Direction = { 0.0f, 0.0f, 0.0f };
	}

	/***********************
	* CameraRay: Constructor for the CameraRay struct
	* @author: Jc Fowles
	* @parameter: _origin: The Origin of the Ray
	* @parameter: _direction: The Direction of the Ray
	********************/
	TCameraRay(v3float _origin, v3float _direction)
	{
		Origin = _origin;
		Direction = _direction;
	}

	/***********************
	* RaySphereIntersect: Checks whether the Ray (In Local Space to a Sphere) intersect the Sphere
	* @author: Jc Fowles
	* @parameter: _sphereRadius: Radius of the Sphere
	* @return: bool: True if the Ray Intersect the Sphere
	********************/
	bool RaySphereIntersect(float _sphereRadius)
	{
		// Calculate the a, b, and c Coefficients of the Discriminant
		float a = Direction.Dot(Direction);
		float b = Direction.Dot(Origin) * 2.0f;
		float c = Origin.Dot(Origin) - pow(_sphereRadius, 2);

		// Calculate the Discriminant (b^2 - 4ac)
		float discriminant = pow(b, 2) - (4 * a * c);
		if (discriminant < 0.0f)
		{
			// The Discriminant is Negative the Ray missed the Sphere
			return false;
		}
		else
		{
			// The Discriminant is Positive the Ray intersected the Sphere
			return true;
		}
	}
};



class DX10_Camera_FirstPerson
{
public:

	/***********************
	* CCamera_FirstPerson: Default Constructor for First Person Camera class
	* @author: Callan Moore
	********************/
	DX10_Camera_FirstPerson();

	/***********************
	* ~CCamera_FirstPerson: Default Destructor for First Person Camera class
	* @author: Callan Moore
	********************/
	~DX10_Camera_FirstPerson();

	/***********************
	* Initialise: Initialise the First Person Camera
	* @author: Callan Moore
	* @parameter: _pRenderer: The Renderer of the Application
	* @Parameter: _hInstance: Handle to the current instance
	* @Parameter: _hWnd: Handle to the Window of the application
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, HINSTANCE _hInstance, HWND _hWnd);

	/***********************
	* Process: Process the Camera
	* @author: Callan Moore
	* @parameter: _dt: The current DeltaTick
	* @return: void
	********************/
	void Process(float _dt);

	/***********************
	* MoveForwards: Move the camera along the target axis
	* @author: Callan Moore
	* @parameter: _dir: Direction of movement
	* @return: void
	********************/
	void MoveForwards(float _dir);

	/***********************
	* Strafe: Move the camera along the right vector axis
	* @author: Callan Moore
	* @parameter: _dir: Direction of movement
	* @return: void
	********************/
	void Strafe(float _dir);

	/***********************
	* Fly: Move the camera along the up vector axis
	* @author: Callan Moore
	* @parameter: _dir: Direction of movement
	* @return: void
	********************/
	void Fly(float _dir);

	/***********************
	* RotateYaw: Rotate the Camera around the Yaw axis
	* @author: Callan Moore
	* @parameter: _dir: Direction of rotation
	* @return: void
	********************/
	void RotateYaw(float _dir);

	/***********************
	* RotatePitch: Rotate the Camera around the Pitch axis
	* @author: Callan Moore
	* @parameter: _dir: Direction of rotation
	* @return: void
	********************/
	void RotatePitch(float _dir);

	/***********************
	* GetRay: Calculates a ray from the mouse position
	* @author: Callan Moore
	* @parameter: _mousePos: The Mouse positions in screen space
	* @return: void
	********************/
	TCameraRay GetRay(v2float _mousePos);

private:
	DX10_Renderer* m_pRenderer;
	DirectInput* m_pDirectInput;

	float m_moveStrafe;
	float m_moveForwards;
	float m_moveFly;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_target;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_defaultForward;
	D3DXVECTOR3 m_defaultRight;
	D3DXVECTOR3 m_forward;
	D3DXVECTOR3 m_right;
	D3DXMATRIX m_rotationMatrix;
	D3DXMATRIX m_matView;

	float m_yaw;
	float m_pitch;
	float m_yawChange;
	float m_pitchChange;
	float m_speed;
	float m_rotSpeed;
	float m_maxRotation;
	float m_minRotation;
};
#endif // __DX10_CAMERA_FIRSTPERSON_H__



