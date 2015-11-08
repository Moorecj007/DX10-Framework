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

	m_constraintIterations = 3;
	m_breakModifier = 2.0f;
	m_windSpeed = 1.0f;
	m_initialisedParticles = false;
	m_burnTime = 3.0f;
	m_complexWeave = false;

	// Set the cloth to initial positions and constraints
	VALIDATE(ResetCloth());

	return true;
}

void Physics_Cloth::Process(eCollisionType _collisionType)
{	

	CalcWorldMatrix();
	// Adding Gravity
	AddForce({ 0.0f, -9.81f, 0.0f }, FT_UNIVERSAL, false);

	TVertexColor* pVertexBuffer = m_pMesh->GetVertexBufferCloth();
	DWORD* pIndices = m_pMesh->GetIndexBuffer();

	// Process each Particle
	// FOR JC
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
	
		// FOR JC
		switch (_collisionType)
		{
			case CT_SPHERE:
			{
				SphereCollision({ 0.0f, 0.0f, 7.0f }, 5.0f);
			}
			break;
			case CT_CAPSULE:
			{
				CapsuleCollision({ 0.0f, -2.0f, 4.0f }, { 0.0f, 2.0f, 4.0f }, 2.0f);
			}
			break;
			case CT_PYRAMID:
			{
				PyramidCollision();
			}
			default: break;
		}

		FloorCollision(-15.0f);
		SelfCollisions();
	}

	// FOR JC
	//SelfCollisions();

	//if (m_collide_Sphere)
	//{
	//	m_pObj_Sphere->Process(_dt);
	//	m_pCloth->SphereCollision(m_pObj_Sphere->GetPosition(), 5.0f + 0.2f);
	//}
	//if (m_collide_Capsule)
	//{
	//	m_pObj_Capsule->Process(_dt);
	//	m_pCloth->CapsuleCollision();
	//}
	//
	//if (m_collide_Pyramid)
	//{
	//	m_pObj_Pyramid->Process(_dt);
	//	m_pCloth->PyramidCollision();
	//}
	//
	//m_pCloth->FloorCollision(m_pObj_Floor->GetPosition().y);



	// FOR JC
	for (int j = 0; j < (int)m_contraints.size(); j++)
	{
		Physics_Particle* pIgnitedParticle = 0;
		switch (m_contraints[j].BurnDown(m_timeStep, pIgnitedParticle))
		{
		case CB_IGNITEOTHERS:
		{
			BurnConnectedConstraints(pIgnitedParticle);
		}
		break;
		case CB_DESTROYED:
		{
			pIndices[(j * 2) + 1] = pIndices[j * 2] = 0;
		}
		break;
		case CB_NOACTION: // Fall Through
		default: break;
		}	// End Switch
	}

	//SelfCollisions();

	// Process each Particle
	for (int i = 0; i < m_particleCount; i++)
	{

		//m_pParticles[i].Process();
		
		pVertexBuffer[i].pos.x = m_pParticles[i].GetPosition()->x;
		pVertexBuffer[i].pos.y = m_pParticles[i].GetPosition()->y;
		pVertexBuffer[i].pos.z = m_pParticles[i].GetPosition()->z;
	}



	//

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

void Physics_Cloth::AddForce(v3float _force, eForceType _forceType, bool _selected)
{
	switch (_forceType)
	{
	case FT_UNIVERSAL: // Adding a Universal Force - The force on each particle is universal (apply the same force to each particle)
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
	case FT_WIND: // Adding a Wind Force - The force on each particle is Dependant on in normal
	{
		// Normalise and Multiply the wind force
		_force = _force.Normalise() * m_windSpeed;
		for (int x = 0; x < m_particlesWidthCount - 1; x++)
		{
			for (int y = 0; y < m_particlesHeightCount - 1; y++)
			{
				AddWindForceForTri(GetParticle(x + 1, y + 1), GetParticle(x, y + 1), GetParticle(x, y), _force);
				AddWindForceForTri(GetParticle(x, y), GetParticle(x + 1, y), GetParticle(x + 1, y + 1), _force);
			}
		}
	}
	break;
	default:break;
	}
}

