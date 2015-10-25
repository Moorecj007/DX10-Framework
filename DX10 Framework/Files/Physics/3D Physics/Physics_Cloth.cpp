/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Cloth.cpp
* Description : TO DO CAL
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
	m_pRenderer = _pRenderer;
	m_pShader = _pShader;
	m_width = _width;
	m_height = _height;
	m_numParticlesWidth = m_width + 1;
	m_numParticlesHeight = m_height + 1;
	m_damping = _damping;
	m_timeStep = _timeStep;

	m_constraintIterations = 2;
	m_initialisedParticles = false;

	// Set the world matrix
	D3DXMatrixIdentity(&m_matWorld);

	// Set the cloth to initial positions and constraints
	VALIDATE(ResetCloth());

	return true;
}

void Physics_Cloth::Process()
{
	// Adding Gravity
	AddForce({ 0.0f, -9.81f, 0.0f });

	DWORD* pIndices = m_pMesh->GetIndexBuffer();

	// Calculate each constraint multiple times
	for (int i = 0; i < m_constraintIterations; i++)
	{
		for (int j = 0; j < (int)m_contraints.size(); j++)
		{
			if (m_contraints[j].SatisfyConstraint())
			{
				pIndices[(j * 2)+1] = m_pIndices[j*2] = 0;
			}
		}
	}
	
	TVertexColor* pVertexBuffer = m_pMesh->GetVertexBufferCloth();
	// Process each Particle
	for (int i = 0; i < m_particleCount; i++)
	{
		m_pParticles[i].Process();
		pVertexBuffer[i].pos.x = m_pParticles[i].GetPosition()->x;
		pVertexBuffer[i].pos.y = m_pParticles[i].GetPosition()->y;
		pVertexBuffer[i].pos.z = m_pParticles[i].GetPosition()->z;
	}

	// Update the Buffer
	m_pMesh->UpdateBufferCloth();
}

void Physics_Cloth::Render()
{
	TCloth cloth;
	cloth.pMatWorld = &m_matWorld;
	cloth.pMesh = m_pMesh;

	m_pShader->Render(cloth);
}

void Physics_Cloth::AddForce(v3float _force)
{
	// Cycle through all the particles
	for (int i = 0; i < m_particleCount; i++)
	{
		// Add force to each particle
		m_pParticles[i].AddForce(_force);
	}
}

void Physics_Cloth::WindForce(v3float _force)
{
	// Cycle through the particles based on the rows and columns (Due to using a triangle strip topology)
	for (int col = 0; col < m_numParticlesWidth - 1; col++)
	{
		for (int row = 0; row < m_numParticlesHeight- 1; row++)
		{
			if (row % 2 == 0)	// Even Row for triangle strip
			{
				// Add wind force to the triangles
				AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row + 1), GetParticle(col, row + 1), _force);
				AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row), GetParticle(col + 1, row + 1), _force);
			}
			else // Odd row for triangle strip
			{
				// Add wind force to the triangles
				AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row), GetParticle(col, row + 1), _force);
				AddWindForceForTri(GetParticle(col + 1, row), GetParticle(col + 1, row + 1), GetParticle(col, row + 1), _force);
			}			
		}
	}
}

void Physics_Cloth::ReleaseCloth()
{
	// Cycle through and release all pinned particles
	std::vector<Physics_Particle*>::iterator pinnedIter;
	for (pinnedIter = m_pinnedParticles.begin(); pinnedIter != m_pinnedParticles.end(); pinnedIter++)
	{
		(*pinnedIter)->SetStaticState(false);
	}

	// Clear the entire list of pinned particles
	m_pinnedParticles.clear();
}

