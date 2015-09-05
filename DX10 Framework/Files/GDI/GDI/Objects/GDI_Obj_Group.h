/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Group.h
* Description : A group ob objects that act as one
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_GROUP_H__
#define __GDI_OBJ_GROUP_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../GDI_Renderer.h"
#include "GDI_Obj_Polygon.h"

class GDI_Obj_Group
{
public:

	/***********************
	* GDI_Obj_Group: Constructor for GDI Object Group class
	* @author: Callan Moore
	********************/
	GDI_Obj_Group();

	/***********************
	* ~GDI_Obj_Group: Destructor for GDI Object Group class
	* @author: Callan Moore
	********************/
	~GDI_Obj_Group();

	/***********************
	* Initialise: Initialise the group object
	* @author: Callan Moore
	*@parameter: _pGDI_Renderer : GDI Renderer for this object
	* @parameter: _pPhysBodies: All the Physics bodies for the object
	* @parameter: _color: Color for all objects within the group
	* @return: bool: Successful or not
	********************/
	bool Initialise(GDI_Renderer* _pGDI_Renderer, std::vector<Physics_Body_2D*>* _pPhysBodies, COLORREF _color);
	
	/***********************
	* Process: Process the object group
	* @author: Callan Moore
	* @parameter: _dt: THe current delta tick
	* @return: void
	********************/
	void Process(float _dt);
	
	/***********************
	* Render: Render all the objects in the group
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

private:
	GDI_Renderer* m_pGDI_Renderer;
	std::vector<GDI_Obj_Generic*>* m_pObjects;
};
#endif	// __GDI_OBJ_GROUP_H__

