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

bool Physics_Cloth::Initialise(DX10_Renderer* _pRenderer, DX10_Shader_LitTex* _pShader, float _width, float _height, int _numParticlesWidth, int _numParticlesHeight)
{
	if (_pRenderer == 0 || _pShader == 0)
	{
		// Pointer is NULL, initialization failed
		return false;
	}

	// TO DO
	m_damping = 0.01f;
	m_timeStep = 0.01666666666f;
	m_constraintIterations = 2;
	m_initialisedParticles = false;

	// Assign member variables
	m_pRenderer = _pRenderer;
	m_pShader = _pShader;
	m_width = _width;
	m_height = _height;
	m_numParticlesWidth = _numParticlesWidth;
	m_numParticlesHeight = _numParticlesHeight;

	// Set a texture and a world matrix
	VALIDATE(m_pRenderer->CreateTexture("Dragon.png", m_pTex));
	D3DXMatrixIdentity(&m_matWorld);

	// Create space for all the particles
	m_particleCount = m_numParticlesWidth * m_numParticlesHeight;
	m_pParticles = new Physics_Particle[m_particleCount];

	m_pMesh = new DX10_Mesh();
	VALIDATE(m_pMesh->InitialisePlane(m_pRenderer, m_numParticlesWidth, { (m_width / (m_numParticlesWidth - 1.0f)), 1.0f, (m_height / (m_numParticlesHeight - 1.0f)) }));

	VALIDATE(ResetCloth());

	return true;
}

void Physics_Cloth::Process()
{
	// Adding Gravity
	AddForce({ 0.0f, -9.81f, 0.0f });

	// Calculate each constraint multiple times
	for (int i = 0; i < m_constraintIterations; i++)
	{
		for (int i = 0; i < (int)m_contraints.size(); i++)
		{
			m_contraints[i].SatisfyConstraint();
		}
	}
	
	TVertexNormalUV* pVertexBuffer = m_pMesh->GetVertexBuffer();
	int index = 0;
	// Process each Particle
	for (int i = 0; i < m_particleCount; i++)
	{
		m_pParticles[i].Process();
		pVertexBuffer[index].pos.x = m_pParticles[i].GetPosition()->x;
		pVertexBuffer[index].pos.y = m_pParticles[i].GetPosition()->y;
		pVertexBuffer[index].pos.z = m_pParticles[i].GetPosition()->z;
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

	m_pShader->Render(litTex, TECH_LITTEX_NOCULL);
}

void Physics_Cloth::AddForce(v3float _forceDir)
{
	std::vector<Physics_Particle>::iterator particleIter;
	// Add force to each particle
	for (int i = 0; i < m_particleCount; i++)
	{
		m_pParticles[i].AddForce(_forceDir);
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

void Physics_Cloth::ReleaseCloth()
{
	std::vector<Physics_Particle*>::iterator pinnedIter;
	// Cycle through and release all pinned particles
	for (pinnedIter = m_pinnedParticles.begin(); pinnedIter != m_pinnedParticles.end(); pinnedIter++)
	{
		(*pinnedIter)->StaticState(false);
	}

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
				m_pinnedParticles[i]->StaticState(false);
				m_pinnedParticles[i]->Move(dir * ratio);

				float length = (*m_pinnedParticles[centrePinIndex]->GetPosition() - *m_pinnedParticles[i]->GetPosition()).Magnitude();
				if (length < 1.0f)
				{
					// Return to previous position if moving the particle brought them too close
					m_pinnedParticles[i]->Move(dir * -ratio);
				}

				m_pinnedParticles[i]->StaticState(true);
			}
		}
	}
}

void Physics_Cloth::PinParticle(Physics_Particle* _particle)
{
	_particle->StaticState(true);
	m_pinnedParticles.push_back(_particle);
}

bool Physics_Cloth::ResetCloth()
{
	// Remove all the current pins
	ReleaseCloth();

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
			}
			else
			{
				// Particle has already been initialized so just reset the position
				m_pParticles[index].SetPosition(pos);
			}
		}
	}
	m_initialisedParticles = true;

	// Clear the constraints list to make way for new ones
	m_contraints.clear();

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