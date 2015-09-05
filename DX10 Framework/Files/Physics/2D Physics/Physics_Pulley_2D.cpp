/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Pulley_2D.cpp
* Description : Physics representation of a pulley system
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Pulley_2D.h"

Physics_Pulley_2D::Physics_Pulley_2D(b2PulleyJoint* _pJoint, float _pixelsPerMeter)
{
	m_pPulleyJoint = _pJoint;
	m_pixelsPerMeter = _pixelsPerMeter;
}

Physics_Pulley_2D::~Physics_Pulley_2D()
{
}

v2float Physics_Pulley_2D::GetAnchorA()
{
	b2Vec2 b2Anchor = m_pPulleyJoint->GetAnchorA();
	return{ b2Anchor.x * m_pixelsPerMeter, b2Anchor.y * m_pixelsPerMeter };
}

v2float Physics_Pulley_2D::GetAnchorB()
{
	b2Vec2 b2Anchor = m_pPulleyJoint->GetAnchorB();
	return{ b2Anchor.x * m_pixelsPerMeter, b2Anchor.y * m_pixelsPerMeter };
}

v2float Physics_Pulley_2D::GetGroundAnchorA()
{
	b2Vec2 b2GroundAnchor = m_pPulleyJoint->GetGroundAnchorA();
	return{ b2GroundAnchor.x * m_pixelsPerMeter, b2GroundAnchor.y * m_pixelsPerMeter };
}

v2float Physics_Pulley_2D::GetGroundAnchorB()
{
	b2Vec2 b2GroundAnchor = m_pPulleyJoint->GetGroundAnchorB();
	return{ b2GroundAnchor.x * m_pixelsPerMeter, b2GroundAnchor.y * m_pixelsPerMeter };
}

