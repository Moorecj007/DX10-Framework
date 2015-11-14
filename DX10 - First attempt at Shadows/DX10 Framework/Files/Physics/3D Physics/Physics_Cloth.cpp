/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Cloth.cpp
* Description : Simulates a cloth using Physics
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Cloth.h"

Physics_Cloth::Physics_Cloth()
{
}

Physics_Cloth::~Physics_Cloth()
{
	// Release allocated memory
	ReleasePtr(m_pMesh);
	ReleasePtrArray(m_pParticles);
}

bool Physics_Cloth::Initialise(DX10_Renderer* _pRenderer, DX10_Shader_Cloth* _pShader, int _width, int _height, float _damping, float _timeStep)
{
	if (_pRenderer == 0 || _pShader == 0)
	{
		// Pointer is NULL, initialization failed
		return false;
	}

	// Assign member variables
	BaseInitialise();
	m_pRenderer = _pRenderer;
	m_pShader = _pShader;
	m_width = _width;
	m_height = _height;
	m_particlesWidthCount = m_width + 1;
	m_particlesHeightCount = m_height + 1;
	m_damping = _damping;
	m_timeStep = _timeStep;

	m_minWidth = m_minHeight = 5;
	m_maxWidth = m_maxHeight = 35;

	m_hooks = 3;
	m_minHooks = 2;
	m_maxHooks = m_maxWidth;

	m_selfCollisionRad = 0.85f;

	m_constraintIterations = 3;
	m_breakModifier = 2.4f;
	m_windSpeed = 1.0f;
	m_initialisedParticles = false;
	m_burnTime = 1.5f;
	m_complexWeave = true;

	m_maxBlastRadius = 25.0f;
	m_blastRadius = 10.0f;

	// Set the cloth to initial positions and constraints
	VALIDATE(ResetCloth());

	return true;
}

void Physics_Cloth::Process(eCollisionType _collisionType)
{	
	CalcWorldMatrix();

	// Adding Gravity
	AddForce({ 0.0f, -9.81f, 0.0f }, FT_GENERIC, false);

	TVertexColor* pVertexBuffer = m_pMesh->GetVertexBufferCloth();
	DWORD* pIndices = m_pMesh->GetIndexBuffer();

	// Process each Particle
	for (int i = 0; i < m_particleCount; i++)
	{
		m_pParticles[i].Process();
	}

	// Calculate each constraint multiple times
	for (int i = 0; i < m_constraintIterations; i++)
	{
		for (int j = 0; j < (int)m_contraints.size(); j++)
		{
			if (m_contraints[j].SatisfyConstraint() == false)
			{
				// Constraint is broken. Stop drawing the line
				pIndices[(j * 2) + 1] = pIndices[j * 2] = 0;
			}
		}
	
		// Calculate the collisions with object, if any
		switch (_collisionType)
		{
			case CT_SPHERE:
			{
				SphereCollision({ 0.0f, 0.0f, 7.0f }, 5.0f);
			}
			break;
			case CT_CAPSULE:
			{
				CapsuleCollision({ 0.0f, -3.0f, 6.0f }, { 0.0f, 3.0f, 6.0f }, 3.0f);
			}
			break;
			case CT_PYRAMID:
			{
				// Hard coded points for the pyramid to use
				v3float _pyraPointA = { 0.0f, 0.408248f * 10.0f, 0.0f + 7.0f };
				v3float _pyraPointB = { 0.5f * 10.0f, -0.408248f * 10.0f, -0.288675f * 10.0f + 7.0f };
				v3float _pyraPointC = { 0.0f, -0.408248f * 10.0f, 0.577350f * 10.0f + 7.0f };
				v3float _pyraPointD = { -0.5f * 10.0f, -0.408248f * 10.0f, -0.288675f * 10.0f + 7.0f };

				PyramidCollision(_pyraPointA, _pyraPointB, _pyraPointC, _pyraPointD);
			}
			default: break;
		}

		// Calculate the permanent collisions
		FloorCollision(-20.0f);
		CollisionsWithSelf();
	}

	// Cycle through all constraints and check their burning status
	for (int j = 0; j < (int)m_contraints.size(); j++)
	{
		Physics_Particle* pIgnitedParticle = 0;
		switch (m_contraints[j].BurnDown(m_timeStep, pIgnitedParticle))
		{
		case IA_IGNITEPARTICLE:
		{
			// The constraint burnt long enough to ignite the particle on the other end
			IgniteConnectedConstraints(pIgnitedParticle);
		}
		break;
		case IA_DESTROYCONSTRAINT:
		{
			// The constraint burnt long enough to be destroyed
			pIndices[(j * 2) + 1] = pIndices[j * 2] = 0;
		}
		break;
		case IA_NOACTION: // Fall Through
		default: break;
		}	// End Switch
	}

	// Update the vertex for each Particle
	for (int i = 0; i < m_particleCount; i++)
	{
		pVertexBuffer[i].pos.x = m_pParticles[i].GetPosition()->x;
		pVertexBuffer[i].pos.y = m_pParticles[i].GetPosition()->y;
		pVertexBuffer[i].pos.z = m_pParticles[i].GetPosition()->z;
	}

	// Update the Buffer
	m_pMesh->UpdateBufferCloth();
}

