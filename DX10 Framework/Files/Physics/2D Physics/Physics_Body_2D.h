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
#include "../../Utility/Utilities.h"

class Physics_Body_2D
{
public:

	/***********************
	* Physics_Body_2D: Constructor for Physics Body 2D class
	* @author: Callan Moore
	* @parameter: _pBody: The Box2D physics body to govern this objects physics interactions
	* @parameter: _scale: Scale of the object (width and height)
	* @parameter: _pixelsPerMeter: The conversion rate of meters to screen space
	********************/
	Physics_Body_2D(b2Body* _pBody, v2float _scale, float _pixelsPerMeter);

	/***********************
	* ~Physics_Body_2D: Default Destructor for Physics Body 2D class
	* @author: Callan Moore
	********************/
	~Physics_Body_2D();
	
	/***********************
	* GetPosition: Retrieve the position of the Body
	* @author: Callan Moore
	* @return: v2float: The position as a float vector 2D
	********************/
	v2float GetPosition();
	
	/***********************
	* GetScale: Retrieve the Scale of the Body
	* @author: Callan Moore
	* @return: v2float: The scale as a float vector 2D
	********************/
	v2float GetScale() { return m_scale * m_pixelsPerMeter; };

	/***********************
	* GetAngle: Retrieve the rotation angle of the Body
	* @author: Callan Moore
	* @return: float: Rotation angle of the Body
	********************/
	float GetAngle() { return m_pBody->GetAngle(); };

	/***********************
	* GetBody: Retrieve the Box2D body of this object
	* @author: Callan Moore
	* @return: b2Body*: The Box2d Body of the physics object
	********************/
	b2Body* GetBody() { return m_pBody; };

private:
	b2Body* m_pBody;
	v2float m_scale;
	float m_pixelsPerMeter;
};
#endif	// __PHYSICS_BODY_2D_H__
