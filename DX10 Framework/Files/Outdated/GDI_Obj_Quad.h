/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Quad.h
* Description : A Quadrilateral for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_QUAD_H__
#define __GDI_OBJ_QUAD_H__

// Local Includes
#include "GDI_Obj_Generic.h"

class GDI_Obj_Quad :
	public GDI_Obj_Generic
{
public:

	/***********************
	* GDI_Obj_Quad: Constructor for GDI Quad class
	* @author: Callan Moore
	* @parameter: _pGDIRenderer: GDI renderer for this object
	********************/
	GDI_Obj_Quad(GDI_Renderer* _pGDIRenderer);

	/***********************
	* ~GDI_Obj_Quad: Default Destructor for GDI Quad class
	* @author: Callan Moore
	********************/
	virtual ~GDI_Obj_Quad();

	/***********************
	* Initialise: Initialise the Quad for use
	* @author: Callan Moore
	* @parameter: _pPhysicsBody: The physics body to govern this objects physics interactions
	* @parameter: _color: Color of the Quad
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _color);
	
	/***********************
	* Process: Process the new frame and update the Quad Object
	* @author: Callan Moore
	* @parameter: _dt: The current Delta Tick
	* @return: void
	********************/
	virtual void Process(float _dt);
};

#endif	// __GDI_OBJ_QUAD_H__