void Physics_Cloth::Render()
{
	// Create the struct to hold the cloth rendering variables
	TCloth cloth;
	cloth.pMatWorld = &m_matWorld;
	cloth.pMesh = m_pMesh;

	// Render the cloth
	m_pShader->Render(cloth);
}

void Physics_Cloth::AddForce(v3float _force, eForceType _forceType, bool _selected)
{
	switch (_forceType)
	{
	case FT_GENERIC: // Adds the same generic force to each particle
	{
		for (int i = 0; i < m_particleCount; i++)
		{
			if (m_pParticles[i].GetSelectedState() == _selected)
			{
				m_pParticles[i].AddForce(_force);
			}
		}
	}
	break;
	case FT_WIND: // Add a wind force that bases the strength of the force on the normal of triangles
	{
		// Multiply the force direction by the current wind speed
		_force = _force.Normalise() * m_windSpeed;

		// Cycle through the particles based on the rows and columns (Due to using a triangle strip topology)
		for (int x = 0; x < m_particlesWidthCount - 1; x++)
		{
			for (int y = 0; y < m_particlesHeightCount - 1; y++)
			{
				// Add wind force to both triangles in the current grid selection
				AddWindForceForTri(GetParticle(x + 1, y + 1), GetParticle(x, y + 1), GetParticle(x, y), _force);
				AddWindForceForTri(GetParticle(x, y), GetParticle(x + 1, y), GetParticle(x + 1, y + 1), _force);
			}
		}
	}
	break;
	default:break;
	}
}

void Physics_Cloth::ReleaseCloth()
{
	// Cycle through and release all pinned particles
	std::vector<Physics_Particle*>::iterator pinnedIter;
	for (pinnedIter = m_hookedParticles.begin(); pinnedIter != m_hookedParticles.end(); pinnedIter++)
	{
		(*pinnedIter)->SetStaticState(false);

		if ((*pinnedIter)->GetIgnitedState() == true)
		{
			m_pVertices[(*pinnedIter)->GetParticleID()].color = d3dxColors::Red;
		}
		else
		{
			m_pVertices[(*pinnedIter)->GetParticleID()].color = d3dxColors::White;
		}
	}

	// Clear the entire list of pinned particles
	m_hookedParticles.clear();
}

void Physics_Cloth::MoveHooks(bool _closer)
{
	// Check that there are hooked particles
	if (m_hookedParticles.empty() == false)
	{
		for (int i = 0; i < (int)m_hookedParticles.size(); i++)
		{
			// Allow movement on the particle
			m_hookedParticles[i]->SetStaticState(false);

			// Calculate the offset from the centre
			float xOffset = 0.0f - m_hookedParticles[i]->GetPosition()->x;
			xOffset /= 100.0f;
			v3float dir = { xOffset, 0.0f, 0.0f };

			// Flip the direction to move hooks further away
			if (_closer == false)
			{
				dir = -dir;
			}

			// Move the hook
			m_hookedParticles[i]->Move(dir);

			// Set the Particle back to static
			m_hookedParticles[i]->SetStaticState(true);
		}
	}
}

void Physics_Cloth::HookParticle(Physics_Particle* _particle)
{
	_particle->SetStaticState(true);
	m_hookedParticles.push_back(_particle);
}

