/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Rope_2D.h
* Description : A physics implementation of a rope
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Rope_2D.h"

Physics_Rope_2D::Physics_Rope_2D(b2World* _pPhysWorld, b2Joint* _pJoint, float _pixelsPerMeter)
{
	m_pPhysWorld = _pPhysWorld;
	m_pJoint = _pJoint;
	m_pixelsPerMeter = _pixelsPerMeter;
}

Physics_Rope_2D::~Physics_Rope_2D()
{
	m_pPhysWorld->DestroyJoint(m_pJoint);
	m_pJoint = 0;
}

v2float Physics_Rope_2D::GetAnchorA()
{
	if (m_pJoint != 0)
	{
		b2Vec2 b2AnchorA = m_pJoint->GetAnchorA();

		m_anchorA = { b2AnchorA.x * m_pixelsPerMeter, b2AnchorA.y * m_pixelsPerMeter };
		return (m_anchorA);
	}
	else
	{
		return m_anchorA;
	}
}

v2float Physics_Rope_2D::GetAnchorB()
{
	if (m_pJoint != 0)
	{
		b2Vec2 b2AnchorB = m_pJoint->GetAnchorB();

		m_anchorB = { b2AnchorB.x * m_pixelsPerMeter, b2AnchorB.y * m_pixelsPerMeter };
		return (m_anchorB);
	}
	else
	{
		return m_anchorB;
	}
}