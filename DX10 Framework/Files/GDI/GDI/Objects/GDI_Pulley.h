/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Pulley.h
* Description : GDI operations for a pulley system
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_PULLEY_H__
#define __GDI_PULLEY_H__

// Local Includes
#include "../GDI_Renderer.h"
#include "GDI_Obj_Polygon.h"
#include "../../../Physics/2D Physics/Physics_World_2D.h"
#include "../../../Physics/2D Physics/Physics_Pulley_2D.h"
#include "../../../Physics/2D Physics/Physics_Utilities.h"

class GDI_Pulley
{
public:

	/***********************
	* GDI_Pulley: Default Constructor for the Pulley class
	* @author: Callan Moore
	* @parameter : _pGDI_Renderer: The renderer for this object
	* @parameter : _pGDI_Renderer: The renderer for this object
	********************/
	GDI_Pulley(GDI_Renderer* _pGDI_Renderer, Physics_World_2D* _pPhysWorld);

	/***********************
	* ~GDI_Pulley: Default Destructor for the Pulley class
	* @author: Callan Moore
	********************/
	~GDI_Pulley();
	
	/***********************
	* Initialise: Initialise the GDI pulley for use
	* @author: Callan Moore
	* @parameter: _pJoint: The joint the pulley is created from
	* @parameter: _color: the color of the pulley
	* @return: bool: Successful or not
	********************/
	bool Initialise(Physics_Pulley_2D* _pJoint, COLORREF _color);
	
	/***********************
	* Process: Process the Pulley
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	void Process(float _dt);
	
	/***********************
	* Render: Render the pulley onto the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

private:
	GDI_Renderer* m_pGDI_Renderer;
	Physics_World_2D* m_pPhysWorld;
	Physics_Pulley_2D* m_pPulley;
	COLORREF m_color;

	GDI_Obj_Generic* m_pStopperA;
	GDI_Obj_Generic* m_pStopperB;
};
#endif	// __GDI_PULLEY_H__