bool Physics_Cloth::ResetCloth()
{
	// Clear Memory
	ReleaseCloth();
	ReleaseSelected();
	m_contraints.clear();
	m_nextIndex = 0;

	if (m_initialisedParticles == false)
	{
		ReleasePtr(m_pMesh);
		ReleasePtrArray(m_pParticles);

		// Create memory for all the particles
		m_particleCount = m_particlesWidthCount * m_particlesHeightCount;
		m_pParticles = new Physics_Particle[m_particleCount];
		m_pVertices = new TVertexColor[m_particleCount];		

		// Calculate how many indices there are with be based on how many particles there are using line list
		int immediateConstraintCount = (m_particlesWidthCount - 1) * (m_particlesHeightCount - 1) * 4 + (m_particlesWidthCount - 1) + (m_particlesHeightCount - 1);
		int secondaryConstraintCount = 0;
		if (m_complexWeave == true)
		{
			// Calculate the secondary indices count only if the weave is set to complex
			secondaryConstraintCount = (m_particlesWidthCount - 2) * (m_particlesHeightCount - 2) * 4 + ((m_particlesWidthCount - 2) * 2) + ((m_particlesHeightCount - 2) * 2);
		}
		
		// Create the indices buffer with the amount of calculated constraints
		m_indexCount = (immediateConstraintCount + secondaryConstraintCount) * 2;
		m_pIndices = new DWORD[m_indexCount];
	}
	
	// Cycle through all the particles
	for (int col = 0; col < m_particlesWidthCount; col++)
	{
		for (int row = 0; row < m_particlesHeightCount; row++)
		{
			// Calculate the position based on the particles row and column
			v3float pos;
			pos.x = m_width * (col / (float)m_width) - ((float)m_width / 2.0f);
			pos.y = -m_height * (row / (float)m_height) + ((float)m_height / 2.0f);
			pos.z = 0.0f;
			int index = row * m_particlesWidthCount + col;

			if (m_initialisedParticles == false)
			{
				// First time. Initialise			
				m_pVertices[index] = { { pos.x, pos.y, pos.z }, d3dxColors::White };
				VALIDATE(m_pParticles[index].Initialise(index, &m_pVertices[index], pos, m_timeStep, m_damping));
			}
			else
			{
				// Particle has already been initialized so just reset the position
				m_pParticles[index].Reset();
				m_pParticles[index].SetPosition(pos, true);
				m_pVertices[index].color = d3dxColors::White;
			}
		}
	}

	// Connect Particles that are immediately to the right and below (include diagonals)
	for (int col = 0; col < m_particlesWidthCount; col++)
	{
		for (int row = 0; row < m_particlesHeightCount; row++)
		{
			// Particle to the Right exists
			if (col < m_particlesWidthCount - 1)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 1, row), true));
				
				// Add the constraint index to each attached particle
				GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col + 1, row)->AddContraintIndex(m_contraints.size() - 1);
			}
	
			// Particle below exists
			if (row < m_particlesHeightCount - 1)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col, row + 1), true));

				// Add the constraint index to each attached particle
				GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col, row + 1)->AddContraintIndex(m_contraints.size() - 1);
			}
	
			// Particle to the right and below exists
			if ((col < m_particlesWidthCount - 1) && (row < m_particlesHeightCount - 1))
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 1, row + 1), true));

				// Add the constraint index to each attached particle
				GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col + 1, row + 1)->AddContraintIndex(m_contraints.size() - 1);

				VALIDATE(MakeConstraint(GetParticleIndex(col + 1, row), GetParticleIndex(col, row + 1), true));

				// Add the constraint index to each attached particle
				GetParticle(col + 1, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col, row + 1)->AddContraintIndex(m_contraints.size() - 1);
			}
		}
	}

	if (m_complexWeave == true)
	{
		// Connect Particles the are one step further away than previous loop
		for (int col = 0; col < m_particlesWidthCount; col++)
		{
			for (int row = 0; row < m_particlesHeightCount; row++)
			{
				// Particle to the Right exists
				if (col < m_particlesWidthCount - 2)
				{
					VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 2, row), false));

					// Add the constraint index to each attached particle
					GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col + 2, row)->AddContraintIndex(m_contraints.size() - 1);
				}

				// Particle below exists
				if (row < m_particlesHeightCount - 2)
				{
					VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col, row + 2), false));

					// Add the constraint index to each attached particle
					GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col, row + 2)->AddContraintIndex(m_contraints.size() - 1);
				}

				// Particle to the right and below exists
				if ((col < m_particlesWidthCount - 2) && (row < m_particlesHeightCount - 2))
				{
					VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 2, row + 2), false));

					// Add the constraint index to each attached particle
					GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col + 2, row + 2)->AddContraintIndex(m_contraints.size() - 1);

					VALIDATE(MakeConstraint(GetParticleIndex(col + 2, row), GetParticleIndex(col, row + 2), false));

					// Add the constraint index to each attached particle
					GetParticle(col + 2, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col, row + 2)->AddContraintIndex(m_contraints.size() - 1);
				}
			}
		}
	}
	
	if (m_initialisedParticles == false)
	{
		// Create a new Cloth Mesh
		m_pMesh = new DX10_Mesh();
		VALIDATE(m_pMesh->InitialiseCloth(m_pRenderer, m_pVertices, m_pIndices, m_particleCount, m_indexCount, sizeof(TVertexColor), D3D10_PRIMITIVE_TOPOLOGY_LINELIST, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DYNAMIC));
	}

	// Create the hooks and pin the cloth
	CreateHooks();

	// Add a wind force of 1 down the Z axis to settle the cloth
	AddForce({ 0.0f, 0.0f, 1.0f }, FT_GENERIC, false);
	Process(CT_NONE);

	m_initialisedParticles = true;
	return true;
}

