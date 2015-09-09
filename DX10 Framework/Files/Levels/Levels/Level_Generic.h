/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_Generic.h
* Description : Base class for all levels to be derived from
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __LEVEL_GENERIC_H__
#define __LEVEL_GENERIC_H__

// Local Includes
#include "../../Utility/Utilities.h"
#include "../../Physics/Physics_2D.h"
#include "../../GDI/GDI.h"

enum eLevelSelection
{
	LS_NONE,
	LS_LEVEL01,
	LS_LEVEL02,
	LS_LEVEL03
};

class Level_Generic
{
public:

	/***********************
	* ~Level_Generic: Default Destructor for Level Generic class
	* @author: Callan Moore
	********************/
	virtual ~Level_Generic();
	
	/***********************
	* ContructLevel: Construct the entire level of physics and GDI objects
	* @author: Callan Moore
	* @return: bool
	********************/
	virtual bool ContructLevel() = 0;

	/***********************
	* InitialSetup: Initial setup of the background and borders
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool InitialSetup();
	
	/***********************
	* DestroyLevel: Destroy all level objects
	* @author: Callan Moore
	* @return: void
	********************/
	void DestroyLevel();
	
	/***********************
	* ResetLevel: Reset the level to the beginning
	* @author: Callan Moore
	* @return: void
	********************/
	void ResetLevel();
	
	/***********************
	* Process: Process the next frame for the level
	* @author: Callan Moore
	* @parameter: _dt: The current Delta tick
	* @return: bool: True if the level is completed during this process
	********************/
	bool Process(float _dt);
	
	/***********************
	* Render: Render the Level to the client window
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();
	
	/***********************
	* CutRope: Check if the cutting line crosses any ropes and cuts each rope that does
	* @author: Callan Moore
	* @parameter: _cutLinePtA: First point of the cutting line
	* @parameter: _cutLinePtB: Second point of the cutting line
	* @return: void
	********************/
	void CutRope(v2float _cutLinePtA, v2float _cutLinePtB);

protected:
	GDI_Renderer* m_pGDI_Renderer;
	int m_clientWidth;
	int m_clientHeight;

	// World Variables
	Physics_World_2D* m_pPhysWorld;

	// Level Objects
	GDI_Obj_Generic* m_pBackground;
	GDI_Obj_Generic* m_pGem;
	GDI_Obj_Generic* m_pWinZone;

	std::vector<GDI_Obj_Generic*>* m_pObjStatics;
	std::vector<GDI_Obj_Generic*>* m_pObjDynamics;
	std::vector<GDI_Obj_Generic*>* m_pObjBreakables;

	std::vector<GDI_Obj_Rope*>* m_pRopes_Cuttable;
	std::vector<GDI_Obj_Rope*>* m_pRopes_Unbreakable;
	std::vector<GDI_Obj_Pulley*>* m_pObjPulleys;
};
#endif	// __LEVEL_GENERIC_H__

