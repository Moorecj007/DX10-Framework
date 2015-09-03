/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Body_2D.h
* Description : A body that holds all physics information for a 2D object
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_BODY_2D_H__
#define __PHYSICS_BODY_2D_H__

// Library Includes
#include <Box2D.h>

// Local Includes
#include "Physics_Utilities.h"

class Physics_Body_2D
{
public:

	/***********************
	* Physics_Body_2D: Constructor for Physics Body 2D for a circle object
	* @author: Callan Moore
	* @parameter: _pBody: The Box2D physics body to govern this objects physics interactions
	* @parameter: _radius: Radius of the circle
	* @parameter: _pixelsPerMeter: The conversion rate of meters to screen space
	********************/
	Physics_Body_2D(b2Body* _pBody, float _radius, float _pixelsPerMeter);

	/***********************
	* Physics_Body_2D: Constructor for Physics Body 2D for a polygon object
	* @author: Callan Moore
	* @parameter: _pBody: The Box2D physics body to govern this objects physics interactions
	* @parameter: _pPoints: The points for the body to be created from
	* @parameter: _size: Size of the array of points
	* @parameter: _pixelsPerMeter: The conversion rate of meters to screen space
	********************/
	Physics_Body_2D(b2Body* _pBody, v2float* _pPoints, UINT _size, float _pixelsPerMeter);

	/***********************
	* ~Physics_Body_2D: Default Destructor for Physics Body 2D class
	* @author: Callan Moore
	********************/
	~Physics_Body_2D();

	/***********************
	* GetBody: Retrieve the Box2D body of this object
	* @author: Callan Moore
	* @return: b2Body*: The Box2d Body of the physics object
	********************/
	b2Body* GetBody() { return m_pBody; };
	
	/***********************
	* GetBreakProperties: Retrieve the Break properties of the body
	* @author: Callan Moore
	* @return: TBreakProperties*: The break properties of the body
	********************/
	TBreakProperties* GetBreakProperties() { return &m_breakProps; };
	
	/***********************
	* GetRadius: Retrieve the radius for a circle body
	* @author: Callan Moore
	* @return: float: The radius of the circle
	********************/
	float GetRadius() { return m_radius * m_pixelsPerMeter; };

	/***********************
	* GetPoints: Retrieve the array of points that make up the body
	* @author: Callan Moore
	* @return: v2float*: The array of points
	********************/
	v2float* GetPoints() { return m_pPoints; };

	/***********************
	* GetSize: Retrieve the size of the body array of points
	* @author: Callan Moore
	* @return: UINT: Amount of points for the body
	********************/
	UINT GetSize() { return m_size; };
	
	/***********************
	* GetPosition: Retrieve the position of the Body
	* @author: Callan Moore
	* @return: v2float: The position as a float vector 2D
	********************/
	v2float GetPosition();

	/***********************
	* GetAngle: Retrieve the rotation angle of the Body
	* @author: Callan Moore
	* @return: float: Rotation angle of the Body
	********************/
	float GetAngle() { return m_pBody->GetAngle(); };
	
	/***********************
	* SetBreakProperties: Set a new Break properties structure for the Body
	* @author: Callan Moore
	* @parameter: _breakProps: The new Break properties structure
	* @return: void
	********************/
	void SetBreakProperties(TBreakProperties _breakProps) { m_breakProps = _breakProps; };

private:
	b2Body* m_pBody;
	TBreakProperties m_breakProps;
	float m_pixelsPerMeter;

	// Circle Properties
	float m_radius;

	// Polygon Properties
	v2float* m_pPoints;
	UINT m_size;	
};
#endif	// __PHYSICS_BODY_2D_H__