void Physics_Cloth::ResizeWidth(float _ratio)
{
	m_width = (int)round(_ratio * (m_maxWidth - m_minWidth)) + m_minWidth;

	if (m_width % 2 == 0)
	{
		// Cloth is even width, want odd
		m_width++;
	}

	// Update Particle Width Count based on new width
	m_particlesWidthCount = m_width + 1;

	// Set the initialized to false to recreate the cloth again
	m_initialisedParticles = false;
	ResetCloth();	// reset the cloth to see changes
}

void Physics_Cloth::ResizeHeight(float _ratio)
{
	m_height = (int)round(_ratio * (m_maxHeight - m_minHeight)) + m_minHeight;

	if (m_height % 2 == 0)
	{
		// Cloth is even height, want odd
		m_height++;
	}

	// Update Particle Height Count based on new height
	m_particlesHeightCount = m_height + 1;

	// Set the initialized to false to recreate the cloth again
	m_initialisedParticles = false;
	ResetCloth();	// reset the cloth to see changes
}

void Physics_Cloth::ResizeHooks(float _ratio)
{
	m_hooks = (int)round(_ratio * (m_particlesWidthCount - m_minHooks)) + m_minHooks;


	m_hooks++;
	ResetCloth();	// reset the cloth to see changes
}

void Physics_Cloth::CreateHooks()
{
	// Move the particles closer together make it look more like a curtain
	for (int i = 0; i < (m_particlesWidthCount - 1) / 2; i++)
	{
		// Determine the ratio of movement
		float ratio = 1.0f - ((float)i / ((float)(m_particlesWidthCount - 1) / 2.0f));
		GetParticle(i, 0)->Move({ ratio, 0.0f, 0.0f });
		GetParticle((m_particlesWidthCount - i - 1), 0)->Move({ -ratio, 0.0f, 0.0f });
	}

	// Calculate the exact distance apart the hooks should be
	float hookDist = (float)m_width / (float)(m_hooks - 1);

	int increment = 0;
	for (int i = 0; i < m_hooks; i++)
	{
		// Calculate a rounded distance so the hook points line up with particle points
		int roundedDist = (int)round(increment * hookDist);

		if (i % 2 == 0) // Hook particle on the left
		{
			HookParticle(GetParticle(roundedDist, 0));
		}
		else // Hook particle on the right
		{
			HookParticle(GetParticle((m_width - roundedDist), 0));

			// Increment the distance every odd/second hook
			increment++;
		}
	}
}

void Physics_Cloth::FloorCollision(float _floorPos)
{
	v3float up = { 0.0f, 1.0f, 0.0f };

	// Cycle through all particles
	for (int i = 0; i < m_particleCount; i++)
	{
		// Check if the particles position is less than the floors position
		if (m_pParticles[i].GetPosition()->y <= _floorPos + 0.1f)
		{
			// Push the particles up if they are under the floor
			float line = abs(m_pParticles[i].GetPosition()->y - (_floorPos));
			m_pParticles[i].SetPosition(*m_pParticles[i].GetPosition() + (up * line), true);
		}
	}
}

