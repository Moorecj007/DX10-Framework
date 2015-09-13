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
#include "../Meshes/DX10_Mesh_Generic.h"

class DX10_Obj_Generic
{
public:

	/***********************
	* ~CGenericObject: Default Destructor for Generic Object class
	* @author: Callan Moore
	********************/
	virtual ~DX10_Obj_Generic() {}

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
		m_rotationPitch = 0;
		m_rotationYaw = 0;
		m_rotationRoll = 0;

		// Set IDs to zero (Invalid ID)
		m_techniqueID = 0;
		m_fxID = 0;
		m_vertexLayoutID = 0;

		// Set all pointers to NULL
		m_pRenderer = 0;
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
		D3DXMatrixRotationX(&matRotatePitch, m_rotationPitch);
		D3DXMatrixRotationY(&matRotateYaw, m_rotationYaw);
		D3DXMatrixRotationZ(&matRotateRoll, m_rotationRoll);

		// Calculate the Full Rotation Matrix and store in the World Matrix
		m_matWorld = matRotatePitch * matRotateYaw * matRotateRoll;

		// Calculate a translation Matrix based on the Objects current position in world space
		D3DXMatrixTranslation(&matTranslation, m_pos.x, m_pos.y, m_pos.z);

		// Calculate the Objects World Matrix
		m_matWorld *= matTranslation;
	}

protected:
	DX10_Renderer* m_pRenderer;
	DX10_Mesh_Generic* m_pMesh;
	D3DXCOLOR m_color;

	float m_dt;

	v3float m_pos;
	float m_rotationPitch;
	float m_rotationYaw;
	float m_rotationRoll;
	
	UINT m_fxID;
	UINT m_techniqueID;
	UINT m_vertexLayoutID;

	D3DXMATRIX m_matWorld;
};

#endif	// __DX10_OBJ_GENERIC_H__

