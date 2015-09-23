/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_Water.h
* Description : 3D Lit Texture Water Object for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_OBJ_WATER_H__
#define __DX10_OBJ_WATER_H__

// Local Includes
#include "DX10_Obj_Generic.h"
#include "../Shaders/DX10_Shader_Water.h"

class DX10_Obj_Water :
	public DX10_Obj_Generic
{
public:

	/***********************
	* DX10_Obj_Water: Constructor for Water Object class
	* @author: Callan Moore
	* @parameter: _pShader: The Shader for this Object
	* @parameter: _textureID: The ID for the Texture for this Object
	* @parameter: _textureTime: Time in seconds to run through all animation frames
	********************/
	DX10_Obj_Water();

	/***********************
	* ~DX10_Obj_Water: Default Destructor for Water Object class
	* @author: Callan Moore
	********************/
	~DX10_Obj_Water();

	/***********************
	* Initialise: Initialise the Water Object for use
	* @author: Callan Moore
	* @parameter: _pRenderer: Renderer for this Application
	* @parameter: _pMesh: Mesh for this Object
	* @parameter: _pShader: The Shader for this Object
	* @parameter: _textureID: The ID vector for the Textures for this Object
	* @parameter: _animationSpeed: Time in seconds to run through all animation frames
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_Water* _pShader, std::vector<std::string>* _pTexNames, float _animationSpeed = 1.0f);
	
	/***********************
	* Initialise: Initialise the Water Object for use
	* @author: Callan Moore
	* @parameter: _pRenderer: Renderer for this Application
	* @parameter: _pMesh: Mesh for this Object
	* @parameter: _pShader: The Shader for this Object
	* @parameter: _textureID: ID of the single texture for this object to display
	* @return: bool : Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_Water* _pShader, std::string _texNames);

	/***********************
	* Process: Process the new frame and update the Water Object
	* @author: Callan Moore
	* @parameter: _dt: The delta tick for this frame
	* @return: void
	********************/
	virtual void Process(float _dt);

	/***********************
	* Render: Render the Water Object to the screen space
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render(eTech_Water _tech = TECH_WATER_STANDARD);

	/***********************
	* SetTransparency: Set the transparency value of the Water
	* @author: Callan Moore
	* @parameter: _transparency: 0-1 transparency value
	* @return: void
	********************/
	virtual void SetTransparency(float _transparency) { m_transparency = _transparency; };

protected:
	DX10_Shader_Water* m_pShader;

	float m_transparency;
};
#endif	// __DX10_OBJ_WATER_H__
