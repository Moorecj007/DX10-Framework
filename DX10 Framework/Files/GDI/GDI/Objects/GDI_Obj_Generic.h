/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Generic.h
* Description : A Generic Object for GDI use (Abstract)
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_GENERIC_H__
#define __GDI_OBJ_GENERIC_H__

// Local Includes
#include "../GDI_Renderer.h"
#include "../../../Physics/2D Physics/Physics_Body_2D.h"

// Library Includes
#include <vector>

class GDI_Obj_Generic
{
public:
	/***********************
	* GDI_Obj_Generic: Constructor for GDI Generic Object class
	* @author: Callan Moore
	********************/
	GDI_Obj_Generic();

	/***********************
	* ~GDI_Obj_Generic: Default Destructor for GDI Generic Object class
	* @author: Callan Moore
	********************/
	virtual ~GDI_Obj_Generic();

	/***********************
	* Initialise: Initialise the Object for use
	* @author: Callan Moore
	* @parameter: _pPhysicsBody: The physics body to govern this objects physics interactions
	* @parameter: _color: Color of the Quad
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _color) = 0;

	/***********************
	* Process: Process the new frame and update the Object
	* @author: Callan Moore
	* @parameter: _dt: The current Delta Tick
	* @return: void
	********************/
	virtual void Process(float _dt) = 0;

	/***********************
	* Render: Render the Object
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render();
	
	/***********************
	* GetPhysicsBody: Retrieve the Physics Body if the Object
	* @author: Callan Moore
	* @return: Physics_Body_2D*: The physics body of the object
	********************/
	Physics_Body_2D* GetPhysicsBody() { return m_pPhysicsBody; };

	/***********************
	* GetColor: Retrieve the Color of the Object
	* @author: Callan Moore
	* @return: COLORREF: The color of the object
	********************/
	virtual COLORREF GetColor()	{ return m_color; };

protected:

	/***********************
	* CalcRotation: Calculate the rotation of the point by the given angle
	* @author: Callan Moore
	* @parameter: _point: The point position to rotate
	* @parameter: _angle: the rotation angle
	* @parameter: _pos: Center of the object containing the point
	* @return: void
	********************/
	virtual void CalcRotation(v2float* _point, float _angle, v2float _pos);

protected:

	GDI_Renderer* m_pGDIRenderer;

	v2float* m_points;
	COLORREF m_color;
	int m_size;

	// Physics
	Physics_Body_2D* m_pPhysicsBody;
};
#endif	// __GDI_OBJ_GENERIC_H__

