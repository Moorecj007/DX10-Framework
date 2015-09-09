/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Rope.h
* Description : GDI representation of a rope
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_LINE_H__
#define __GDI_LINE_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../../../Physics/2D Physics/Physics_Rope_2D.h"
#include "../GDI_Renderer.h"

class GDI_Obj_Rope
{
public:

	/***********************
	* GDI_Obj_Rope: Constructor for GDI Rope class
	* @author: Callan Moore
	* @parameter: _pGDI_Renderer: GDI Renderer for this line
	********************/
	GDI_Obj_Rope(GDI_Renderer* _pGDI_Renderer);

	/***********************
	* ~GDI_Obj_Rope: Destructor for GDI Rope class
	* @author: Callan Moore
	********************/
	~GDI_Obj_Rope();

	/***********************
	* Initialise: Initialise a line using a physics joint
	* @author: Callan Moore
	* @parameter: _pPhysRope: Physics Rope Joint
	* @parameter: _color: Color for the Line
	* @return: bool: Successful or not
	********************/
	bool Initialise(Physics_Rope_2D* _pPhysRope, COLORREF _color);

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

	/***********************
	* GetPositionA: Retrieve the first end of the rope line
	* @author: Callan Moore
	* @return: v2float: The world space coordinate of the ropes position of the first end
	********************/
	v2float GetPositionA() { return m_posA; };
	
	/***********************
	* GetPositionB: Retrieve the second end of the rope line
	* @author: Callan Moore
	* @return: v2float: The world space coordinate of the ropes position of the second end
	********************/
	v2float GetPositionB() { return m_posB; };

private:
	GDI_Renderer* m_pGDI_Renderer;
	Physics_Rope_2D* m_pPhysRope;
	v2float m_posA;
	v2float m_posB;
	COLORREF m_color;
};
#endif	// __GDI_LINE_H__
