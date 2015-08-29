/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_LitTex.h
* Description : 3D Lit Texture Object for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_OBJ_LITTEX_H__
#define __DX10_OBJ_LITTEX_H__

// Local Includes
#include "DX10_Obj_Generic.h"
#include "../DX10_Shader_Structures.h"
#include "../Shaders/DX10_Shader_LitTex.h"

class DX10_Obj_LitTex :
	public DX10_Obj_Generic
{
public:

	/***********************
	* CDX10_LitTex_Obj: Constructor for Lit Texture Object class
	* @author: Callan Moore
	* @parameter: _pShader: The Shader for this Object
	* @parameter: _textureID: The ID for the Texture for this Object
	* @parameter: _textureTime: Time in seconds to run through all animation frames
	********************/
	DX10_Obj_LitTex(DX10_Shader_LitTex* _pShader, std::vector<UINT>* _textureID, float _textureTime);

	/***********************
	* ~CDX10_LitTex_Obj: Default Destructor for Lit Texture Object class
	* @author: Callan Moore
	********************/
	~DX10_Obj_LitTex();

	/***********************
	* Process: Process the new frame and update the LitTex Object
	* @author: Callan Moore
	* @parameter: _dt: The delta tick for this frame
	* @return: void
	********************/
	virtual void Process(float _dt);

	/***********************
	* Render: Render the Lit Texture Object to the screen space
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render();

private:
	DX10_Shader_LitTex* m_pShader;
	std::vector<UINT>* m_pTextureID;
	UINT m_texIndex;
	float m_timeElapsed;
	float m_texTimer;
};
#endif	// __DX10_OBJ_LITTEX_H__
