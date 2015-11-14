/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_Generic.h
* Description : 3D Generic Object
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_OBJ_GENERIC_H__
#define __DX10_OBJ_GENERIC_H__

// Local Includes
#include "../Meshes/DX10_Mesh.h"

class DX10_Obj_Generic
{
public:

	/***********************
	* ~CGenericObject: Default Destructor for Generic Object class
	* @author: Callan Moore
	********************/
	virtual ~DX10_Obj_Generic() {};

	/***********************
	* BaseInitialise: Initialise the Base member variables
	* @author: Callan Moore
	* @return: void
	********************/
	void BaseInitialise()
	{
		// Set initial position to origin point
		m_pos = { 0, 0, 0 };

		// Initialise Rotations to zero
		m_rotation_Pitch = 0;
		m_rotation_Yaw = 0;
		m_rotation_Roll = 0;
		m_rotPerSecond_Pitch = 0;
		m_rotPerSecond_Yaw = 0;
		m_rotPerSecond_Roll = 0;

		// Set all pointers to NULL
		m_pRenderer = 0;
		m_pTextures = 0;

		// Animation variables
		m_animating = false;
		m_texIndex = 0;
		m_animationSpeed = 0;
		m_animationTimer = 0;

		// Scrolling Variables
		m_scrolling = false;
		m_scrollSpeed = 0;
		m_scrollTimer = 0;
		m_scrollDir = { 0, 0 };
		m_scrollTranslation = { 0, 0 };
	}
	
	/***********************
	* BaseProcess: Process the Generic properties
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	void BaseProcess(float _dt)
	{
		// Calculate the new Rotation
		m_rotation_Pitch += m_rotPerSecond_Pitch * _dt;
		m_rotation_Yaw += m_rotPerSecond_Yaw * _dt;
		m_rotation_Roll += m_rotPerSecond_Roll * _dt;

		// Calculate the new world matrix
		CalcWorldMatrix();

		if (m_animating == true)
		{
			// Update the Animation timer and calculate which texture to be showing
			m_animationTimer += _dt;
			m_texIndex = (int)(m_animationTimer * (float)m_pTextures->size() / m_animationSpeed);

			// Reset the animation timer once a full cycle is complete
			if (m_animationTimer >= m_animationSpeed)
			{
				m_animationTimer -= m_animationSpeed;
				m_texIndex = 0;
			}
		}

		if (m_scrolling == true)
		{
			// Update the Scroll timer and calculate the translation coordinates of the texture
			m_scrollTimer += _dt;
			m_scrollTranslation = m_scrollDir * (m_scrollTimer / m_scrollSpeed);

			// Reset the Scroll timer once a full cycle is complete
			if (m_scrollTimer >= m_scrollSpeed)
			{
				m_scrollTimer -= m_scrollSpeed;
			}
		}
	}
	
	/***********************
	* CalcWorldMatrix: Calculate the World Matrix 
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void CalcWorldMatrix()
	{
		// Matrices to make up the World Matrix
		D3DXMATRIX matRotatePitch;
		D3DXMATRIX matRotateYaw;
		D3DXMATRIX matRotateRoll;
		D3DXMATRIX matTranslation;

		// Initialise each matrix to Identity
		D3DXMatrixIdentity(&matRotatePitch);
		D3DXMatrixIdentity(&matRotateYaw);
		D3DXMatrixIdentity(&matRotateRoll);
		D3DXMatrixIdentity(&matTranslation);

		// Reset the Objects World Matrix for new Calculation
		D3DXMatrixIdentity(&m_matWorld);

		// Calculate a Rotation Matrices around the Pitch, Yaw and Roll axes
		D3DXMatrixRotationX(&matRotatePitch, m_rotation_Pitch);
		D3DXMatrixRotationY(&matRotateYaw, m_rotation_Yaw);
		D3DXMatrixRotationZ(&matRotateRoll, m_rotation_Roll);

		// Calculate the Full Rotation Matrix and store in the World Matrix
		m_matWorld = matRotatePitch * matRotateYaw * matRotateRoll;

		// Calculate a translation Matrix based on the Objects current position in world space
		D3DXMatrixTranslation(&matTranslation, m_pos.x, m_pos.y, m_pos.z);

		// Calculate the Objects World Matrix
		m_matWorld *= matTranslation;
	}

	/***********************
	* GetPosition: Retrieve the Position of the Object
	* @author: Callan Moore
	* @return: v3float: 3D position of the Objects center
	********************/
	v3float GetPosition() { return m_pos; };
	
	/***********************
	* GetRotationPitch: Retrieve the Objects current rotation around the X axis
	* @author: Callan Moore
	* @return: float: Rotation in radians
	********************/
	float GetRotationPitch() { return m_rotation_Pitch; };
	
	/***********************
	* GetRotationYaw: Retrieve the Objects current rotation around the Y axis
	* @author: Callan Moore
	* @return: float: Rotation in radians
	********************/
	float GetRotationYaw() { return m_rotation_Yaw; };
	
	/***********************
	* GetRotationRoll: Retrieve the Objects current rotation around the Z axis
	* @author: Callan Moore
	* @return: float: Rotation in radians
	********************/
	float GetRotationRoll() { return m_rotation_Roll; };
	
