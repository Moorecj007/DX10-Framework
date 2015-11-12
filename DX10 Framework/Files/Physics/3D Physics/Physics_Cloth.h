/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Cloth.h
* Description : Simulates a cloth using Physics
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_CLOTH_H__
#define __PHYSICS_CLOTH_H__

// Local Includes
#include "../../DX10/DX10.h"
#include "Physics_Constraint.h"

// Enumerators
/***********************
* eForceType: Enum for the different types of Force that can be enacted upon the Cloth
* @author: Callan Moore
********************/
enum eForceType
{
	FT_GENERIC,
	FT_WIND
};

/***********************
* eCollisionType: Enum for the different types of collisions the Cloth can be subject to
* @author: Callan Moore
********************/
enum eCollisionType
{
	CT_NONE,
	CT_SPHERE,
	CT_CAPSULE,
	CT_PYRAMID
};

class Physics_Cloth
	: public DX10_Obj_Generic
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
	* @parameter: DX10_Shader_Cloth: The shader that will be used to render the object
	* @parameter: _width: The width of the Cloth
	* @parameter: _height: The height of the Cloth
	* @parameter: _damping: The damping to apply to the cloth
	* @parameter: _timeStep: The time step for the physics calculations
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Shader_Cloth* _pShader, int _width, int _height, float _damping = 0.01f, float _timeStep = 0.016667f);
	
	/***********************
	* Process: Process the Cloth for the time step
	* @author: Callan Moore
	* @parameter: _collisionType: The type of collision to use during this process
	* @return: void
	********************/
	void Process(eCollisionType _collisionType);
	
	/***********************
	* Render: Render the Cloth the to the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();
	
	/***********************
	* AddForce: Add a force to the entire cloth
	* @author: Callan Moore
	* @parameter: _force: Force vector of the force to add (Direction and magnitude)
	* @parameter: _forceType: The type of force to add
	* @parameter: _selected: Whether to only affect currently selected particles
	* @return: void
	********************/
	void AddForce(v3float _force, eForceType _forceType, bool _selected);
	
	/***********************
	* ReleaseCloth: Release all the pins that holds the cloth in place
	* @author: Callan Moore
	* @return: void
	********************/
	void ReleaseCloth();
	
	/***********************
	* MoveHooks: Move the hooked particles closer or further away from the center
	* @author: Callan Moore
	* @parameter: _closer: true to move closer. False to move particles away
	* @return: void
	********************/
	void MoveHooks(bool _closer);
	
	/***********************
	* HookParticle: Hook the input particle
	* @author: Callan Moore
	* @parameter: _particle: Pointer to the particle to hook
	* @return: void
	********************/
	void HookParticle(Physics_Particle* _particle);
	
	/***********************
	* ResetCloth: Reset the cloth to the default positions and states
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool ResetCloth();
	
	/***********************
	* ResizeWidth: Resize the width of the cloth
	* @author: Callan Moore
	* @parameter: _ratio: The new ratio of the Width size
	* @return: void
	********************/
	void ResizeWidth(float _ratio);

	/***********************
	* ResizeHeight: Resize the height of the cloth
	* @author: Callan Moore
	* @parameter: _ratio: The new ratio of the height size
	* @return: void
	********************/
	void ResizeHeight(float _ratio);
	
	/***********************
	* ResizeHooks: Resize the amount of hooks used by the cloth
	* @author: Callan Moore
	* @parameter: _ratio: The new ratio of the hook amount
	* @return: void
	********************/
	void ResizeHooks(float _ratio);
	
	/***********************
	* CreateHooks: Create and attach the number of hooks
	* @author: Callan Moore
	* @return: void
	********************/
	void CreateHooks();

	/***********************
	* UpdateWindSpeed: Add the speed change to the current wind speed
	* @author: Callan Moore
	* @parameter: float _speed: The ratio of max wind speed
	* @return: void
	********************/
	void UpdateWindSpeed(float _ratio);
	
	/***********************
	* ReleaseSelected: Set all the selected particles to unselected
	* @author: Callan Moore
	* @return: void
	********************/
	void ReleaseSelected();
	
	/***********************
	* Ignite: Ignites all particles that are in the ray
	* @author: Callan Moore
	* @parameter: _camRay: The current camera ray cast at the mouse point
	* @parameter: _selectRadius: The radius of the particles to see if they intersect the ray
	* @return: void
	********************/
	void Ignite(TCameraRay _camRay, float _selectRadius);
	
	/***********************
	* Cut: Cut all particles that are in the ray
	* @author: Callan Moore
	* @parameter: _camRay: The current camera ray cast at the mouse point
	* @parameter: _selectRadius: The radius of the particles to see if they intersect the ray
	* @return: void
	********************/
	void Cut(TCameraRay _camRay, float _selectRadius);
	
	/***********************
	* Manipulate: Manipulate the cloths selected particles to follow the mouse
	* @author: Callan Moore
	* @parameter: _camRay: The current camera ray cast at the mouse point
	* @parameter: _selectRadius: The radius of the particles to see if they intersect the ray
	* @parameter: _firstCast: Whether the first cast has been done. First cast selects the particles to manipulate
	* @return: void
	********************/
	void Manipulate(TCameraRay _camRay, float _selectRadius, bool _firstCast);
	
	/***********************
	* ToggleWeave: Toggle the weave of the cloth between simple and complex (double stitching)
	* @author: Callan Moore
	* @return: void
	********************/
	void ToggleWeave() { m_complexWeave = !m_complexWeave; m_initialisedParticles = false; ResetCloth(); };
		
	/***********************
	* Explode: Explode the centre of the cloth
	* @author: Callan Moore
	* @parameter: _ratio: Ratio to determine the blast radius
	* @return: void
	********************/
	void Explode(float _ratio);

