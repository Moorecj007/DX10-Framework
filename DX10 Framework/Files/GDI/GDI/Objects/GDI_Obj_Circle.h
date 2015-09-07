/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Circle.h
* Description : A Circle for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_CIRCLE_H__
#define __GDI_OBJ_CIRCLE_H__

// Local Includes
#include "GDI_Obj_Generic.h"

class GDI_Obj_Circle :
	public GDI_Obj_Generic
{
public:
	/***********************
	* GDI_Obj_Circle: Constructor for GDI Circle class
	* @author: Callan Moore
	* @parameter: _pGDIRenderer: GDI renderer for this object
	********************/
	GDI_Obj_Circle(GDI_Renderer* _pGDIRenderer);

	/***********************
	* ~GDI_Obj_Circle: Default Destructor for GDI Circle class
	* @author: Callan Moore
	********************/
	virtual ~GDI_Obj_Circle();

	/***********************
	* Initialise: Initialise the Circle for use
	* @author: Callan Moore
	* @parameter: _pPhysicsBody: The physics body to govern this objects physics interactions
	* @parameter: _colorFill: Color to fill the circle with
	* @parameter: _colorOutline: Color of the outline for the circle
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _colorFill, COLORREF _colorOutline);

	/***********************
	* Process: Process the new frame and update the Circle Object
	* @author: Callan Moore
	* @parameter: _dt: The current Delta Tick
	* @return: void
	********************/
	virtual void Process(float _dt);

	/***********************
	* Render: Render the Circle
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render();
};
#endif	// __GDI_OBJ_CIRCLE_H__

