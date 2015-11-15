/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_Shadow.h
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
#include "../3D Objects/DX10_Obj_Generic.h"


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

	}

	/***********************
	* Render: Ready the shader technique with object specific details and setting the objects mesh to render
	* @author: Callan Moore
	// TO DO CAL
	* @return: void
	********************/
	void Render(DX10_Obj_Generic* _pObj, D3DXMATRIX _matLightView, DX10_ShadowMap* _pShadowMap)
	{
		int lightCount = m_pDX10_Renderer->GetLightCount();

		m_pEyePos->SetRawValue(m_pDX10_Renderer->GetEyePos(), 0, sizeof(D3DXVECTOR3));		
		m_pLights->SetRawValue(m_pDX10_Renderer->GetActiveLights(), 0, lightCount * sizeof(TLight));
		m_pShadowMap->SetResource(_pShadowMap->GetShaderResourceView());

		// Don't transform texture coordinates
		D3DXMATRIX matTex;
		D3DXMatrixIdentity(&matTex);

		D3D10_TECHNIQUE_DESC techDesc;
		m_pTech_Standard->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			D3DXMATRIX matWorld_Obj = _pObj->GetWorldMatrix();
			m_pMatWVP_Object->SetMatrix((float*)&(matWorld_Obj * *m_pDX10_Renderer->GetViewMatrix() * *m_pDX10_Renderer->GetProjMatrix()));
			m_pMatWVP_Light->SetMatrix((float*)&(matWorld_Obj * _matLightView * m_pDX10_Renderer->GetShadowProjMatrix()));
			m_pMatWorld->SetMatrix((float*)&matWorld_Obj);
			m_pMatTex->SetMatrix((float*)&matTex);
			m_pMapDiffuse->SetResource(_pObj->GetTexture());
			m_pMapSpec->SetResource(m_pSpecularTex);

			m_pTech_Standard->GetPassByIndex(i)->Apply(0);
			_pObj->GetMesh()->Render();
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
		VALIDATE(m_pDX10_Renderer->BuildFX("Shadow.fx", "ShadowTech", m_pFX, m_pTech_Standard));

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
		m_pLights  = m_pFX->GetVariableByName("gLight");;
		m_pEyePos  = m_pFX->GetVariableByName("gEyePosW");;
		
		// Per Object
		m_pMatWVP_Light = m_pFX->GetVariableByName("gLightWVP")->AsMatrix();
		m_pMatWVP_Object = m_pFX->GetVariableByName("gWVP")->AsMatrix();
		m_pMatWorld = m_pFX->GetVariableByName("gWorld")->AsMatrix();
		m_pMatTex = m_pFX->GetVariableByName("gTexMtx")->AsMatrix();

		// Globals
		m_pMapDiffuse = m_pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
		m_pMapSpec = m_pFX->GetVariableByName("gSpecMap")->AsShaderResource();
		m_pShadowMap = m_pFX->GetVariableByName("gShadowMap")->AsShaderResource();

		VALIDATE(m_pLights != 0);
		VALIDATE(m_pEyePos != 0);

		VALIDATE(m_pMatWVP_Light != 0);
		VALIDATE(m_pMatWVP_Object != 0);
		VALIDATE(m_pMatWorld != 0);
		VALIDATE(m_pMatTex != 0);

		VALIDATE(m_pMapDiffuse != 0);
		VALIDATE(m_pMapSpec != 0);
		VALIDATE(m_pShadowMap != 0);

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

	//ID3D10EffectMatrixVariable*			m_pMatWVP;
	//ID3D10EffectShaderResourceVariable* m_pMapDiffuse;

	ID3D10EffectVariable* m_pLights;
	ID3D10EffectVariable* m_pEyePos;
	ID3D10EffectMatrixVariable* m_pMatWVP_Light;
	ID3D10EffectMatrixVariable* m_pMatWVP_Object;
	ID3D10EffectMatrixVariable* m_pMatWorld;
	ID3D10EffectMatrixVariable* m_pMatTex;
	//ID3D10EffectVectorVariable* mfxReflectMtrlVar;
	//ID3D10EffectScalarVariable* mfxCubeMapEnabledVar;
	ID3D10EffectShaderResourceVariable* m_pMapDiffuse;
	ID3D10EffectShaderResourceVariable* m_pMapSpec;
	//ID3D10EffectShaderResourceVariable* mfxNormalMapVar;
	ID3D10EffectShaderResourceVariable* m_pShadowMap;
	//ID3D10EffectShaderResourceVariable* mfxCubeMapVar;
};

#endif	// __DX10_SHADER_SHADOW_H__