void Physics_Cloth::SphereCollision(v3float _center, float _sphereRadius)
{
	// Cycle through all the particles
	for (int i = 0; i < m_particleCount; i++)
	{
		// Calculate the line between the particle and the sphere
		v3float line = *m_pParticles[i].GetPosition() - _center;
		float distanceApart = line.Magnitude();

		// Check if the distance apart is less than the spheres radius
		if (distanceApart < _sphereRadius)
		{
			// The particle is in the sphere. Push it out using the shortest path possible
			m_pParticles[i].Move(line.Normalise() * (_sphereRadius - distanceApart));
		}
	}
}

void  Physics_Cloth::CapsuleCollision(v3float _sphereCentre1, v3float _sphereCentre2, float _capsuleRadius)
{
	// Cycle through all particles
	for (int i = 0; i < m_particleCount; i++)
	{
		v3float particlePos = (*m_pParticles[i].GetPosition());

		// Calculate the line within the capsule (sphere centre to sphere centre)
		v3float capsuleLine = _sphereCentre2 - _sphereCentre1;
		float capsuleLineLengthSq = pow(capsuleLine.Magnitude(), 2.0f);

		// Calculate the line between the capsule and particle and project that onto the capsule line
		v3float capsuleToParticle = particlePos - _sphereCentre1;
		float dot = capsuleToParticle.Dot(capsuleLine);

		// Calculate the ratio down the length of the capsule line
		float ratioOnCapuleLine = dot / capsuleLineLengthSq;

		// Clamp the ratio to 0-1 range
		if (ratioOnCapuleLine < 0.0f)
		{
			ratioOnCapuleLine = 0.0f;
		}
		else if (ratioOnCapuleLine > 1.0f)
		{
			ratioOnCapuleLine = 1.0f;
		}

		// Calculate the closest point on the capsule line to the particle
		v3float closestCapsulePoint = (_sphereCentre1 + (capsuleLine *ratioOnCapuleLine));

		// Calculate a sphere collision to determine if the point is inside the capsule
		SphereCollision(closestCapsulePoint, _capsuleRadius);
	}
}

void Physics_Cloth::PyramidCollision(v3float _pyraPointA, v3float _pyraPointB, v3float _pyraPointC, v3float _pyraPointD)
{
	// Cycle through all particles
	for (int i = 0; i < m_particleCount; i++)
	{
		v3float particlePos = *m_pParticles[i].GetPosition();

		// Starts the closest point as the position of the particle and the closest distance to infinity
		v3float closestPt = particlePos;
		float closestDist = FLT_MAX;

		// Declare extra variables
		v3float closestTriPoint;
		float dist;

		// Calculate the closest point on the first triangle and compare
		closestTriPoint = ClosestPointOnTriangle(particlePos, _pyraPointA, _pyraPointB, _pyraPointC);
		dist = pow((particlePos - closestTriPoint).Magnitude(), 2);
		if (dist < closestDist)
		{
			// Save the new closest point and distance
			closestDist = dist;
			closestPt = closestTriPoint;
		}

		// Calculate the closest point on the second triangle and compare
		closestTriPoint = ClosestPointOnTriangle(particlePos, _pyraPointA, _pyraPointC, _pyraPointD);
		dist = pow((particlePos - closestTriPoint).Magnitude(), 2);
		if (dist < closestDist)
		{
			// Save the new closest point and distance
			closestDist = dist;
			closestPt = closestTriPoint;
		}

		// Calculate the closest point on the third triangle and compare
		closestTriPoint = ClosestPointOnTriangle(particlePos, _pyraPointA, _pyraPointD, _pyraPointB);
		dist = pow((particlePos - closestTriPoint).Magnitude(), 2);
		if (dist < closestDist)
		{
			// Save the new closest point and distance
			closestDist = dist;
			closestPt = closestTriPoint;
		}

		// Calculate the closest point on the fourth triangle and compare
		closestTriPoint = ClosestPointOnTriangle(particlePos, _pyraPointB, _pyraPointD, _pyraPointC);
		dist = pow((particlePos - closestTriPoint).Magnitude(), 2);
		if (dist < closestDist)
		{
			// Save the new closest point and distance
			closestDist = dist;
			closestPt = closestTriPoint;
		}

		// If the point is outside all planes then the point is within the bounds of the pyramid
		if (	(PointOutsideOfPlane(particlePos, _pyraPointA, _pyraPointB, _pyraPointC) == true)
			&&	(PointOutsideOfPlane(particlePos, _pyraPointA, _pyraPointC, _pyraPointD) == true)
			&&	(PointOutsideOfPlane(particlePos, _pyraPointA, _pyraPointD, _pyraPointB) == true)
			&&	(PointOutsideOfPlane(particlePos, _pyraPointB, _pyraPointD, _pyraPointC) == true))
		{
			// Move the particle outside the pyramid using the closest point 
			closestPt = closestPt + ((closestPt - particlePos).Normalise() * 0.5f);
			m_pParticles[i].SetPosition(closestPt, true);
		}
		else
		{
			// Particle is not inside the pyramid
			v3float diff = particlePos - closestPt;
			dist = diff.Magnitude();
			
			// check if the particle is too close to the pyramid
			if (dist < 0.5f)
			{
				// Move the particle a small distance from the pyramid
				closestPt = closestPt + ((particlePos - closestPt).Normalise() * 0.5f);
				m_pParticles[i].SetPosition(closestPt, true);
			}
		}
	}
}