void Physics_Cloth::AddWindForce(v3float _force)
{
	_force = _force.Normalise() * m_windSpeed;

	// Cycle through the particles based on the rows and columns (Due to using a triangle strip topology)
	for (int col = 0; col < m_particlesWidthCount - 1; col++)
	{
		for (int row = 0; row < m_particlesHeightCount- 1; row++)
		{
			// Add wind force to the triangles
			AddWindForceForTri(GetParticle(col + 1, row + 1), GetParticle(col, row + 1), GetParticle(col, row), _force);
			AddWindForceForTri(GetParticle(col, row), GetParticle(col + 1, row), GetParticle(col + 1, row + 1), _force);	
		}
	}
}

void Physics_Cloth::ReleaseCloth()
{
	// Cycle through and release all pinned particles
	std::vector<Physics_Particle*>::iterator pinnedIter;
	for (pinnedIter = m_hookedParticles.begin(); pinnedIter != m_hookedParticles.end(); pinnedIter++)
	{
		(*pinnedIter)->SetStaticState(false);
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
			secondaryConstraintCount = (m_particlesWidthCount - 2) * (m_particlesHeightCount - 2) * 4 + ((m_particlesWidthCount - 2) * 2) + ((m_particlesHeightCount - 2) * 2);
		}
		
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
				m_pVertices[index] = { { pos.x, pos.y, pos.z }, d3dxColors::Black };
				(m_pParticles[index].Initialise(index, &m_pVertices[index], pos, m_timeStep, m_damping));
			}
			else
			{
				// Particle has already been initialized so just reset the position
				m_pParticles[index].Reset();
				m_pParticles[index].SetPosition(pos);
				m_pVertices[index] = { { pos.x, pos.y, pos.z }, d3dxColors::Black };
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
				GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col + 1, row)->AddContraintIndex(m_contraints.size() - 1);
			}
	
			// Particle below exists
			if (row < m_particlesHeightCount - 1)
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col, row + 1), true));
				GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col, row + 1)->AddContraintIndex(m_contraints.size() - 1);
			}
	
			// Particle to the right and below exists
			if ((col < m_particlesWidthCount - 1) && (row < m_particlesHeightCount - 1))
			{
				VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 1, row + 1), true));
				GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
				GetParticle(col + 1, row + 1)->AddContraintIndex(m_contraints.size() - 1);
				VALIDATE(MakeConstraint(GetParticleIndex(col + 1, row), GetParticleIndex(col, row + 1), true));
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
					GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col + 2, row)->AddContraintIndex(m_contraints.size() - 1);
				}

				// Particle below exists
				if (row < m_particlesHeightCount - 2)
				{
					VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col, row + 2), false));
					GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col, row + 2)->AddContraintIndex(m_contraints.size() - 1);
				}

				// Particle to the right and below exists
				if ((col < m_particlesWidthCount - 2) && (row < m_particlesHeightCount - 2))
				{
					VALIDATE(MakeConstraint(GetParticleIndex(col, row), GetParticleIndex(col + 2, row + 2), false));
					GetParticle(col, row)->AddContraintIndex(m_contraints.size() - 1);
					GetParticle(col + 2, row + 2)->AddContraintIndex(m_contraints.size() - 1);
					VALIDATE(MakeConstraint(GetParticleIndex(col + 2, row), GetParticleIndex(col, row + 2), false));
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

	// Add a little wind force to settle the cloth
	AddWindForce({ 0.0f, 0.0f, 0.001f });
	Process(CT_NONE);

	m_initialisedParticles = true;
	return true;
}

