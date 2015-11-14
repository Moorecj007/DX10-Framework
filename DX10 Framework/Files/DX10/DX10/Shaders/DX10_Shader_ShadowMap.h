/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_ShadowMap.h
* Description : Shader Instructions for a the ShadowMap shader
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_SHADOWMAP_H__
#define __DX10_SHADER_SHADOWMAP_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

class DX10_Shader_ShadowMap
{
public:

	/***********************
	* DX10_Shader_ShadowMap: Default Constructor for ShadowMap Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_ShadowMap()
	{
	}

	/***********************
	* ~DX10_Shader_ShadowMap: Default Destructor for ShadowMap Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_ShadowMap()
	{
	}

	/***********************
	* Initialise: Initialise the shader for use
	* @author: Callan Moore
	* @parameter: _pDX10_Renderer: DX10 Renderer for this application
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pDX10_Renderer)
	{
		m_pDX10_Renderer = _pDX10_Renderer;

		VALIDATE(BuildFX());
		VALIDATE(CreateFXVarPointers());
		VALIDATE(CreateVertexLayout());

		return true;
	}

	/***********************
	* SetUpPerFrame: Setup the shader file with the variables used for the whole frame
	* @author: Callan Moore
	* @return: void
	********************/
	void SetUpPerFrame()
	{

	}

	/***********************
	* Render: Ready the shader technique with object specific details and setting the objects mesh to render
	* @author: Callan Moore
	* @return: void
	********************/
	void Render(DX10_Obj_Generic* _obj, D3DXMATRIX _matLightView)
	{
		m_pDX10_Renderer->SetPrimitiveTopology(_obj->GetMesh()->GetPrimTopology());
		m_pDX10_Renderer->SetInputLayout(m_pVertexLayout_Standard);

		D3D10_TECHNIQUE_DESC techDesc;
		m_pTech_Standard->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			D3DXMATRIX objWVP = *_obj->GetWorldMatrix() * _matLightView * m_pDX10_Renderer->GetShadowProjMatrix();

			m_pMatWVP->SetMatrix((float*)&objWVP);
			m_pMapDiffuse->SetResource(_obj->GetTexture());

			m_pTech_Standard->GetPassByIndex(i)->Apply(0);
			_obj->Draw();
		}
	}

private:

	/***********************
	* BuildFX: Build the FX file for the Shader
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool BuildFX()
	{
		VALIDATE(m_pDX10_Renderer->BuildFX("ShadowMap.fx", "CreateShadowMapTech", m_pFX, m_pTech_Standard));

		return true;
	}

	/***********************
	* CreateFXVarPointers: Create the pointers to the FX memory on the graphics card
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool CreateFXVarPointers()
	{
		// Per Frame	
		m_pMatWVP = m_pFX->GetVariableByName("g_matLightWVP")->AsMatrix();

		// Globals
		m_pMapDiffuse = m_pFX->GetVariableByName("g_mapDiffuse")->AsShaderResource();

		VALIDATE(m_pMatWVP != 0);
		VALIDATE(m_pMapDiffuse != 0);

		return true;
	}

	/***********************
	* CreateVertexLayout: Create the vertex layout for this shader
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool CreateVertexLayout()
	{
		// Vertex Desc for a basic vertex with Normals and UV coordinates
		D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT elementNum = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Standard, m_pVertexLayout_Standard);

		return true;
	}

private:
	DX10_Renderer*						m_pDX10_Renderer;
	ID3D10Effect* m_pFX;

	ID3D10InputLayout*					m_pVertexLayout_Standard;
	ID3D10EffectTechnique*				m_pTech_Standard;

	ID3D10EffectMatrixVariable*			m_pMatWVP;
	ID3D10EffectShaderResourceVariable* m_pMapDiffuse;

};

#endif	// __DX10_SHADER_SHADOWMAP_H__