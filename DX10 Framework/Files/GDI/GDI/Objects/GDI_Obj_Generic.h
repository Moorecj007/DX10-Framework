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
	* ~GDI_Obj_Generic: Default Destructor for GDI Generic Object class
	* @author: Callan Moore
	********************/
	virtual ~GDI_Obj_Generic()
	{
		ReleasePtr(m_pPhysicsBody);
		ReleasePtrArray(m_points);
	}

	/***********************
	* Initialise: Initialise the Object for use
	* @author: Callan Moore
	* @parameter: _pPhysicsBody: The physics body to govern this objects physics interactions
	* @parameter: _colorFill: Color of the objects fill
	* @parameter: _colorOutline: Color of the objects outline
	* @return: bool: Successful or not
	********************/
	virtual bool Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _colorFill, COLORREF _colorOutline) = 0;

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
	virtual void Render()
	{
		m_pGDIRenderer->RenderPolygon(m_points, m_colorFill, m_colorOutline, m_size);
	}
	
	/***********************
	* GetPhysicsBody: Retrieve the Physics Body if the Object
	* @author: Callan Moore
	* @return: Physics_Body_2D*: The physics body of the object
	********************/
	Physics_Body_2D* GetPhysicsBody() { return m_pPhysicsBody; };

	/***********************
	* GetColorFill: Retrieve the Color of the Objects fill
	* @author: Callan Moore
	* @return: COLORREF: The color of the objects fill 
	********************/
	COLORREF GetColorFill()	{ return m_colorFill; };

	/***********************
	* GetColorOutline: Retrieve the Color of the Objects outline
	* @author: Callan Moore
	* @return: COLORREF: The color of the objects Outline
	********************/
	COLORREF GetColorOutline()	{ return m_colorOutline; };

protected:

	/***********************
	* CalcRotation: Calculate the rotation of the point by the given angle
	* @author: Callan Moore
	* @parameter: _point: The point position to rotate
	* @parameter: _angle: the rotation angle
	* @parameter: _pos: Center of the object containing the point
	* @return: void
	********************/
	void CalcRotation(v2float* _point, float _angle, v2float _pos)
	{
		// Translate the point to origin
		(*_point).x = (*_point).x - _pos.x;
		(*_point).y = (*_point).y - _pos.y;

		v2float pt;
		pt.x = ((*_point).x * cos(_angle) - (*_point).y * sin(_angle));
		pt.y = ((*_point).y * cos(_angle) + (*_point).x * sin(_angle));

		(*_point).x = pt.x + _pos.x;
		(*_point).y = pt.y + _pos.y;
	}

protected:

	GDI_Renderer* m_pGDIRenderer;

	v2float* m_points;
	COLORREF m_colorFill;
	COLORREF m_colorOutline;
	int m_size;

	// Physics
	Physics_Body_2D* m_pPhysicsBody;
};
#endif	// __GDI_OBJ_GENERIC_H__