void Physics_Cloth::MovePinned(bool _closer)
{
	if (m_pinnedParticles.size() > 1)
	{
		// Find the Central pin
		int centrePinIndex = (m_pinnedParticles.size() - 1) / 2;
		Physics_Particle* centrePin = m_pinnedParticles[centrePinIndex];

		// Cycle through all pins and move them closer to the centre pin
		for (int i = 0; i < (int)m_pinnedParticles.size(); i++)
		{
			if (i != centrePinIndex)
			{
				// Calculate the direction of movement towards the centre
				v3float dir = *m_pinnedParticles[centrePinIndex]->GetPosition() - *m_pinnedParticles[i]->GetPosition();
				dir.Normalise();
				dir = dir / 30.0f;

				// Reverse the direction if the user wants further away
				if (_closer == false)
				{
					dir *= -1.0f;
				}

				// calculate the ratio of movement based upon how far away the pinned point is from the centre
				float ratio = abs((float)(i - centrePinIndex) / (float)centrePinIndex);

				// Allow movement and move the pinned particle then return to static state
				m_pinnedParticles[i]->SetStaticState(false);
				m_pinnedParticles[i]->Move(dir * ratio);

				float length = (*m_pinnedParticles[centrePinIndex]->GetPosition() - *m_pinnedParticles[i]->GetPosition()).Magnitude();
				if (length < 1.0f)
				{
					// Return to previous position if moving the particle brought them too close
					m_pinnedParticles[i]->Move(dir * -ratio);
				}

				// Set the static state back to true once movement is complete
				m_pinnedParticles[i]->SetStaticState(true);
			}
		}
	}
}

void Physics_Cloth::PinParticle(Physics_Particle* _particle)
{
	_particle->SetStaticState(true);
	m_pinnedParticles.push_back(_particle);
}

bool Physics_Cloth::ResetCloth()
{
	// Clear Memory
	ReleaseCloth();
	m_contraints.clear();

	//if (m_initialisedParticles == true)
	//{
		ReleasePtr(m_pMesh);
		ReleasePtrArray(m_pParticles);

		// Create memory for all the particles
		m_nextIndex = 0;
		m_particleCount = m_numParticlesWidth * m_numParticlesHeight;
		m_pParticles = new Physics_Particle[m_particleCount];
		m_pVertices = new TVertexColor[m_particleCount];

		// Calculate how many indices there with be based on how many particles there are using line list
		int immediateConstraintCount = (m_numParticlesWidth - 1) * (m_numParticlesHeight - 1) * 4 + (m_numParticlesWidth - 1) + (m_numParticlesHeight - 1);
		int secondaryConstraintCount = (m_numParticlesWidth - 2) * (m_numParticlesHeight - 2) * 4 + ((m_numParticlesWidth - 2) * 2) + ((m_numParticlesHeight - 2) * 2);
		m_indexCount = (immediateConstraintCount + secondaryConstraintCount) * 2;

		m_pIndices = new DWORD[m_indexCount];
	//}

	// Cycle through all the particles
	for (int col = 0; col < m_numParticlesWidth; col++)
	{
		for (int row = 0; row < m_numParticlesHeight; row++)
		{
			// Calculate the position based on the particles row and column
			v3float pos = { m_width * (col / (float)m_numParticlesWidth), -m_height * (row / (float)m_numParticlesHeight), 0.0f };
			int index = row * m_numParticlesWidth + col;

			if (m_initialisedParticles == false)
			{
				// First time. Initialise
				VALIDATE(m_pParticles[index].Initialise(pos, m_timeStep, m_damping));
				m_pVertices[index] = { { pos.x, pos.y, pos.z }, d3dxColors::Black };
			}
			else
			{
				// Particle has already been initialized so just reset the position
				m_pParticles[index].SetPosition(pos);
			}
		}
	}
	m_initialisedParticles = true;

	// Connect Particles that are immediately to the right and below (include diagonals)
	for (int col = 0; col < m_numParticlesWidth; col++)
	{
		for (int row = 0; row < m_numParticlesHeight; row++)
		{
			// Particle to the Right exists
			if (col < m_numParticlesWidth - 1)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 1, row)));
			}

			// Particle below exists
			if (row < m_numParticlesHeight - 1)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col, row + 1)));
			}

			// Particle to the right and below exists
			if ((col < m_numParticlesWidth - 1) && (row < m_numParticlesHeight - 1))
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 1, row + 1)));
				VALIDATE(MakeConstraint(GetParticleIndex(col + 1, row), GetParticleIndex(col, row + 1)));
			}
		}
	}

	// Connect Particles the are one step further away than previous loop
	for (int col = 0; col < m_numParticlesWidth; col++)
	{
		for (int row = 0; row < m_numParticlesHeight; row++)
		{
			// Particle to the Right exists
			if (col < m_numParticlesWidth - 2)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 2, row)));
			}

			// Particle below exists
			if (row < m_numParticlesHeight - 2)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col, row + 2)));
			}

			// Particle to the right and below exists
			if ((col < m_numParticlesWidth - 2) && (row < m_numParticlesHeight - 2))
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 2, row + 2)));
				VALIDATE(MakeConstraint(GetParticleIndex(col + 2, row), GetParticleIndex(col, row + 2)));
			}
		}
	}
	
	// Create a new Cloth Mesh
	m_pMesh = new DX10_Mesh();
	VALIDATE(m_pMesh->InitialiseCloth(m_pRenderer, m_pVertices, m_pIndices, m_particleCount, m_indexCount, sizeof(TVertexColor), D3D10_PRIMITIVE_TOPOLOGY_LINELIST, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DYNAMIC));

	// Pin Particles
	GetParticle(0, 0)->Move({ 1.0f, 0.0f, 0.0f });
	GetParticle((m_numParticlesWidth - 1), 0)->Move({ -1.0f, 0.0f, 0.0f });

	int pinMid = ((m_numParticlesWidth - 1) / 2);
	int pinQuarter = pinMid / 2;

	PinParticle(GetParticle(0, 0));
	PinParticle(GetParticle(pinQuarter, 0));
	PinParticle(GetParticle(pinMid, 0));
	PinParticle(GetParticle(pinMid + pinQuarter, 0));
	PinParticle(GetParticle((m_numParticlesWidth - 1), 0));

	WindForce({ 0.0f, 0.0f, 0.001f });
	Process();
	return true;
}

