/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Line.h
* Description : A line for drawing in GDI
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_LINE_H__
#define __GDI_LINE_H__

// Local Includes
#include "../../Utility/Utilities.h"
#include "../../Physics/2D Physics/Physics_Joint_2D.h"
#include "GDI_Renderer.h"

class GDI_Line
{
public:

	/***********************
	* GDI_Line: Constructor for GDI Line class
	* @author: Callan Moore
	* @parameter: _pGDI_Renderer: GDi Renderer for this line
	********************/
	GDI_Line(GDI_Renderer* _pGDI_Renderer);

	/***********************
	* ~GDI_Line: Destructor for GDI Line class
	* @author: Callan Moore
	********************/
	~GDI_Line();

	// TO DO
	bool Initialise(Physics_Joint_2D* _pJoint, COLORREF _color);

	// TO DO
	bool Initialise(v2float _posA, v2float _posB, COLORREF _color);

	// TO DO
	void Process(float _dt);

	// TO DO
	void Render();

private:
	GDI_Renderer* m_pGDI_Renderer;
	Physics_Joint_2D* m_pJoint;
	v2float m_posA;
	v2float m_posB;
	COLORREF m_color;
};
#endif	// __GDI_LINE_H__
