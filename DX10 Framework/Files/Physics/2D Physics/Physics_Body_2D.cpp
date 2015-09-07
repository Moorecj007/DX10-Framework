/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Body_2D.cpp
* Description : A body that holds all physics information for a 2D object
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Body_2D.h"

Physics_Body_2D::Physics_Body_2D(b2World* _pPhysWorld, b2Body* _pBody, float _radius, float _pixelsPerMeter)
{
	m_pPhysWorld = _pPhysWorld;
	m_pBody = _pBody;
	m_radius = _radius;
	m_pixelsPerMeter = _pixelsPerMeter;
	m_pPoints = 0;

	ZeroMemory(&m_collisionProps, sizeof(m_collisionProps));
}

Physics_Body_2D::Physics_Body_2D(b2World* _pPhysWorld, b2Body* _pBody, TPhysicsProperties _physProps, float _pixelsPerMeter)
{
	m_pPhysWorld = _pPhysWorld;
	m_pBody = _pBody;
	m_pPoints = _physProps.pPoints;
	m_size = _physProps.size;
	m_pixelsPerMeter = _pixelsPerMeter;
	m_physProps = _physProps;

	ZeroMemory(&m_collisionProps, sizeof(m_collisionProps));
}

Physics_Body_2D::~Physics_Body_2D()
{
	m_pPhysWorld->DestroyBody(GetBody());
	ReleasePtrArray(m_pPoints);
}

v2float Physics_Body_2D::GetPosition()
{
	b2Vec2 bodyPos = m_pBody->GetPosition();
	v2float pos;
	pos.x = bodyPos.x * m_pixelsPerMeter;
	pos.y = bodyPos.y * m_pixelsPerMeter;

	return pos;
}