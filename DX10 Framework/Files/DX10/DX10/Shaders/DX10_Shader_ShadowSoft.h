/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_ShadowSoft.h
* Description : Shader Instructions for a the Shadow shader
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_SHADOWSOFT_H__
#define __DX10_SHADER_SHADOWSOFT_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"
#include "../3D Objects/DX10_Obj_Generic.h"

class DX10_Shader_ShadowSoft
{
public:

	/***********************
	* DX10_Shader_ShadowSoft: Default Constructor for Soft Shadow Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_ShadowSoft()
	{
	}

	/***********************
	* ~DX10_Shader_ShadowSoft: Default Destructor for Soft Shadow Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_ShadowSoft()
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

		//VALIDATE(m_pDX10_Renderer->CreateTexture("defaultSpecular.dds", m_pSpecularTex));

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
	// TO DO CAL
	* @return: void
	********************/
	void Render(DX10_Obj_Generic* _pObj, DX10_ShadowMap* _pShadowMap1 , DX10_ShadowMap* _pShadowMap2)
	{
		m_pDX10_Renderer->SetInputLayout(m_pVertexLayout_Standard);

		D3D10_TECHNIQUE_DESC techDesc;
		m_pTech_Standard->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			D3DXMATRIX matWorld_Obj = _pObj->GetWorldMatrix();
			m_pMatWorld->SetMatrix((float*)&matWorld_Obj);
			m_pMatView->SetMatrix((float*)m_pDX10_Renderer->GetViewMatrix());
			m_pMatProj->SetMatrix((float*)m_pDX10_Renderer->GetProjMatrix());

			m_pMapDiffuse->SetResource(_pObj->GetTexture());
			m_pShadowTex1->SetResource(_pShadowMap1->GetBlurTex_UpSized()->GetShaderResourceView());
			m_pShadowTex2->SetResource(_pShadowMap2->GetBlurTex_UpSized()->GetShaderResourceView());
			
			int lightCount = m_pDX10_Renderer->GetLightCount();
			m_pLights->SetRawValue(m_pDX10_Renderer->GetActiveLights(), 0, lightCount * sizeof(TLight));
			m_pLightCount->SetInt(lightCount);

			m_pTech_Standard->GetPassByIndex(i)->Apply(0);
			_pObj->GetMesh()->Render();

			m_pShadowTex1->SetResource(NULL);
			m_pShadowTex2->SetResource(NULL);
			m_pTech_Standard->GetPassByIndex(i)->Apply(0);
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
		VALIDATE(m_pDX10_Renderer->BuildFX("ShadowSoft.fx", "SoftShadowTech", m_pFX, m_pTech_Standard));

		return true;
	}

	/***********************
	* CreateFXVarPointers: Create the pointers to the FX memory on the graphics card
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool CreateFXVarPointers()
	{		
		// Per Object
		m_pMatWorld = m_pFX->GetVariableByName("worldMatrix")->AsMatrix();
		m_pMatView = m_pFX->GetVariableByName("viewMatrix")->AsMatrix();
		m_pMatProj = m_pFX->GetVariableByName("projectionMatrix")->AsMatrix();

		// Globals
		m_pMapDiffuse = m_pFX->GetVariableByName("shaderTexture")->AsShaderResource();
		m_pShadowTex1 = m_pFX->GetVariableByName("shadowTexture1")->AsShaderResource();
		m_pShadowTex2 = m_pFX->GetVariableByName("shadowTexture2")->AsShaderResource();
		m_pLights = m_pFX->GetVariableByName("g_lights");
		m_pLightCount = m_pFX->GetVariableByName("g_lightCount")->AsScalar();

		VALIDATE(m_pMatWorld != 0);
		VALIDATE(m_pMatView != 0);
		VALIDATE(m_pMatProj != 0);

		VALIDATE(m_pMapDiffuse != 0);
		VALIDATE(m_pShadowTex1 != 0);
		VALIDATE(m_pShadowTex2 != 0);

		VALIDATE(m_pLights != 0);
		VALIDATE(m_pLightCount != 0);

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
	ID3D10Effect*						m_pFX;
	ID3D10ShaderResourceView*			m_pSpecularTex;

	ID3D10InputLayout*					m_pVertexLayout_Standard;
	ID3D10EffectTechnique*				m_pTech_Standard;

	ID3D10EffectMatrixVariable* m_pMatWorld;
	ID3D10EffectMatrixVariable* m_pMatView;
	ID3D10EffectMatrixVariable* m_pMatProj;

	ID3D10EffectShaderResourceVariable* m_pMapDiffuse;
	ID3D10EffectShaderResourceVariable* m_pShadowTex1;
	ID3D10EffectShaderResourceVariable* m_pShadowTex2;

	ID3D10EffectVariable* m_pLights;
	ID3D10EffectScalarVariable* m_pLightCount;
};

#endif	// __DX10_SHADER_SHADOWSOFT_H__