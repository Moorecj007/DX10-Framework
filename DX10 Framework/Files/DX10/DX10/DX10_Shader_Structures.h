/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_Structures.h
* Description : Holds all the structures for the different Shader Inputs for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_STRUCTURES_H__
#define __DX10_SHADER_STRUCTURES_H__

// Local Includes
#include "DX10_Utilities.h"

#include "Meshes/DX10_Mesh_Generic.h"

/***********************
* TLitTex: Shader Variables for the LitTex FX
* @author: Callan Moore
********************/
struct TLitTex
{
	DX10_Mesh_Generic* pMesh;
	D3DXMATRIX* pMatWorld;
	UINT textureID;
};
#endif	// __DX10_SHADER_STRUCTURES_H__