private:
	
	/***********************
	* GetParticle: Retrieve the particle at the input row and column index
	* @author: Callan Moore
	* @parameter: _x: Column index
	* @parameter: _y: Row index
	* @return: Physics_Particle*: Pointer to the returned particle
	********************/
	Physics_Particle* GetParticle(int _x, int _y) { return &m_pParticles[_y * m_particlesWidthCount + _x]; };
	
	/***********************
	* GetParticleIndex: Calculate the index of a particle based on the row and column
	* @author: Callan Moore
	* @parameter: _x: X Position (Column)
	* @parameter: _y: Y Position (Row)
	* @return: int: Particle Index
	********************/
	int GetParticleIndex(int _x, int _y) { return (_y * m_particlesWidthCount + _x); };
	
	/***********************
	* MakeConstraint: Create a constraint between two particles
	* @author: Callan Moore
	* @parameter: _particleIndexA: Index of the First particle to constrain
	* @parameter: _particleIndexB: Index of the Second particle to constrain
	* @parameter: _immediate: Whether the constraint is an immediate neighbor
	* @return: bool: Successful or not
	********************/
	bool MakeConstraint(int _particleIndexA, int _particleIndexB, bool _immediate);
	
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

	/***********************
	* SelectParticles: Select particles that intersect with the ray
	* @author: Callan Moore
	* @parameter: _camRay: The current camera ray cast at the mouse point
	* @parameter: _selectRadius: The radius of the particles to see if they intersect the ray
	* @return: void
	********************/
	void SelectParticles(TCameraRay _camRay, float _selectRadius);
	
	/***********************
	* IgniteConnectedConstraints: Burn all constraints connected to the input particle
	* @author: Callan Moore
	* @parameter: _pParticle: The particle that is connected to the constraints that need to ignite
	* @return: void
	********************/
	void IgniteConnectedConstraints(Physics_Particle* _pParticle);
	
	/***********************
	* CollisionsWithSelf: Calculate the collisions with itself so that it does not penetrate or clip
	* @author: Callan Moore
	* @return: void
	********************/
	void CollisionsWithSelf();
	
	/***********************
	* FloorCollision: Calculate collisions with a floor (plane)
	* @author: Callan Moore
	* @parameter: float _floorPos: The Y position of the floor
	* @return: void
	********************/
	void FloorCollision(float _floorPos);
	
	/***********************
	* SphereCollision: Calculate collisions with a sphere
	* @author: Callan Moore
	* @parameter: _center:The spheres centre position
	* @parameter: _sphereRadius: The radius of the sphere
	* @return: void
	********************/
	void SphereCollision(v3float _center, float _sphereRadius);
	
	/***********************
	* CapsuleCollision: Calculate collisions with a capsule
	* @author: Callan Moore
	* @parameter: _sphereCentre1: The first sphere of the capsules centre position
	* @parameter: _sphereCentre2: The second sphere of the capsules centre position
	* @parameter: float _capsuleRadius: The radius of the capsule
	* @return: void
	********************/
	void CapsuleCollision(v3float _sphereCentre1, v3float _sphereCentre2, float _capsuleRadius);
	
	/***********************
	* PyramidCollision: Calculate collisions with a pyramid
	* @author: Callan Moore
	* @parameter: v3float _pyraPointA: First point of the pyramid
	* @parameter: v3float _pyraPointB: Second point of the pyramid
	* @parameter: v3float _pyraPointC: Third point of the pyramid
	* @parameter: v3float _pyraPointD: Fourth point of the pyramid
	* @return: void
	********************/
	void PyramidCollision(v3float _pyraPointA, v3float _pyraPointB, v3float _pyraPointC, v3float _pyraPointD);
	
	
private:
	DX10_Shader_Cloth* m_pShader;

	bool m_initialisedParticles;

	int m_width;
	int m_height;
	int m_maxWidth;
	int m_minWidth;
	int m_maxHeight;
	int m_minHeight;
	int m_particlesWidthCount;
	int m_particlesHeightCount;
	int m_hooks;
	int m_minHooks;
	int m_maxHooks;	
	float m_selfCollisionRad;

	int m_particleCount;
	int m_constraintIterations;

	float m_damping;
	float m_timeStep;
	float m_breakModifier;
	float m_windSpeed;

	Physics_Particle* m_pParticles;
	std::vector<Physics_Constraint> m_contraints;
	std::vector<Physics_Particle*> m_hookedParticles;

	TVertexColor* m_pVertices;
	DWORD* m_pIndices;
	int m_indexCount;
	int m_nextIndex;

	std::vector<Physics_Particle*> m_selectedParticles;

	// Ignition Variables
	float m_burnTime;
	bool m_complexWeave;

	float m_maxBlastRadius;
	float m_blastRadius;
};
#endif	// __PHYSICS_CLOTH_H__
