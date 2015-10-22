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

	// TO DO CAL
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Shader_LitTex* _pShader, float _width, float _height, int _numParticlesWidth, int _numParticlesHeight);
	void Process();
	void Render();
	void AddForce(v3float _forceDir);
	void WindForce(v3float _forceDir);
	void ReleaseCloth();
	void MovePinned(bool _closer);
	void PinParticle(Physics_Particle* _particle);
	bool ResetCloth();

private:
	// TO DO CAL
	Physics_Particle* GetParticle(int _x, int _y) { return &m_pParticles[_y * m_numParticlesWidth + _x]; };
	bool MakeConstraint(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, float _restDist = -1.0f);

	v3float CalcTriangleNormal(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC);
	void AddWindForceForTri(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC, v3float _forceDir);

private:
	// TO DO CAL
	// Maybe change to particles per unit width/height. allows easy scaling
	// Also have a standard with/height of the cloth

	int m_numParticlesWidth;
	int m_numParticlesHeight;
	float m_width;
	float m_height;

	DX10_Renderer* m_pRenderer;
	DX10_Mesh* m_pMesh;
	DX10_Shader_LitTex* m_pShader;

	D3DXMATRIX m_matWorld;
	ID3D10ShaderResourceView* m_pTex;

	bool m_initialisedParticles;
	Physics_Particle* m_pParticles;
	int m_particleCount;
	std::vector<Physics_Constraint> m_contraints;
	std::vector<Physics_Particle*> m_pinnedParticles;

	int m_constraintIterations;
	float m_damping;
	float m_timeStep;

	



};
#endif	// __PHYSICS_CLOTH_H__