void Physics_Cloth::ResizeWidth(bool _smaller)
{
	if (_smaller == true)	// Make the cloth smaller in width
	{
		if (m_width > m_minWidth)	// Make smaller only if the cloth isn't at minimum width already
		{
			m_width -= 2;

			// Update Particle Width Count based on new width
			m_particlesWidthCount = m_width + 1;

			// Set the initialized to false to recreate the cloth again
			m_initialisedParticles = false;
			ResetCloth();
		}
	}
	else	// Make the cloth bigger in width
	{
		if (m_width < m_maxWidth)	// Make bigger only if the cloth isn't at maximum width already
		{
			m_width += 2;

			// Update Particle Width Count based on new width
			m_particlesWidthCount = m_width + 1;

			// Set the initialized to false to recreate the cloth again
			m_initialisedParticles = false;
			ResetCloth();
		}
	}
}

void Physics_Cloth::ResizeHeight(bool _smaller)
{
	if (_smaller == true)	// Make the cloth smaller in height
	{
		if (m_height > m_minHeight)	// Make the cloth smaller in height
		{
			m_height -= 2;

			// Update Particle Height Count based on new width
			m_particlesHeightCount = m_height + 1;

			// Set the initialized to false to recreate the cloth again
			m_initialisedParticles = false;
			ResetCloth();
		}
	}
	else	// Make the cloth bigger in height
	{
		if (m_height < m_maxHeight)	// Make bigger only if the cloth isn't at maximum height already
		{
			m_height += 2;

			// Update Particle Height Count based on new width
			m_particlesHeightCount = m_height + 1;

			// Set the initialized to false to recreate the cloth again
			m_initialisedParticles = false;
			ResetCloth();
		}
	}	
}

void Physics_Cloth::ResizeHooks(bool _less)
{
	if (_less == true)
	{
		if (m_hooks > m_minHooks)
		{
			m_hooks--;
			ResetCloth();
		}
	}
	else
	{
		if (m_hooks < m_maxHooks && m_hooks < m_particlesWidthCount)
		{
			m_hooks++;
			ResetCloth();
		}
	}
}

void Physics_Cloth::CreateHooks()
{
	// Position the particles Closer Together, to create the Curtain effect
	for (int i = 0; i < (m_particlesWidthCount - 1) / 2; i++)
	{
		float movement = 1.0f - ((float)i / ((float)(m_particlesWidthCount - 1) / 2.0f));
		GetParticle(i, 0)->Move({ movement, 0.0f, 0.0f });
		GetParticle((m_particlesWidthCount - i - 1), 0)->Move({ -movement, 0.0f, 0.0f });
	}

	// Select the Particles and Pin them
	float pinDist = (float)m_width / (float)(m_hooks - 1);
	int roundedDist = (int)round(pinDist);

	int increment = 0;
	for (int i = 0; i < m_hooks; i++)
	{
		if (i % 2 == 0) // Even i - Go Right from Left
		{
			HookParticle(GetParticle((int)(increment * roundedDist), 0));
		}
		else // Odd i - Go Left from Right
		{
			HookParticle(GetParticle((m_width - (int)(increment * roundedDist)), 0));
			increment++;
		}
	 }


	//for (int i = 0; i < (m_particlesWidthCount - 1) / 2; i++)
	//{
	//	float movement = 1.0f - ((float)i / ((float)(m_particlesWidthCount - 1) / 2.0f));
	//
	//	GetParticle(i, 0)->Move({ movement, 0.0f, 0.0f });
	//	GetParticle((m_particlesWidthCount - i - 1), 0)->Move({ -movement, 0.0f, 0.0f });
	//}
	//
	//// Determine the number of hooks to place limited by the width of the current cloth
	//int hooks = m_hooks;
	//if (m_width < m_hooks)
	//{
	//	hooks = m_width;
	//}
	//
	//float increment = (float)m_width / ((float)hooks - 1.0f);
	//
	//for (int i = 0; i < hooks; i++)
	//{
	//	HookParticle(GetParticle((int)(i * increment), 0));
	//	HookParticle(GetParticle((m_particlesWidthCount - (int)(i * increment) - 1), 0));
	//}	
}

