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

Physics_Rope_2D::Physics_Rope_2D(b2Joint* _pJoint, float _pixelsPerMeter)
{
	m_pJoint = _pJoint;
	m_pixelsPerMeter = _pixelsPerMeter;
}

Physics_Rope_2D::~Physics_Rope_2D()
{
}

v2float Physics_Rope_2D::GetAnchorA()
{
	b2Vec2 b2AnchorA = m_pJoint->GetAnchorA();

	v2float anchorA{ b2AnchorA.x, b2AnchorA.y };
	return (anchorA * m_pixelsPerMeter);
}

v2float Physics_Rope_2D::GetAnchorB()
{
	b2Vec2 b2AnchorB = m_pJoint->GetAnchorB();

	v2float anchorB{ b2AnchorB.x, b2AnchorB.y };
	return (anchorB * m_pixelsPerMeter);
}