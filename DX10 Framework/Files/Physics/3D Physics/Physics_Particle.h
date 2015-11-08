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
#include "../../DX10/DX10/DX10_Utilities.h"
#include "../../DX10/DX10/DX10_Vertex.h"

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
	
	/***********************
	* Initialise: Initialise the Physics Particle for use
	* @author: Callan Moore
	* @parameter: _particleID: TO DO CAL
	* @parameter: _pVertex: Pointer to the Vertex that is used for drawing this particle
	* @parameter: _pos: The first position for the particle
	* @parameter: _timeStep: The time step used for physics calculations
	* @parameter: _damping: The damping of the particle
	* @parameter: _static: Whether the particle is affected by forces
	* @return: bool: Successful or not
	********************/
	bool Initialise(int _particleID, TVertexColor* _pVertex, v3float _pos, float _timeStep, float _damping, bool _static = false);
	
	/***********************
	* Process: Process the Particle
	* @author: Callan Moore
	* @return: void
	********************/
	void Process();
	
	/***********************
	* Move: The the particle by the input amount (only if the particle is not static)
	* @author: Callan Moore
	* @parameter: _movement: Vector to move the particle by
	* @return: void
	********************/
	void Move(v3float _movement);

	/***********************
	* AddForce: Add force to the particle in the input direction
	* @author: Callan Moore
	* @parameter: _force: A directional force to apply to the particle
	* @return: void
	********************/
	void AddForce(v3float _force) { m_acceleration += (_force / m_mass); };
	
	/***********************
	* SetStaticState: Set the static state of the particle
	* @author: Callan Moore
	* @parameter: _state: The new state
	* @return: void
	********************/
	void SetStaticState(bool _state) { m_static = _state; };
	
	/***********************
	* ResetAcceleration: Reset the acceleration of the particle to zero
	* @author: Callan Moore
	* @return: void
	********************/
	void ResetAcceleration() { m_acceleration = { 0.0f, 0.0f, 0.0f }; };
	
	/***********************
	* GetPosition: Retrieve the position of the particle
	* @author: Callan Moore
	* @return: v3float*: The position of the particle in world space
	********************/
	v3float* GetPosition() { return &m_pos; };
	
	/***********************
	* SetPosition: Set the position of the particle
	* @author: Callan Moore
	* @parameter: _pos: Particles new position
	* @return: void
	********************/
	void SetPosition(v3float _pos) { m_pos = m_prevPos = _pos; };

	// TO DO CAL
	void SetSelectedPosition(v3float _pos);
	void SetSelectedState(bool _selected) { m_selected = _selected; };
	bool GetSelectedState() { return m_selected; };
	bool GetStaticState() { return m_static; };
	int GetParticleID() { return m_particleID; };

	// FOR JC
	// TO DO CAL
	std::vector<UINT> GetContraintIndices() { return m_contraintIndices; };
	void AddContraintIndex(UINT _index) { m_contraintIndices.push_back(_index); };
	bool GetIgnitedStatus() { return m_ignited; };
	void Ignite(float _burnTime);
	void Reset();

private:
	int m_particleID;
	bool m_static;
	bool m_selected;
	
	v3float m_pos;
	v3float m_prevPos;
	v3float m_acceleration;

	float m_mass;
	float m_timeStep;	// FOR JC
	float m_timeStepSquared;
	float m_damping;
	float m_dampingInverse;

	// FOR JC
	TVertexColor* m_pVertex;
	std::vector<UINT> m_contraintIndices;
	bool m_ignited;
	float m_lightTime;
	float m_destroyTime;
	float m_timeUntilFullyLit;
	float m_timeUntilDestroyed;
};
#endif	// __PHYSICS_PARTICLE_H__

