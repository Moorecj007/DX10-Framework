/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Polygon.h
* Description : A Polygon for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_POLYGON_H__
#define __GDI_OBJ_POLYGON_H__

// Local Includes
#include "GDI_Obj_Generic.h"

class GDI_Obj_Polygon :
	public GDI_Obj_Generic
{
public:
	/***********************
	* GDI_Obj_Polygon: Constructor for GDI Polygon class
	* @author: Callan Moore
	* @parameter: _pGDIRenderer: GDI renderer for this object
	********************/
	GDI_Obj_Polygon(GDI_Renderer* _pGDIRenderer);

	/***********************
	* ~GDI_Obj_Polygon: Default Destructor for GDI Polygon class
	* @author: Callan Moore
	********************/
	virtual ~GDI_Obj_Polygon();

	/***********************
	* Initialise: Initialise the Polygon for use
	* @author: Callan Moore
	* @parameter: _pPhysicsBody: The physics body to govern this objects physics interactions
	* @parameter: _colorFill: Color of the Polygons Fill
	* @parameter: _colorOutline: Color of the Polygons Outline
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _colorFill, COLORREF _colorOutline);

	/***********************
	* Process: Process the new frame and update the Polygon Object
	* @author: Callan Moore
	* @parameter: _dt: The current Delta Tick
	* @return: void
	********************/
	virtual void Process(float _dt);
};
#endif	// __GDI_OBJ_POLYGON_H__

