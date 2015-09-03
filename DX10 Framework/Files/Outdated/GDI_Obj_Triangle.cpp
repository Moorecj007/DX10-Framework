/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Triangle.h
* Description : A Triangle for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Obj_Triangle.h"


GDI_Obj_Triangle::GDI_Obj_Triangle(GDI_Renderer* _pGDIRenderer)
{
	m_pGDIRenderer = _pGDIRenderer;
}


GDI_Obj_Triangle::~GDI_Obj_Triangle()
{
}

bool GDI_Obj_Triangle::Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _color)
{
	// Assign Member variables
	m_pPhysicsBody = _pPhysicsBody;
	m_color = _color;
	m_size = 3;

	// Create a new array of points
	m_points = new v2float[m_size];

	return true;
}

void GDI_Obj_Triangle::Process(float _dt)
{
	//v2float pos = m_pPhysicsBody->GetPosition();
	////v2float scale = m_pPhysicsBody->GetScale();
	//float angle = m_pPhysicsBody->GetAngle();
	//
	//// Bottom Left point
	//m_points[0].x = (pos.x - (scale.x / 2.0f));
	//m_points[0].y = (pos.y + (scale.y / 2.0f));
	//
	//// Top Middle point
	//m_points[1].x = (pos.x);
	//m_points[1].y = (pos.y - (scale.y / 2.0f));
	//
	//// Bottom Right point
	//m_points[2].x = (pos.x + (scale.x / 2.0f));
	//m_points[2].y = (pos.y + (scale.y / 2.0f));
	//
	//// Calculate the Rotation
	//for (int i = 0; i < m_size; i++)
	//{
	//	CalcRotation(&m_points[i], angle, pos);
	//}
}
