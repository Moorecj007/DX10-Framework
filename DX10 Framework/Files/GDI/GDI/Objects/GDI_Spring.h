/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Spring.h
* Description : GDI operations for a Spring
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_SPRING_H__
#define __GDI_SPRING_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../GDI_Renderer.h"
#include "../../../Physics/2D Physics/Physics_Spring_2D.h"

class GDI_Spring
{
public:

	/***********************
	* GDI_Spring: Default Constructor for GDI Spring class
	* @author: Callan Moore
	* @parameter: _pGDI_Renderer; The renderer for this object
	********************/
	GDI_Spring(GDI_Renderer* _pGDI_Renderer);

	/***********************
	* ~GDI_Spring: Default Destructor for GDI Spring class
	* @author: Callan Moore
	********************/
	~GDI_Spring();
	
	/***********************
	* Initialise: Initialise the Spring for use
	* @author: Callan Moore
	* @parameter: _pSpring:The physics representation of a spring
	* @parameter: _color: Color for the spring
	* @return: bool: Successful or not
	********************/
	bool Initialise(Physics_Spring_2D* _pSpring, COLORREF _color);

	
	/***********************
	* Process: Process and update the Spring Object
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	void Process(float _dt);
	
	/***********************
	* Render: Render the spring to the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

private:
	GDI_Renderer* m_pGDI_Renderer;
	Physics_Spring_2D* m_pSpring;
	COLORREF m_color;
};
#endif	// __GDI_SPRING_H__

