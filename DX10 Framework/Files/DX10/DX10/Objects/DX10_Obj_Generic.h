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
	* CGenericObject: Default Constructor for Generic Object class
	* @author: Callan Moore
	********************/
	DX10_Obj_Generic();

	/***********************
	* ~CGenericObject: Default Destructor for Generic Object class
	* @author: Callan Moore
	********************/
	virtual ~DX10_Obj_Generic();

	/***********************
	* BaseInitialise: Initialise the Base member variables
	* @author: Callan Moore
	* @return: void
	********************/
	void BaseInitialise();

	/***********************
	* Initialise: Initialise the Generic Object for use
	* @author: Callan Moore
	* @parameter: _pRenderer: Renderer for this Application
	* @parameter: _pMesh: Generic Mesh for this Object
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(DX10_Renderer* _pRenderer, DX10_Mesh_Generic* _pMesh);
	
	/***********************
	* Process: Process the new frame and update the Generic Object
	* @author: Callan Moore
	* @parameter: _dt: The delta tick for this frame
	* @return: void
	********************/
	virtual void Process(float _dt) = 0;
	
	/***********************
	* Render: Render the Generic Object to the screen space
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render() = 0;
	
	/***********************
	* CalcWorldMatrix: Calculate the World Matrix 
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void CalcWorldMatrix();

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