void Physics_Cloth::UpdateWindSpeed(float _ratio)
{
	// Update the Wind Speed
	m_windSpeed = _ratio * 40.0f;
}

void Physics_Cloth::Ignite(TCameraRay _camRay, float _selectRadius)
{
	// Select the particles that intersect with the ray
	SelectParticles(_camRay, _selectRadius);

	// Cycle through all the currently selected particles
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		// Ignite all the connected constraints each selected particle
		IgniteConnectedConstraints(m_selectedParticles[i]);
	}
}

void Physics_Cloth::Cut(TCameraRay _camRay, float _selectRadius)
{
	// Select the particles that intersect with the ray
	SelectParticles(_camRay, _selectRadius);

	// Cycle through all the currently selected particles
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		// Set the position extremely far away to simulate the cutting of the cloth (The particle breaks all constraints)
		m_selectedParticles[i]->SetPosition({ 1000000.0f, 1000000.0f, 10000000.0f }, true);
	}
}

void Physics_Cloth::Manipulate(TCameraRay _camRay, float _selectRadius, bool _firstCast)
{
	// Check if this is the first cast
	if (_firstCast == true)
	{
		// Select the particles that intersect with the ray
		SelectParticles(_camRay, _selectRadius);
	}

	// Cycle through all the currently selected particles
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		// Calculate the direction and distance of the ray origin to the particle
		v3float rayOriginToParticle = (*(m_selectedParticles[i]->GetPosition()) - _camRay.Origin);
		
		// ensure the direction of the camRay is normalised
		_camRay.Direction.Normalise();

		// Project the particle onto the rays direction
		float distanceDownRay = _camRay.Direction.Dot(rayOriginToParticle);

		v3float newPos = _camRay.Origin + (_camRay.Direction * distanceDownRay);
		m_selectedParticles[i]->SetPosition(newPos, false);
	}
}

