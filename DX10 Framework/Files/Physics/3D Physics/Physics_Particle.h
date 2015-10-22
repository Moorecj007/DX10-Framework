/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Particle.h
* Description : TO DO CAL
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_PARTICLE_H__
#define __PHYSICS_PARTICLE_H__

// Local Includes
#include "../../Utility/Utilities.h"

class Physics_Particle
{
public:
	/***********************
	* Physics_Particle: Default Constructor for Physics Particle class
	* @author: Callan Moore
	********************/
	Physics_Particle();

	/***********************
	* Physics_Particle: Constructor for Physics Particle class
	* @author: Callan Moore
	* @parameter: _pos: Starting position for the Particle
	********************/
	Physics_Particle(v3float _pos);

	/***********************
	* ~Physics_Particle: Default Destructor for Physics Particle class
	* @author: Callan Moore
	********************/
	~Physics_Particle();

	// TO DO CAL
	bool Initialise(v3float _pos, float _timeStep, float _damping, bool _static = false);
	void Process();

	void AddForce(v3float _force) { m_acceleration += (_force / m_mass); };
	void Move(v3float _movement);
	void StaticState(bool _state) { m_static = _state; };
	void ResetAcceleration() { m_acceleration = { 0.0f, 0.0f, 0.0f }; };
	v3float* GetPosition() { return &m_pos; };
	void SetPosition(v3float _pos) { m_pos = m_prevPos = _pos; };

private:
	bool m_static;
	float m_mass;
	v3float m_pos;
	v3float m_prevPos;
	v3float m_acceleration;

	float m_timeStepSquared;
	float m_damping;
	float m_dampingInverse;
};
#endif	// __PHYSICS_PARTICLE_H__