void Physics_Cloth::FloorCollision(float _floorPos)
{
	v3float up = { 0.0f, 1.0f, 0.0f };

	for (int i = 0; i < m_particleCount; i++)
	{
		if (m_pParticles[i].GetPosition()->y <= _floorPos + 0.1f)
		{
			// Push the particles up if they are under the floor
			float pierce = abs(m_pParticles[i].GetPosition()->y - (_floorPos));
			//m_pParticles[i].Move(up*pierce);
			m_pParticles[i].SetPosition(*m_pParticles[i].GetPosition() + (up*pierce));

		}
	}
}

void Physics_Cloth::SphereCollision(v3float _center, float _radius)
{
	// TO DO CAL - re comment
	for (int i = 0; i < m_particleCount; i++)
	{
		// Calculate the Direction of and length of a potential Pierce
		v3float pierce = *m_pParticles[i].GetPosition() - _center;
		float pierceLength = pierce.Magnitude();

		// Check if the Particle is Inside the Sphere
		if (pierceLength < _radius)
		{
			// Push the Particle out of the Sphere
			m_pParticles[i].Move(pierce.Normalise() * (_radius - pierceLength));
		}
	}
}

void  Physics_Cloth::CapsuleCollision(v3float _endPoint1, v3float _endPoint2, float _capsuleRadius)
{
	for (int i = 0; i < m_particleCount; i++)
	{
		
		v3float particlePos =(*m_pParticles[i].GetPosition()) ;// v3float(0.0f, 0.0f, 0.0f);

		v3float LineDiffVect = _endPoint2 - _endPoint1;
		float lineSegSqrLength = LineDiffVect.Magnitude();
		lineSegSqrLength *= lineSegSqrLength;

		v3float LineToPointVect = particlePos - _endPoint1;
		float dotProduct = LineToPointVect.Dot(LineDiffVect);

		float percAlongLine = dotProduct / lineSegSqrLength ;

		if (percAlongLine  < 0.0f)
		{
			percAlongLine = 0.0f;
		}
		else if(percAlongLine  > 1.0f)
		{
			percAlongLine = 1.0f;
		}
		else
		{
			int c = 9;
		}

		v3float pointOnLine = (_endPoint1 + (LineDiffVect *percAlongLine));

		if ((!(pointOnLine == _endPoint2)) || (!(pointOnLine == _endPoint2)))
		{
		
			int c = 0;

		}
		// Calculate the Direction of and length of a potential Pierce
		v3float pierce = particlePos - pointOnLine;
		float pierceLength = pierce.Magnitude();

		// Check if the Particle is Inside the Sphere
		if (pierceLength < _capsuleRadius)
		{
			// Push the Particle out of the Sphere
			m_pParticles[i].Move(pierce.Normalise() * (_capsuleRadius - pierceLength));
		}



		//v3float capsuleCentreVector = _endPoint2 - _endPoint1;
		//float distanceFactorFromEP1 = (*m_pParticles[i].GetPosition()).Dot(_endPoint1) / capsuleCentreVector.Dot(capsuleCentreVector);
		//if (distanceFactorFromEP1 < 0){ distanceFactorFromEP1 = 0; }
		//if (distanceFactorFromEP1 > 1){ distanceFactorFromEP1 = 1; }
		//v3float closetPoint = _endPoint1 + (capsuleCentreVector * distanceFactorFromEP1);
		//v3float collisionVector = (*m_pParticles[i].GetPosition()) - closetPoint;
		//float distance = collisionVector.Magnitude();
		//v3float collisionNormal = collisionVector.Normalise();
		//if (distance < _capsuleRadius)
		//{
		//	m_pParticles[i].Move(collisionVector.Normalise() * (_capsuleRadius - distance));
		//}



	}
	//vector cylCenterVector = endPoint2 - endpoint1;
	//float distanceFactorFromEP1 = Dot(sphereCenter - endPoint1) / Dot(cylCenterVector, cylCenterVector);
	//if (distanceFactorFromEP1 < 0) distanceFactorFromEP1 = 0;// clamp to endpoints if neccesary
	//if (distanceFactorFromEP1 > 1) distanceFactorFromEP1 = 1;
	//vector closestPoint = endPoint1 + (cylCenterVector * distanceFactorFromEP1);
	//vector collisionVector = sphereCenter - closestPoint;
	//float distance = collisionVector.Length();
	//vector collisionNormal = collisionVector / distance;
	//if (distance < sphereRadius + cylRadius)
	//{
	//	//collision occurred. use collisionNormal to reflect sphere off cyl
	//	float factor = Dot(velocity, collisionNormal);
	//	velocity = velocity - (2 * factor * collisionNormal);
	//}


}