void Physics_Cloth::SelectParticles(TCameraRay _camRay, float _selectRadius)
{
	// Release all selected particles before selecting new ones
	ReleaseSelected();

	// Cycle through all particles
	for (int i = 0; i < m_particleCount; i++)
	{
		 // Calculate the world matrix for the current particle
		D3DXMATRIX particleWorldMatrix;
		D3DXMatrixTranslation(&particleWorldMatrix, m_pParticles[i].GetPosition()->x, m_pParticles[i].GetPosition()->y, m_pParticles[i].GetPosition()->z);
		D3DXMatrixMultiply(&particleWorldMatrix, &m_matWorld, &particleWorldMatrix);

		// Calculate the inverse of the particles world matrix
		D3DXMATRIX invWorldMatrix;
		D3DXMatrixInverse(&invWorldMatrix, NULL, &particleWorldMatrix);

		// Convert the camRay variables into D3DXVECTOR3
		D3DXVECTOR3 rayOrigin = { _camRay.Origin.x, _camRay.Origin.y, _camRay.Origin.z };
		D3DXVECTOR3 rayDirection = { _camRay.Direction.x, _camRay.Direction.y, _camRay.Direction.z };

		// Transform the ray by the inverse world matrix
		D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &invWorldMatrix);
		D3DXVec3TransformNormal(&rayDirection, &rayDirection, &invWorldMatrix);

		// Convert the D3DXVECTOR3 back into a camera ray struct
		TCameraRay TransformedRay({ rayOrigin.x, rayOrigin.y, rayOrigin.z }, { rayDirection.x, rayDirection.y, rayDirection.z });
		TransformedRay.Direction.Normalise();

		// Test particle with the transformed ray
		bool intersect = TransformedRay.RaySphereIntersect(_selectRadius);

		// Check if the particle intersected
		if (intersect == true)
		{
			// Set the select state for the particle to true and push it onto the vector
			m_pParticles[i].SetSelectedState(true);
			m_selectedParticles.push_back(&m_pParticles[i]);
		}

		//// Calculate the world matrix for the current particle
		//D3DXMATRIX translationMatrix;
		//D3DXMATRIX transWorldMatrix;
		//D3DXMatrixTranslation(&translationMatrix, m_pParticles[i].GetPosition()->x, m_pParticles[i].GetPosition()->y, m_pParticles[i].GetPosition()->z);
		//D3DXMatrixMultiply(&transWorldMatrix, &m_matWorld, &translationMatrix);
		//
		//// Get the Inverse of the Translated World Matrix
		//D3DXMATRIX invWorldMatrix;
		//D3DXMatrixInverse(&invWorldMatrix, NULL, &transWorldMatrix);
		//
		//// Transform the Ray Origin and the Ray Direction from View Space to World Space
		//D3DXVECTOR3 Origin = { _camRay.Origin.x, _camRay.Origin.y, _camRay.Origin.z };
		//D3DXVECTOR3 Direction = { _camRay.Direction.x, _camRay.Direction.y, _camRay.Direction.z };
		//D3DXVECTOR3  tmpRayDirection, tmpRayOrigin;
		//D3DXVec3TransformCoord(&tmpRayOrigin, &Origin, &invWorldMatrix);
		//D3DXVec3TransformNormal(&tmpRayDirection, &Direction, &invWorldMatrix);
		//TCameraRay TransformedRay({ tmpRayOrigin.x, tmpRayOrigin.y, tmpRayOrigin.z }, { tmpRayDirection.x, tmpRayDirection.y, tmpRayDirection.z });
		//TransformedRay.Direction.Normalise();
		//
		//// Test each 
		//bool intersect = TransformedRay.RaySphereIntersect(_selectRadius);
		//
		//// Check if the particle intersected
		//if (intersect == true)
		//{
		//	// Set the select state for the particle to true and push it onto the vector
		//	m_pParticles[i].SetSelectedState(true);
		//	m_selectedParticles.push_back(&m_pParticles[i]);
		//}
	}
}

void Physics_Cloth::ReleaseSelected()
{
	// Cycle through all currently selected particles
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		// Set particle select state to false
		m_selectedParticles[i]->SetSelectedState(false);
	}

	// Clear all members out of the vector
	m_selectedParticles.clear();
}

// Private Functions

bool Physics_Cloth::MakeConstraint(int _particleIndexA, int _particleIndexB, bool _immediate)
{
	// Create and initialize a new constraint
	m_contraints.push_back(Physics_Constraint());
	VALIDATE(m_contraints.back().Initialise(&m_pParticles[_particleIndexA], &m_pParticles[_particleIndexB], _immediate, m_breakModifier));

	// Add indices to the index array to draw them
	m_pIndices[m_nextIndex++] = _particleIndexA;
	m_pIndices[m_nextIndex++] = _particleIndexB;
	return true;
}

v3float Physics_Cloth::CalcTriangleNormal(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC)
{
	// Retrieve the positions of the 3 particles
	v3float posA = *_pParticleA->GetPosition();
	v3float posB = *_pParticleB->GetPosition();
	v3float posC = *_pParticleC->GetPosition();

	// Calculate two direction vector with focus on the first particle (A)
	v3float vecA = posB - posA;
	v3float vecB = posC - posA;

	// Calculate the cross product to get the normal to the triangle
	return vecA.Cross(vecB);
}

