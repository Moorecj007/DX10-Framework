/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_LitTex.cpp
* Description : Shader Instructions for a the LitTexture shader
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_LITTEX_H__
#define __DX10_SHADER_LITTEX_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

/***********************
* TLitTex: Shader Variables for the LitTex FX
* @author: Callan Moore
********************/
struct TLitTex
{
	DX10_Mesh* pMesh;
	D3DXMATRIX* pMatWorld;
	ID3D10ShaderResourceView* pTexBase;
	D3DXMATRIX* pTexTranslation;
	float transparency = 0.0f;
};

enum eTech_LitTex
{
	TECH_LITTEX_STANDARD,
	TECH_LITTEX_ANIMWATER
};

class DX10_Shader_LitTex
{
public:

	/***********************
	* DX10_Shader_LitTex: Default Constructor for Lit Texture Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_LitTex()
	{
	}

	/***********************
	* ~DX10_Shader_LitTex: Default Destructor for Lit Texture Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_LitTex()
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

		VALIDATE(m_pDX10_Renderer->CreateTexture("defaultSpecular.dds", m_pSpecularTex));

		return true;
	}
	
	/***********************
	* SetUpPerFrame: Setup the shader file with the variables used for the whole frame
	* @author: Callan Moore
	* @return: void
	********************/
	void SetUpPerFrame()
	{
		m_pLight->SetRawValue(m_pDX10_Renderer->GetActiveLight(), 0, sizeof(Light));
		m_pEyePos->SetRawValue(m_pDX10_Renderer->GetEyePos(), 0, sizeof(D3DXVECTOR3));
		m_pMatView->SetMatrix((float*)m_pDX10_Renderer->GetViewMatrix());
		m_pMatProj->SetMatrix((float*)m_pDX10_Renderer->GetProjMatrix());
	}
	
	/***********************
	* Render: Ready the shader technique with object specific details and setting the objects mesh to render
	* @author: Callan Moore
	* @parameter: _litTex: Structure containing all details for a litTex object
	* @parameter: _eTech: Technique Identifier to determine which technique to use
	* @return: void
	********************/
	void Render(TLitTex _litTex, eTech_LitTex _eTech)
	{
		// Reset draw states in case they're different
		m_pDX10_Renderer->RestoreDefaultDrawStates();
		SetCurrentPtrs(_eTech);

		// Set the Renderer Input layout and primitive topology to be the correct ones for this shader
		m_pDX10_Renderer->SetInputLayout(m_pCurrentVertexLayout);
		m_pDX10_Renderer->SetPrimitiveTopology(_litTex.pMesh->GetPrimTopology());

		D3DXMATRIX matTex;
		D3DXMatrixIdentity(&matTex);

		if (m_pCurrentTech != 0)
		{
			D3D10_TECHNIQUE_DESC techDesc;
			m_pCurrentTech->GetDesc(&techDesc);
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				m_pMatWorld->SetMatrix((float*)&_litTex.pMatWorld);
				m_pMatTex->SetMatrix((float*)&matTex);
				m_pMapDiffuse->SetResource(_litTex.pTexBase);
				m_pMapSpecular->SetResource(m_pSpecularTex);
				m_pTransparency->SetFloat(_litTex.transparency);

				m_pCurrentTech->GetPassByIndex(p)->Apply(0);
				_litTex.pMesh->Render();			
			}
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
		VALIDATE(m_pDX10_Renderer->BuildFX("litTex.fx", "StandardTech", m_pFX, m_pTech_Standard));
		VALIDATE(m_pDX10_Renderer->BuildFX("litTex.fx", "AnimateWaterTech", m_pFX, m_pTech_AnimWater));

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
		m_pLight = m_pFX->GetVariableByName("g_light");
		m_pEyePos = m_pFX->GetVariableByName("g_eyePosW");

		m_pMatView = m_pFX->GetVariableByName("g_matView")->AsMatrix();
		m_pMatProj = m_pFX->GetVariableByName("g_matProj")->AsMatrix();

		// Per Object
		m_pMatWorld = m_pFX->GetVariableByName("g_matWorld")->AsMatrix();
		m_pMatTex = m_pFX->GetVariableByName("g_matTex")->AsMatrix();
		m_pTransparency = m_pFX->GetVariableByName("g_transparency")->AsScalar();

		// Globals
		m_pMapDiffuse = m_pFX->GetVariableByName("g_mapDiffuse")->AsShaderResource();
		m_pMapSpecular = m_pFX->GetVariableByName("g_mapSpec")->AsShaderResource();

		VALIDATE(m_pLight != 0);
		VALIDATE(m_pEyePos != 0);
		VALIDATE(m_pMatView != 0);
		VALIDATE(m_pMatProj != 0);
		VALIDATE(m_pMatWorld != 0);
		VALIDATE(m_pMatTex != 0);
		VALIDATE(m_pTransparency != 0);
		VALIDATE(m_pMapDiffuse != 0);
		VALIDATE(m_pMapSpecular != 0);

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
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elementNum = sizeof(vertexDesc) / sizeof(vertexDesc[0]);
		
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Standard, m_pVertexLayout_Standard);
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_AnimWater, m_pVertexLayout_AnimWater);
	
		return true;
	}

	/***********************
	* SetCurrentPtrs: Set the Current Vertex Layout and Technique pointers
	* @author: Callan Moore
	* @parameter: _tech: Enumerator to determine which set of pointers to use as current
	* @return: void
	********************/
	void SetCurrentPtrs(eTech_LitTex _tech)
	{
		switch (_tech)
		{
			case TECH_LITTEX_STANDARD:
			{
				m_pCurrentVertexLayout = m_pVertexLayout_Standard;
				m_pCurrentTech = m_pTech_Standard;
			}
			break;
			case TECH_LITTEX_ANIMWATER:
			{
				m_pCurrentVertexLayout = m_pVertexLayout_AnimWater;
				m_pCurrentTech = m_pTech_AnimWater;
			}
			break;
			default:
			{
				m_pCurrentVertexLayout = 0;
				m_pCurrentTech = 0;
			}
		}	// End Switch
	}

private:

	ID3D10Effect* m_pFX;
	ID3D10ShaderResourceView* m_pSpecularTex;

	ID3D10InputLayout* m_pCurrentVertexLayout;
	ID3D10InputLayout* m_pVertexLayout_Standard;
	ID3D10InputLayout* m_pVertexLayout_AnimWater;

	ID3D10EffectTechnique* m_pCurrentTech;
	ID3D10EffectTechnique* m_pTech_Standard;
	ID3D10EffectTechnique* m_pTech_AnimWater;

	DX10_Renderer*						m_pDX10_Renderer;

	ID3D10EffectVariable*				m_pLight;
	ID3D10EffectVariable*				m_pEyePos;

	ID3D10EffectMatrixVariable*			m_pMatView;
	ID3D10EffectMatrixVariable*			m_pMatProj;
	ID3D10EffectMatrixVariable*			m_pMatWorld;
	ID3D10EffectMatrixVariable*			m_pMatTex;

	ID3D10EffectShaderResourceVariable* m_pMapDiffuse;
	ID3D10EffectShaderResourceVariable* m_pMapSpecular;

	ID3D10EffectScalarVariable*			m_pTransparency;
};

#endif	// __DX10_SHADER_LITTEX_H__