void  Physics_Cloth::PyramidCollision()
{

}

void Physics_Cloth::UpdateWindSpeed(float _speed)
{
	// Update the Wind Speed
	m_windSpeed += _speed;

	// Ensure the Windspeed exists
	if (m_windSpeed < 1.0f)
	{
		m_windSpeed = 1.0f;
	}
}

void Physics_Cloth::Ignite(TCameraRay _camRay)
{
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		BurnConnectedConstraints(m_selectedParticles[i]);
	}
}

void Physics_Cloth::Cut(TCameraRay _camRay)
{
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		// FOR JC
		m_selectedParticles[i]->SetSelectedPosition({ 1000000.0f, 1000000.0f, 10000000.0f });
	}
}

void Physics_Cloth::Manipulate(TCameraRay _camRay)
{
	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		// TO DO CAL - re comment
		v3float tmpA = (*(m_selectedParticles[i]->GetPosition()) - _camRay.Origin);
		_camRay.Direction.Normalise();
		float tmpC = (_camRay.Direction.Dot(tmpA));
		v3float Result = _camRay.Origin + _camRay.Direction * tmpC;
		Result.z = m_selectedParticles[i]->GetPosition()->z;
		m_selectedParticles[i]->SetSelectedPosition(Result);
	}
}

void Physics_Cloth::SelectParticles(TCameraRay _camRay, float _selectRadius)
{
	// TO DO CAL

	ReleaseSelected();
	// Check The Ray with each particle
	for (int i = 0; i < m_particleCount; i++)
	{
		// Calculate the Individual World matrix of each particle
		D3DXMATRIX translationMatrix;
		D3DXMATRIX transWorldMatrix;
		D3DXMatrixTranslation(&translationMatrix, m_pParticles[i].GetPosition()->x, m_pParticles[i].GetPosition()->y, m_pParticles[i].GetPosition()->z);
		D3DXMatrixMultiply(&transWorldMatrix, &m_matWorld, &translationMatrix);

		// Get the Inverse of the Translated World Matrix
		D3DXMATRIX invWorldMatrix;
		D3DXMatrixInverse(&invWorldMatrix, NULL, &transWorldMatrix);

		// Transform the Ray Origin and the Ray Direction from View Space to World Space
		D3DXVECTOR3 Origin = { _camRay.Origin.x, _camRay.Origin.y, _camRay.Origin.z };
		D3DXVECTOR3 Direction = { _camRay.Direction.x, _camRay.Direction.y, _camRay.Direction.z };
		D3DXVECTOR3  tmpRayDirection, tmpRayOrigin;
		D3DXVec3TransformCoord(&tmpRayOrigin, &Origin, &invWorldMatrix);
		D3DXVec3TransformNormal(&tmpRayDirection, &Direction, &invWorldMatrix);
		TCameraRay TransformedRay({ tmpRayOrigin.x, tmpRayOrigin.y, tmpRayOrigin.z }, { tmpRayDirection.x, tmpRayDirection.y, tmpRayDirection.z });
		TransformedRay.Direction.Normalise();

		// Perform the Ray-Sphere Intersection Test
		// Where the Sphere is Around each individual Cloth Particle
		bool intersect = TransformedRay.RaySphereIntersect(_selectRadius);
		// Handle the Intersection
		if (intersect == true)
		{
			m_pParticles[i].SetSelectedState(true);
			// Color Selected Particles Red
			//m_pVertices[i].color = d3dxColors::Red;

			m_selectedParticles.push_back(&m_pParticles[i]);
			//pSelectedParticle = &m_pParticleArray[i];
		}
		else
		{
			m_pParticles[i].SetSelectedState(false);
		}
	}
}

