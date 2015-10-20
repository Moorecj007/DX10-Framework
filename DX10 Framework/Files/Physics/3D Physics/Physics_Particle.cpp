/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Particle.cpp
* Description : TO DO CAL
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Particle.h"

Physics_Particle::Physics_Particle()
{
}

Physics_Particle::~Physics_Particle()
{
}

bool Physics_Particle::Initialise(v3float _pos, float _timeStep, float _damping, bool _static)
{
	// Assign member variables
	m_pos = _pos;
	m_timeStepSquared = pow(_timeStep, 2.0f);
	m_damping = _damping;
	m_static = _static;

	m_mass = 1.0f;
	m_prevPos = m_pos;
	m_acceleration = { 0.0f, 0.0f, 0.0f };

	m_dampingInverse = (1.0f - m_damping);

	return true;
}

void Physics_Particle::Process()
{
	if (m_static == false)
	{
		v3float temp = m_pos;
		m_pos = m_pos + ((m_pos - m_prevPos) * m_dampingInverse) + (m_acceleration * m_timeStepSquared);
		m_prevPos = temp;
		m_acceleration = { 0.0f, 0.0f, 0.0f};
	}
}

void Physics_Particle::Move(v3float _movement)
{
	if (m_static == false)
	{
		m_pos += _movement;
	}
}