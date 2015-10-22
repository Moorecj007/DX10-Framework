/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Cloth.h
* Description : TO DO CAL
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_CLOTH_H__
#define __PHYSICS_CLOTH_H__

// Local Includes
#include "Physics_Constraint.h"
#include "../../DX10/DX10/DX10_Renderer.h"
#include "../../DX10/DX10/Meshes/DX10_Mesh.h"
#include "../../DX10/DX10/Shaders/DX10_Shader_LitTex.h"

class Physics_Cloth
{
public:
	/***********************
	* Physics_Cloth: Default Constructor for Physics Cloth class
	* @author: Callan Moore
	********************/
	Physics_Cloth();

	/***********************
	* ~Physics_Cloth: Default Destructor for Physics Cloth class
	* @author: Callan Moore
	********************/
	~Physics_Cloth();
	
	/***********************
	* Initialise: Initialise the Physics Cloth for use
	* @author: Callan Moore
	* @parameter: _pRenderer: The renderer for this object
	* @parameter: _pShader: The shader that will be used to render the object
	* @parameter: _width: The width of the Cloth
	* @parameter: _height: The height of the Cloth
	* @parameter: _damping: The damping to apply to the cloth
	* @parameter: _timeStep: The time step for the physics calculations
	* @return: bool: TO DO
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Shader_LitTex* _pShader, int _width, int _height, float _damping = 0.01f, float _timeStep = 0.016667f);
	
	/***********************
	* Process: Process the Cloth for the time step
	* @author: Callan Moore
	* @return: void
	********************/
	void Process();
	
	/***********************
	* Render: Render the Cloth the to the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();
	
	/***********************
	* AddForce: Add a force to the entire cloth
	* @author: Callan Moore
	* @parameter: _force: Force vector of the force to apply (Direction and magnitude)
	* @return: void
	********************/
	void AddForce(v3float _force);
	
	/***********************
	* WindForce: Add a Wind force to the entire cloth
	* @author: Callan Moore
	* @parameter: _force: Force vector of the force to apply (Direction and magnitude)
	* @return: void
	********************/
	void WindForce(v3float _force);
	
	/***********************
	* ReleaseCloth: Release all the pins that holds the cloth in place
	* @author: Callan Moore
	* @return: void
	********************/
	void ReleaseCloth();
	
	/***********************
	* MovePinned: Move the pinned particles closer or further away from the centre
	* @author: Callan Moore
	* @parameter: _closer: true to move closer. False to move particles away
	* @return: void
	********************/
	void MovePinned(bool _closer);
	
	/***********************
	* PinParticle: Pin the input particle
	* @author: Callan Moore
	* @parameter: _particle: Pointer to the particle to pin
	* @return: void
	********************/
	void PinParticle(Physics_Particle* _particle);
	
	/***********************
	* ResetCloth: Reset the cloth to the default positions and states
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool ResetCloth();

private:
	
	/***********************
	* GetParticle: Retrieve the particle at the input row and column index
	* @author: Callan Moore
	* @parameter: _x: Column index
	* @parameter: _y: Row index
	* @return: Physics_Particle*: Pointer to the returned particle
	********************/
	Physics_Particle* GetParticle(int _x, int _y) { return &m_pParticles[_y * m_numParticlesWidth + _x]; };
	
	/***********************
	* MakeConstraint: Create a constraint between two particles
	* @author: Callan Moore
	* @parameter: _pParticleA: First particle to constrain
	* @parameter: _pParticleB: Second particle to constrain
	* @parameter: _restDist: The resting distance to constrain the particles to (Default to the initial distance between the input particles)
	* @return: bool: Successful or not
	********************/
	bool MakeConstraint(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, float _restDist = -1.0f);
	
	/***********************
	* CalcTriangleNormal: Calculate the normal of the triangle defined by the three input particles
	* @author: Callan Moore
	* @parameter: _pParticleA: First particle of the triangle
	* @parameter: _pParticleB: Second particle of the triangle
	* @parameter: _pParticleC: Third particle of the triangle
	* @return: v3float: The calculated normal to the triangle
	********************/
	v3float CalcTriangleNormal(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC);
	
	/***********************
	* AddWindForceForTri: Add wind force to the triangle defined by the three input particles
	* @author: Callan Moore
	* @parameter: _pParticleA: First particle of the triangle
	* @parameter: _pParticleB: Second particle of the triangle
	* @parameter: _pParticleC: Third particle of the triangle
	* @parameter: _force: Force vector of the force to apply (Direction and magnitude)
	* @return: void
	********************/
	void AddWindForceForTri(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC, v3float _force);

private:
	DX10_Renderer* m_pRenderer;
	DX10_Mesh* m_pMesh;
	DX10_Shader_LitTex* m_pShader;

	bool m_initialisedParticles;

	int m_width;
	int m_height;
	int m_numParticlesWidth;
	int m_numParticlesHeight;
	int m_particleCount;
	int m_constraintIterations;

	float m_damping;
	float m_timeStep;

	D3DXMATRIX m_matWorld;
	ID3D10ShaderResourceView* m_pTex;

	Physics_Particle* m_pParticles;
	std::vector<Physics_Constraint> m_contraints;
	std::vector<Physics_Particle*> m_pinnedParticles;

	
	

	



};
#endif	// __PHYSICS_CLOTH_H__