	/***********************
	* GetRotPerFramePitch: Retrieve the Rotation per frame around the X axis
	* @author: Callan Moore
	* @return: float: The rotation that occurs each frame in radians
	********************/
	float GetRotPerFramePitch() { return m_rotPerSecond_Pitch; };
	
	/***********************
	* GetRotPerFrameYaw: Retrieve the Rotation per frame around the X axis
	* @author: Callan Moore
	* @return: float: The rotation that occurs each frame in radians
	********************/
	float GetRotPerFrameYaw() { return m_rotPerSecond_Yaw; };
	
	/***********************
	* GetRotPerFrameRoll: Retrieve the Rotation per frame around the X axis
	* @author: Callan Moore
	* @return: float: The rotation that occurs each frame in radians
	********************/
	float GetRotPerFrameRoll() { return m_rotPerSecond_Roll; };
	
	/***********************
	* SetPosition: Set the Position of the object
	* @author: Callan Moore
	* @parameter: _pos: 3D position in world space for the center of the object
	* @return: void 
	********************/
	void SetPosition(v3float _pos) { m_pos = _pos; };
	
	/***********************
	* SetRotationPitch: Set the rotation around the X axis
	* @author: Callan Moore
	* @parameter: _rotX: Rotation in radians
	* @return: void
	********************/
	void SetRotationPitch(float _rotX) { m_rotation_Pitch = _rotX; };
	
	/***********************
	* SetRotationYaw: Set the rotation around the Y axis
	* @author: Callan Moore
	* @parameter: _rotY: Rotation in radians
	* @return: void
	********************/
	void SetRotationYaw(float _rotY) { m_rotation_Yaw = _rotY; };
	
	/***********************
	* SetRotationRoll: Set the rotation around the Z axis
	* @author: Callan Moore
	* @parameter: _rotZ: Rotation in radians
	* @return: void
	********************/
	void SetRotationRoll(float _rotZ) { m_rotation_Roll = _rotZ; };
	
	/***********************
	* SetRotPerSecondPitch: Set the rotation increment that will occur each Second around the X axis
	* @author: Callan Moore
	* @parameter: _rotX: Rotation in radians
	* @return: void
	********************/
	void SetRotPerSecondPitch(float _rotX) { m_rotation_Pitch = _rotX; };
	
	/***********************
	* SetRotPerSecondYaw: Set the rotation increment that will occur each Second around the X axis
	* @author: Callan Moore
	* @parameter: _rotY: Rotation in radians
	* @return: void
	********************/
	void SetRotPerSecondYaw(float _rotY) { m_rotation_Yaw = _rotY; };
	
	/***********************
	* SetRotPerSecondRoll: Set the rotation increment that will occur each Second around the X axis
	* @author: Callan Moore
	* @parameter: _rotZ: Rotation in radians
	* @return: void
	********************/
	void SetRotPerSecondRoll(float _rotZ) { m_rotation_Roll = _rotZ; };
	
	/***********************
	* SetScroll: Set the Object to be a scrolling object with scroll speed and direction
	* @author: Callan Moore
	* @parameter: _scrollSpeed: The speed to do a full scroll cycle in seconds
	* @parameter: _scrollDir: The direction to scroll in
	* @return: void
	********************/
	virtual void SetScroll(float _scrollSpeed, v2float _scrollDir)
	{	
		m_scrolling = true;
		m_scrollSpeed = _scrollSpeed;
		m_scrollDir = _scrollDir.Normalise();
	}

	/***********************
	* Draw: Direct call to get the object to draw itself without going through its shader
	* @author: Callan Moore
	* @return: void
	********************/
	void Draw()
	{
		m_pMesh->Render();
	}

	/***********************
	* GetWorldMatrix: Retrieve the objects world matrix
	* @author: Callan Moore
	* @return: D3DXMATRIX: A world matrix
	********************/
	D3DXMATRIX GetWorldMatrix() { return m_matWorld; };
	
	/***********************
	* GetTexture: Retrieve the objects texture
	* @author: Callan Moore
	* @return: ID3D10ShaderResourceView*: The objects texture as a dx10 resource
	********************/
	ID3D10ShaderResourceView* GetTexture() { return (*m_pTextures)[0]; };
	
	/***********************
	* GetMesh: Retrieve the mesh of the object
	* @author: Callan Moore
	* @return: DX10_Mesh*: The mesh of the object
	********************/
	DX10_Mesh* GetMesh() { return m_pMesh; };

protected:
	DX10_Renderer* m_pRenderer;
	DX10_Mesh* m_pMesh;
	D3DXCOLOR m_color;

	float m_dt;

	v3float m_pos;
	float m_rotation_Pitch;
	float m_rotation_Yaw;
	float m_rotation_Roll;
	float m_rotPerSecond_Pitch;
	float m_rotPerSecond_Yaw;
	float m_rotPerSecond_Roll;

	D3DXMATRIX m_matWorld;

	std::vector<ID3D10ShaderResourceView*>* m_pTextures;

	// Animating
	bool m_animating;
	UINT m_texIndex;
	float m_animationTimer;
	float m_animationSpeed;

	// Scrolling
	bool m_scrolling;
	float m_scrollTimer;
	float m_scrollSpeed;
	v2float m_scrollDir;
	v2float m_scrollTranslation;
};

#endif	// __DX10_OBJ_GENERIC_H__

