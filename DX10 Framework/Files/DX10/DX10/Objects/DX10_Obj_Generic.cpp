/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_Generic.cpp
* Description : 3D Generic Object
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_Obj_Generic.h"

DX10_Obj_Generic::DX10_Obj_Generic()
{
	
}

DX10_Obj_Generic::~DX10_Obj_Generic()
{
	
}

void DX10_Obj_Generic::BaseInitialise()
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

bool DX10_Obj_Generic::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh_Generic* _pMesh)
{
	// Save the Renderer and Mesh on the Object
	m_pRenderer = _pRenderer;
	m_pMesh = _pMesh;

	return true;
}

void DX10_Obj_Generic::CalcWorldMatrix()
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