void Physics_Cloth::ReleaseSelected()
{
	// TO DO CAL - re comment

	for (UINT i = 0; i < m_selectedParticles.size(); i++)
	{
		int ID = m_selectedParticles[i]->GetParticleID();
		if (m_selectedParticles[i]->GetStaticState())
		{
			m_pVertices[ID].color = d3dxColors::Blue;
		}
		else if (m_selectedParticles[i]->GetIgnitedStatus())
		{
			m_pVertices[ID].color = d3dxColors::Red;
		}
		else
		{
			m_pVertices[ID].color = d3dxColors::Black;
		}
		m_selectedParticles[i]->SetSelectedState(false);
	}
	m_selectedParticles.clear();
}





// Private Functions

bool Physics_Cloth::MakeConstraint(int _particleIndexA, int _particleIndexB, bool _immediate, bool _draw)
{
	// Create and initialize a new constraint
	m_contraints.push_back(Physics_Constraint());
	VALIDATE(m_contraints.back().Initialise(&m_pParticles[_particleIndexA], &m_pParticles[_particleIndexB], _immediate, m_breakModifier));

	if (_draw == true)
	{
		// Add indices to the index array to draw them
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

void Physics_Cloth::BurnConnectedConstraints(Physics_Particle* _pParticle)
{
	if (_pParticle != 0)
	{
		if (_pParticle->GetIgnitedStatus() == false)
		{
			float modifier = 1 + (float)((rand() % 60) - 30) / 100.0f;
			float modifiedBurnTime = m_burnTime * modifier;

			// Ignite the Particles
			_pParticle->Ignite(modifiedBurnTime);

			std::vector<UINT> connectedConstraints = _pParticle->GetContraintIndices();
			for (UINT i = 0; i < connectedConstraints.size(); i++)
			{
				if (m_contraints[connectedConstraints[i]].CanBeIgnited() == true)
				{
					// TO DO CAL - create variable for burn time and random function to 
					m_contraints[connectedConstraints[i]].Ignite(modifiedBurnTime);
				}
			}
		}
	}
}

void Physics_Cloth::SelfCollisions()
{
	for (int i = 0; i < m_particleCount; i++)
	{
		v3float particlePos = *m_pParticles[i].GetPosition();

		// TO DO CAL - re comment
		for (int j = i + 1; j < m_particleCount; j++)
		{
			// Calculate the Direction of and length of a potential Pierce
			v3float pierce = *m_pParticles[j].GetPosition() - particlePos;
			float pierceLength = pierce.Magnitude();

			// Check if the Particle is Inside the Sphere
			if (pierceLength < 0.9f)
			{
				// Push the Particle out of the Sphere
				//m_pParticles[j].SetPosition(*m_pParticles[j].GetPosition() + ((pierce.Normalise() * (0.5f - pierceLength))/2.0f));
				//m_pParticles[i].SetPosition(*m_pParticles[i].GetPosition() + ((pierce.Normalise() * (0.5f - pierceLength))/-2.0f));
				m_pParticles[j].Move((pierce.Normalise() * (0.9f - pierceLength))/2.0f);
				m_pParticles[i].Move((pierce.Normalise() * (0.9f - pierceLength)) / -2.0f);
			}
		}
	}

	
}




