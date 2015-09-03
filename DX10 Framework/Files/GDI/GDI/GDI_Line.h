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
	* @parameter: _pGDI_Renderer: GDI Renderer for this line
	********************/
	GDI_Line(GDI_Renderer* _pGDI_Renderer);

	/***********************
	* ~GDI_Line: Destructor for GDI Line class
	* @author: Callan Moore
	********************/
	~GDI_Line();

	/***********************
	* Initialise: Initialise a line using a physics joint
	* @author: Callan Moore
	* @parameter: _pJoint: Physics Rope Joint
	* @parameter: _color: Color for the Line
	* @return: bool: Successful or not
	********************/
	bool Initialise(Physics_Joint_2D* _pJoint, COLORREF _color);

	/***********************
	* Initialise: Initialise a line using points on the screen
	* @author: Callan Moore
	* @parameter: _posA: The first point for the line
	* @parameter: _posA: The second point for the line
	* @parameter: _color: Color for the Line
	* @return: bool: Successful or not
	********************/
	bool Initialise(v2float _posA, v2float _posB, COLORREF _color);

	/***********************
	* Process: Process any changes to the line for the current frame
	* @author: Callan Moore
	* @parameter: _dt; The current delta tick
	* @return: void
	********************/
	void Process(float _dt);

	/***********************
	* Render: Draw the line to the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

private:
	GDI_Renderer* m_pGDI_Renderer;
	Physics_Joint_2D* m_pJoint;
	v2float m_posA;
	v2float m_posB;
	COLORREF m_color;
};
#endif	// __GDI_LINE_H__