void Physics_Cloth::ResizeWidth(bool _smaller)
{
	if (_smaller == true)
	{
		if (m_width >= 8)
		{
			m_width -= 2;
			m_numParticlesWidth = m_width + 1;
		}
	}
	else
	{
		if (m_width <= 48)
		{
			m_width += 2;
			m_numParticlesWidth = m_width + 1;
		}
	}
	m_initialisedParticles = false;

	ResetCloth();
}

void Physics_Cloth::ResizeHeight(bool _smaller)
{
	if (_smaller == true)
	{
		if (m_height >= 8)
		{
			m_height -= 2;
			m_numParticlesHeight = m_height + 1;
		}
	}
	else
	{
		if (m_height <= 48)
		{
			m_height += 2;
			m_numParticlesHeight = m_height + 1;
		}
	}
	m_initialisedParticles = false;

	ResetCloth();
}

void Physics_Cloth::FloorCollision(float _floorPos)
{
	v3float up = { 0.0f, 1.0f, 0.0f };

	for (int i = 0; i < m_particleCount; i++)
	{
			if (m_pParticles[i].GetPosition()->y <= _floorPos)
		{
			// Push the particles up if they are under the floor
			float pierce = abs(m_pParticles[i].GetPosition()->y - (_floorPos));
			m_pParticles[i].Move(up*pierce);
		}
	}
}

void Physics_Cloth::BallCollision(v3float _center, float _radius)
{
	//std::vector<Phy_Cloth_Particle>::iterator particleIter;
	//for (particleIter = m_particleList.begin(); particleIter != m_particleList.end(); particleIter++)
	//{
	//	v3float Pierce = (*particleIter).GetPosition() - _Center;
	//	float PierceLength = Pierce.Magnitude();
	//	if (PierceLength < _Radius) // if the particle is inside the ball
	//	{
	//		(*particleIter).OffSetPosition(Pierce.Normalise() * (_Radius - PierceLength)); // project the particle to the surface of the ball
	//	}
	//}
}

bool Physics_Cloth::MakeConstraint(int _particleIndexA, int _particleIndexB, bool _draw, float _restDist)
{
	// Create and initialize a new constraint
	m_contraints.push_back(Physics_Constraint());
	VALIDATE(m_contraints.back().Initialise(&m_pParticles[_particleIndexA], &m_pParticles[_particleIndexB], _restDist));

	if (_draw == true)
	{
		m_pIndices[m_nextIndex++] = _particleIndexA;
		m_pIndices[m_nextIndex++] = _particleIndexB;
	}

	return true;
}

v3float Physics_Cloth::CalcTriangleNormal(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC)
{
	// retrieve the positions of the 3 particles
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
	// Calculate the normal to the triangle
	v3float normal = CalcTriangleNormal(_pParticleA, _pParticleB, _pParticleC);
	v3float normalised = normal;
	normalised.Normalise();

	// Calculate the force the wind can apply to the triangle
	v3float force = normal * (normalised.Dot(_force));

	// Add the calculated force to all particles on the triangle
	_pParticleA->AddForce(force);
	_pParticleB->AddForce(force);
	_pParticleC->AddForce(force);
}