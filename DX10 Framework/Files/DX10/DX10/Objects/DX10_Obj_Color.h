/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_Color.h
* Description : 3D Color Object for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_OBJ_COLOR_H__
#define __DX10_OBJ_COLOR_H__

// Local Includes
#include "DX10_Obj_Generic.h"

class DX10_Obj_Color :
	public DX10_Obj_Generic
{
public:

	/***********************
	* DX10_Color_Obj: Default Constructor for Color Object class
	* @author: Callan Moore
	********************/
	DX10_Obj_Color();

	/***********************
	* ~DX10_Color_Obj: Default Constructor for Color Object class
	* @author: Callan Moore
	********************/
	~DX10_Obj_Color();

	/***********************
	* Process: Process the new frame and update the LitTex Object
	* @author: Callan Moore
	* @parameter: _dt: The delta tick for this frame
	* @return: void
	********************/
	virtual void Process(float _dt);

	/***********************
	* Render: Render the Color Object to the screen space
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render();
};

#endif	// __DX10_OBJ_COLOR_H__

