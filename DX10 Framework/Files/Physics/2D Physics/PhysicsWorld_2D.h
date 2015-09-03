/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : PhysicsWorld_2D.h
* Description : Simulates all 2D physics for the application
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICSWORLD_2D_H__
#define __PHYSICSWORLD_2D_H__

// Local Includes
#include "Physics_Utilities.h"
#include "Physics_Body_2D.h"
#include "Physics_Rope_2D.h"
#include "Physics_Joint_2D.h"
#include "Physics_Break_Listener.h"

class PhysicsWorld_2D
{
public:

	/***********************
	* C2D_Physics: Default Constructor for 2D Physics class
	* @author: Callan Moore
	********************/
	PhysicsWorld_2D();

	/***********************
	* ~C2D_Physics: Default Destructor for 2D Physics class
	* @author: Callan Moore
	********************/
	~PhysicsWorld_2D();

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
	* CreatePhysicsObject: Create a Physics body for a 2D Object. 
	* @author: Callan Moore
	* @parameter: _props: Properties for the Physics object
	* @return: Physics_Body_2D*: Physics Body for a 2D object
	********************/
	Physics_Body_2D* CreatePhysicsObject(TPhysicsProperties _props);

	/***********************
	* CreatePhysicsPolygon: Create a Physics body for a 2D Polygon
	* @author: Callan Moore
	* @parameter: _props: Properties for the Physics object
	* @return: Physics_Body_2D*: Physics Body for a 2D object
	********************/
	Physics_Body_2D* CreatePhysicsPolygon(TPhysicsProperties _props);

	/***********************
	* CreatePhysicsCircle: Create a Physics body for a 2D Circle
	* @author: Callan Moore
	* @parameter: _props: Properties for the Physics object
	* @return: Physics_Body_2D*: Physics Body for a 2D object
	********************/
	Physics_Body_2D* CreatePhysicsCircle(TPhysicsProperties _props);
	
	/***********************
	* CreateResoluteJoint: Creates a Resolute Joint between two physics objects
	* @author: Callan Moore
	* @parameter: _bodyA: First Physics Body
	* @parameter: _bodyB: Second Physics Body
	* @parameter: _jointPos: Position of the Joint in world space
	* @parameter: _collide: Can the two connect bodies collide with each other
	* @return: void
	********************/
	void CreateRevoluteJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _jointPos, bool _collide);

	/***********************
	* CreateRopeJoint: Create a rope joint between to Bodies at the given anchor points (relative to the bodies center)
	* @author: Callan Moore
	* @parameter: Physics_Body_2D * _bodyA: First Body to rope
	* @parameter: Physics_Body_2D * _bodyB: Second body to rope
	* @parameter: v2float _relativeAnchorA: Anchor point on the first body
	* @parameter: v2float _relativeAnchorB: Anchor point on the second body
	* @parameter: _collide: Can the two connect bodies collide with each other
	* @return: Physics_Joint_2D*: Reference to the created Joint
	********************/
	//Physics_Joint_2D* CreateRopeJoint(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, bool _collide);

	// TO DO
	Physics_Rope_2D*  CreateRope(Physics_Body_2D* _bodyA, Physics_Body_2D* _bodyB, v2float _relativeAnchorA, v2float _relativeAnchorB, COLORREF _color);

	// TO DO
	std::vector<Physics_Body_2D*>* BreakObject(Physics_Body_2D* _body);

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

#endif	// __PHYSICSWORLD_2D_H__


