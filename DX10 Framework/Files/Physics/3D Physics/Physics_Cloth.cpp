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
}

bool Physics_Cloth::Initialise(DX10_Renderer* _pRenderer, DX10_Shader_LitTex* _pShader, float _width, float _height, int _numParticlesWidth, int _numParticlesHeight)
{
	if (_pRenderer == 0 || _pShader == 0)
	{
		// Pointer is NULL, initialization failed
		return false;
	}

	// TO DO
	float damping = 0.01f;
	float timestep = 0.5f;
	m_constraintIterations = 15;

	// Assign member variables
	m_pRenderer = _pRenderer;
	m_pShader = _pShader;
	m_width = _width;
	m_height = _height;
	m_numParticlesWidth = _numParticlesWidth;
	m_numParticlesHeight = _numParticlesHeight;

	VALIDATE(m_pRenderer->CreateTexture("WaterMap.png", m_pTex));
	D3DXMatrixIdentity(&m_matWorld);

	// Create space for all the particles
	m_particles.resize(m_numParticlesWidth * m_numParticlesHeight);

	// Generate a Particle grid that starts has the top left corner at origin
	for (int col = 0; col < m_numParticlesWidth; col++)
	{
		for (int row = 0; row < m_numParticlesHeight; row++)
		{
			v3float pos = { m_width * (col / (float)m_numParticlesWidth), -m_height * (row / (float)m_numParticlesHeight), 0.0f };
			
			int index = row * m_numParticlesWidth + col;
			m_particles[index] = Physics_Particle();
			VALIDATE(m_particles[index].Initialise(pos, timestep, damping));
		}
	}

	// Connect Particles that are immediately to the right and below (include diagonals)
	for (int col = 0; col < m_numParticlesWidth; col++)
	{
		for (int row = 0; row < m_numParticlesHeight; row++)
		{
			// Particle to the Right exists
			if (col < m_numParticlesWidth - 1)
			{
				VALIDATE(MakeConstraint(GetParticle(col, row), GetParticle(col + 1, row)));
			}

			// Particle below exists
			if (row < m_numParticlesHeight - 1)
			{
				VALIDATE(MakeConstraint(GetParticle(col, row), GetParticle(col, row + 1)));
			}

			// Particle to the right and below exists
			if ((col < m_numParticlesWidth - 1) && (row < m_numParticlesHeight - 1))
			{
				VALIDATE(MakeConstraint(GetParticle(col, row), GetParticle(col + 1, row + 1)));
				VALIDATE((GetParticle(col + 1, row), GetParticle(col, row + 1)));
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
				VALIDATE(MakeConstraint(GetParticle(col, row), GetParticle(col + 2, row)));
			}

			// Particle below exists
			if (row < m_numParticlesHeight - 2)
			{
				VALIDATE(MakeConstraint(GetParticle(col, row), GetParticle(col, row + 2)));
			}

			// Particle to the right and below exists
			if ((col < m_numParticlesWidth - 2) && (row < m_numParticlesHeight - 2))
			{
				VALIDATE(MakeConstraint(GetParticle(col, row), GetParticle(col + 2, row + 2)));
				VALIDATE(MakeConstraint(GetParticle(col + 2, row), GetParticle(col, row + 2)));
			}
		}
	}

	// TO DO CAL - create a new function to InitialiseSquarePlane() and InitialisePlane()
	m_pMesh = new DX10_Mesh();
	VALIDATE(m_pMesh->InitialisePlane(m_pRenderer, m_numParticlesWidth, { (m_width / (m_numParticlesWidth - 1.0f)), 1.0f, (m_height / (m_numParticlesHeight- 1.0f)) }));

	// TO DO CAL - change to hang on a rod
	// Pin the three most left and three most right (and top) particles
	for (int i = 0; i < 3; i++)
	{
		GetParticle(i, 0)->Move({ 0.5f, 0.0f, 0.0f });
		GetParticle(i, 0)->StaticState(true);

		GetParticle((m_numParticlesWidth - 1 - i), 0)->Move({ -0.5f, 0.0f, 0.0f });
		GetParticle((m_numParticlesWidth - 1 - i), 0)->StaticState(true);
	}
	return true;
}

void Physics_Cloth::Process()
{
	std::vector<Physics_Constraint>::iterator constraintIter;
	// Calculate each constraint multiple times
	for (int i = 0; i < m_constraintIterations; i++)
	{
		for (constraintIter = m_contraints.begin(); constraintIter != m_contraints.end(); constraintIter++)
		{
			(*constraintIter).SatisfyConstraint();
		}
	}
	
	TVertexNormalUV* pVertexBuffer = m_pMesh->GetVertexBuffer();
	int index = 0;
	std::vector<Physics_Particle>::iterator particleIter;
	// Process each Particle
	for (particleIter = m_particles.begin(); particleIter != m_particles.end(); particleIter++)
	{
		(*particleIter).Process();
		pVertexBuffer[index].pos.x = (*particleIter).GetPosition()->x;
		pVertexBuffer[index].pos.y = (*particleIter).GetPosition()->y;
		pVertexBuffer[index].pos.z = (*particleIter).GetPosition()->z;
		index++;
	}

	m_pMesh->CalculateNormals();
}

void Physics_Cloth::Render()
{
	TLitTex litTex;
	litTex.pMatWorld = &m_matWorld;
	litTex.pMesh = m_pMesh;
	litTex.pTexBase = m_pTex;

	m_pShader->Render(litTex);
}

void Physics_Cloth::AddForce(v3float _forceDir)
{
	std::vector<Physics_Particle>::iterator particleIter;
	// Add force to each particle
	for (particleIter = m_particles.begin(); particleIter != m_particles.end(); particleIter++)
	{
		(*particleIter).AddForce(_forceDir);
	}
}

void Physics_Cloth::WindForce(v3float _forceDir)
{
	for (int col = 0; col < m_numParticlesWidth - 1; col++)
	{
		for (int row = 0; row < m_numParticlesHeight- 1; row++)
		{
			if (row % 2 == 0)	// Even Row for triangle strip
			{
				AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row + 1), GetParticle(col, row + 1), _forceDir);
				AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row), GetParticle(col + 1, row + 1), _forceDir);
			}
			else // Odd row for triangle strip
			{
				AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row), GetParticle(col, row + 1), _forceDir);
				AddWindForceForTri(GetParticle(col + 1, row), GetParticle(col + 1, row + 1), GetParticle(col, row + 1), _forceDir);
			}			
		}
	}
}

bool Physics_Cloth::MakeConstraint(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, float _restDist)
{
	m_contraints.push_back(Physics_Constraint());
	VALIDATE(m_contraints.back().Initialise(_pParticleA, _pParticleB, _restDist));

	return true;
}

v3float Physics_Cloth::CalcTriangleNormal(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC)
{
	v3float posA = *_pParticleA->GetPosition();
	v3float posB = *_pParticleB->GetPosition();
	v3float posC = *_pParticleC->GetPosition();

	v3float vecA = posB - posA;
	v3float vecB = posC - posA;

	return vecA.Cross(vecB);
}

void Physics_Cloth::AddWindForceForTri(Physics_Particle* _pParticleA, Physics_Particle* _pParticleB, Physics_Particle* _pParticleC, v3float _forceDir)
{
	v3float normal = CalcTriangleNormal(_pParticleA, _pParticleB, _pParticleC);
	v3float normalised = normal;
	normalised.Normalise();

	v3float force = normal * (normalised.Dot(_forceDir));

	_pParticleA->AddForce(force);
	_pParticleB->AddForce(force);
	_pParticleC->AddForce(force);
}