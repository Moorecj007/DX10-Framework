/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_03.h
* Description : Builds and runs Level 01 for the game
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __LEVEL_03_H__
#define __LEVEL_03_H__

// Local Includes
#include "Level_Generic.h"

class Level_03 :
	public Level_Generic
{
public:

	/***********************
	* Level_02: Default Constructor for Level 03
	* @author: Callan Moore
	* @parameter: _pGDI_Renderer: The Renderer for this level
	* @parameter: _clientWidth: The Width of the client window
	* @parameter: _clientHeight: The Height of the client window
	********************/
	Level_03(GDI_Renderer* _pGDI_Renderer, int _clientWidth, int _clientHeight);

	/***********************
	* Level_01: Default Destructor for Level 03
	* @author: Callan Moore
	********************/
	~Level_03();

	/***********************
	* ContructLevel: Construct the entire level of physics and GDI objects
	* @author: Callan Moore
	* @return: bool
	********************/
	virtual bool ContructLevel();
};
#endif	// __LEVEL_03_H__