void Physics_Cloth::AddWindForceForTri(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC, v3float _force)
{
	// Calculate the 3 lines and lengths of the triangle
	v3float vecA = (*_pParticleB->GetPosition() - *_pParticleA->GetPosition());
	v3float vecB = (*_pParticleC->GetPosition() - *_pParticleA->GetPosition());
	v3float vecC = (*_pParticleB->GetPosition() - *_pParticleC->GetPosition());
	float magA = vecA.Magnitude();
	float magB = vecB.Magnitude();
	float magC = vecC.Magnitude();

	// Particle A and C always make up the hypotenuse so multiple by sqroot 2;
	if (magA > m_breakModifier || magB > m_breakModifier * sqrt(2.0f) || magC > m_breakModifier)
	{
		return;
	}

	// Calculate the normal to the triangle
	v3float normal = vecA.Cross(vecB);
	v3float normalised = normal;
	normalised.Normalise();

	// Calculate the force the wind can apply to the triangle
	v3float force = normal * (normalised.Dot(_force));

	// Add the calculated force to all particles on the triangle
	_pParticleA->AddForce(force);
	_pParticleB->AddForce(force);
	_pParticleC->AddForce(force);
}

void Physics_Cloth::IgniteConnectedConstraints(Physics_Particle* _pParticle)
{
	// Ensure the particle pointer is not NULL
	if (_pParticle != 0)
	{
		// Ensure the particle has not already been ignited
		if (_pParticle->GetIgnitedState() == false)
		{
			// Calculate a variable burn time for each particle individually
			float modifier = 1 + (float)((rand() % 80) - 40) / 100.0f;
			float modifiedBurnTime = m_burnTime * modifier;

			// Ignite the Particle
			_pParticle->Ignite(modifiedBurnTime);

			// Cycle through all connected constraints
			std::vector<UINT> connectedConstraints = _pParticle->GetContraintIndices();
			for (UINT i = 0; i < connectedConstraints.size(); i++)
			{
				// Ensure the constraint can be ignited
				if (m_contraints[connectedConstraints[i]].CanBeIgnited() == true)
				{
					// Ignite the constraint with the the burn time of the particle
					m_contraints[connectedConstraints[i]].Ignite(modifiedBurnTime);
				}
			}
		}
	}
}

void Physics_Cloth::CollisionsWithSelf()
{
	// Cycle through all particles
	for (int i = 0; i < m_particleCount; i++)
	{
		if (m_pParticles[i].GetActiveState() == true)
		{
			// Cycle through all particles that are after the i'th particle
			for (int j = i + 1; j < m_particleCount; j++)
			{
				if (m_pParticles[j].GetActiveState() == true)
				{
					// Calculate the line between the particles
					v3float line = *m_pParticles[j].GetPosition() - *m_pParticles[i].GetPosition();
					float distanceApart = line.Magnitude();

					// Check if the distance apart is less than the self collision radius
					if (distanceApart < m_selfCollisionRad)
					{
						// Push the Particles apart with equal force in opposite directions
						m_pParticles[j].Move((line.Normalise() * (m_selfCollisionRad - distanceApart)) / 2.0f);
						m_pParticles[i].Move((line.Normalise() * (m_selfCollisionRad - distanceApart)) / -2.0f);

						if (distanceApart < 0.6f)
						{
							if (m_pParticles[j].GetIgnitedState() == true)
							{
								IgniteConnectedConstraints(&m_pParticles[i]);
							}

							if (m_pParticles[i].GetIgnitedState() == true)
							{
								IgniteConnectedConstraints(&m_pParticles[j]);
							}
						}
					}
				}
			}
		}
	}
}

void Physics_Cloth::Explode(float _ratio)
{
	v3float center = { 0, 0, 0 };
	m_blastRadius = m_maxBlastRadius * _ratio;

	// Cycle through all the particles
	for (int i = 0; i < m_particleCount; i++)
	{
		// Calculate the line between the particle and the sphere
		v3float line = *m_pParticles[i].GetPosition() - center;
		float distanceApart = line.Magnitude();

		// Check if the distance apart is less than the spheres radius
		if (distanceApart < m_blastRadius)
		{
			// The particle is in the sphere. Push it out using the shortest path possible
			m_pParticles[i].Move(line.Normalise() * (m_blastRadius - distanceApart));

			IgniteConnectedConstraints(&m_pParticles[i]);
		}
	}
}

