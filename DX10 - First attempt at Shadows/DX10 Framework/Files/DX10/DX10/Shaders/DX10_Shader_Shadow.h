/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_Shadow.cpp
* Description : Shader Instructions for a the Shadow shader
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_SHADOW_H__
#define __DX10_SHADER_SHADOW_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

class DX10_Shader_Shadow
{
public:

	/***********************
	* DX10_Shader_Shadow: Default Constructor for Shadow Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_Shadow()
	{
	}

	/***********************
	* ~DX10_Shader_Shadow: Default Destructor for Shadow Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_Shadow()
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
		m_pMatProjLight->SetMatrix((float*)m_pDX10_Renderer->GetOrthographicMatrix());		
	}

	/***********************
	* CreateShadowMaps: Creates a shadow map for each light
	* @author: Callan Moore
	// TO DO CAL
	* @return: void
	********************/
	void CreateShadowMaps(DX10_Obj_LitTex* _obj, TLight* _light)
	{		
		D3DXMATRIX matViewLight;
		D3DXMATRIX matWorldObject = _obj->GetWorldMatrix();
		D3DXVECTOR3 lightPosition = { 0, 40, -40 };//{ _light->pos_range.x, _light->pos_range.y, _light->pos_range.z };
		_light->dir_spotPow = { 00, -1, 1, 0 };
		D3DXMatrixLookAtLH(&matViewLight, &lightPosition, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		m_pDX10_Renderer->SetInputLayout(m_pVertexLayout_ShadowBuild);
		m_pDX10_Renderer->SetPrimitiveTopology(_obj->GetMesh()->GetPrimTopology());

		D3D10_TECHNIQUE_DESC techDesc;
		m_pTech_CreateShadow->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			D3DXMATRIX wvp = matWorldObject * matViewLight * *m_pDX10_Renderer->GetOrthographicMatrix();
			m_pMatViewLight->SetMatrix((float*)matViewLight);
			m_pMatWorldObject->SetMatrix((float*)matWorldObject);
			m_pMapDiffuse->SetResource(_obj->GetFirstTexture());

			m_pMatWVP->SetMatrix((float*)wvp);

			m_pTech_CreateShadow->GetPassByIndex(p)->Apply(0);

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
		VALIDATE(m_pDX10_Renderer->BuildFX("shadow.fx", "CreateShadowMapTech", m_pFX, m_pTech_CreateShadow));

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
		
		m_pMatProjLight = m_pFX->GetVariableByName("g_matProjLight")->AsMatrix();

		// Per Object	
		m_pMatWorldObject = m_pFX->GetVariableByName("g_matWorldObject")->AsMatrix();
		m_pMatViewLight = m_pFX->GetVariableByName("g_matViewLight")->AsMatrix();
		m_pMapDiffuse = m_pFX->GetVariableByName("g_mapDiffuse")->AsShaderResource();

		m_pMatWVP = m_pFX->GetVariableByName("g_matWVP")->AsMatrix();

		VALIDATE(m_pMatViewLight != 0);
		VALIDATE(m_pMatProjLight != 0);
		VALIDATE(m_pMatWorldObject != 0);
		VALIDATE(m_pMapDiffuse != 0);
		VALIDATE(m_pMatWVP != 0);
		

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

		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_CreateShadow, m_pVertexLayout_ShadowBuild);

		return true;
	}

private:

	ID3D10Effect* m_pFX;
	ID3D10InputLayout* m_pVertexLayout_ShadowBuild;
	ID3D10EffectTechnique* m_pTech_CreateShadow;

	DX10_Renderer*						m_pDX10_Renderer;

	ID3D10EffectMatrixVariable*			m_pMatViewLight;
	ID3D10EffectMatrixVariable*			m_pMatProjLight;
	ID3D10EffectMatrixVariable*			m_pMatWorldObject;

	ID3D10EffectShaderResourceVariable* m_pMapDiffuse;

	// TO DO CAL
	ID3D10EffectMatrixVariable*			m_pMatWVP;
};

#endif	// __DX10_SHADER_SHADOW_H__