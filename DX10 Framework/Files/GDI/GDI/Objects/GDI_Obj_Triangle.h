/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Triangle.h
* Description : A Triangle for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_TRIANGLE_H__
#define __GDI_OBJ_TRIANGLE_H__

// Local Includes
#include "GDI_Obj_Generic.h"

class GDI_Obj_Triangle :
	public GDI_Obj_Generic
{
public:

	/***********************
	* GDI_Obj_Triangle: Constructor for GDI Triangle class
	* @author: Callan Moore
	* @parameter: _pGDIRenderer: GDI renderer for this object
	********************/
	GDI_Obj_Triangle(GDI_Renderer* _pGDIRenderer);

	/***********************
	* ~GDI_Obj_Triangle: Default Destructor for GDI Triangle class
	* @author: Callan Moore
	********************/
	virtual ~GDI_Obj_Triangle();

	/***********************
	* Initialise: Initialise the Triangle for use
	* @author: Callan Moore
	* @parameter: _pPhysicsBody: The physics body to govern this objects physics interactions
	* @parameter: _color: Color of the Triangle
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _color);

	/***********************
	* Process: Process the new frame and update the Triangle Object
	* @author: Callan Moore
	* @parameter: _dt: The current Delta Tick
	* @return: void
	********************/
	virtual void Process(float _dt);

};
#endif	// __GDI_OBJ_TRIANGLE_H__

