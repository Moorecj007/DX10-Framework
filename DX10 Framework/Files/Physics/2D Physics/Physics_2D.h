/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : 2D_Physics.h
* Description : Simulates all 2D physics for the application
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __2D_PHYSICS_H__
#define __2D_PHYSICS_H__

// Library Includes
#pragma comment(lib, "Box2D.lib")
#include <Box2D.h>

// Local Includes
#include "../../Utility/Utilities.h"
#include "Physics_Body_2D.h"

// Struct
struct TPhysicsProperties
{
	bool movable;
	v2float	pos;
	v2float scale;
	float density;
	float friction;
	float restitution;
	float angle;
};

class Physics_2D
{
public:

	/***********************
	* C2D_Physics: Default Constructor for 2D Physics class
	* @author: Callan Moore
	********************/
	Physics_2D();

	/***********************
	* ~C2D_Physics: Default Destructor for 2D Physics class
	* @author: Callan Moore
	********************/
	~Physics_2D();

	/***********************
	* Initialise: Initialise the Physics 2D simulator
	* @author: Callan Moore
	* @parameter: _pixelsPerMeter: The conversion rate of meters to screen space
	* @return: bool: Successful or not
	********************/
	bool Initialise(float _pixelsPerMeter);

	/***********************
	* Process: Process the Physics world and step the bodies forward by the timestep
	* @author: Callan Moore
	* @return: void
	********************/
	void Process();

	/***********************
	* CreatePhysicsObject_Quad: Create a Physics body for a 2D Quad
	* @author: Callan Moore
	* @parameter: _props: Properties for the Physics object
	* @return: Physics_Body_2D*: Physics Body for a 2D object
	********************/
	Physics_Body_2D* Physics_2D::CreatePhysicsObject_Quad(TPhysicsProperties _props);

	/***********************
	* CreatePhysicsObject_Tri: Create a Physics body for a 2D Triangle
	* @author: Callan Moore
	* @parameter: _props: Properties for the Physics object
	* @return: Physics_Body_2D*: Physics Body for a 2D object
	********************/
	Physics_Body_2D* Physics_2D::CreatePhysicsObject_Tri(TPhysicsProperties _props);

	/***********************
	* CreatePhysicsObject_Circle: Create a Physics body for a 2D Circle
	* @author: Callan Moore
	* @parameter: _props: Properties for the Physics object
	* @return: Physics_Body_2D*: Physics Body for a 2D object
	********************/
	Physics_Body_2D* Physics_2D::CreatePhysicsObject_Circle(TPhysicsProperties _props);
	
	/***********************
	* CreateResoluteJoint: Creates a Resolute Joint between two physics objects
	* @author: Callan Moore
	* @parameter: _bodyA: First Physics Body
	* @parameter: _bodyB: Second Physics Body
	* @parameter: _jointPos: Position of the Joint in world space
	* @return: void
	********************/
	void CreateResoluteJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _jointPos);

	// TO DO
	void CreateRopeJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _anchorPosA, v2float _anchorPosB);

private:

	// World Variables
	b2World* m_pWorld;
	bool m_allowSleep;
	float m_timeStep;
	int m_velocityIterations;
	int m_posIterations;

	// Conversion to 2D screen space
	float m_pixelsPerMeter;
};

#endif	// __2D_PHYSICS_H__



