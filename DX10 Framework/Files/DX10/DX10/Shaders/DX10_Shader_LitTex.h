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
	ID3D10ShaderResourceView* pTex2;
	float reduceAlpha = 0.0f;
	D3DXPLANE plane;
};

enum eTech_LitTex
{
	TECH_LITTEX_STANDARD,
	TECH_LITTEX_FADE,
	TECH_LITTEX_BLENDTEX2,
	TECH_LITTEX_REFLECT,
	TECH_LITTEX_REFRACT
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
		SetCurrentPtrs(_eTech);

		// Set the Renderer Input layout and primitive topology to be the correct ones for this shader
		m_pDX10_Renderer->SetInputLayout(m_pCurrentVertexLayout);
		m_pDX10_Renderer->SetPrimitiveTopology(_litTex.pMesh->GetPrimTopology());

		// Don't transform texture coordinates
		D3DXMATRIX matTex;
		D3DXMatrixIdentity(&matTex);

		if (m_pCurrentTech != NULL)
		{
			D3D10_TECHNIQUE_DESC techDesc;
			m_pCurrentTech->GetDesc(&techDesc);
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				D3DXMATRIX matWorld = *_litTex.pMatWorld;

				m_pLight->SetRawValue(m_pDX10_Renderer->GetActiveLight(), 0, sizeof(Light));
				m_pMatWorld->SetMatrix((float*)&matWorld);
				m_pMatTex->SetMatrix((float*)&matTex);
				m_pMapDiffuse->SetResource(_litTex.pTexBase);
				m_pMapSpecular->SetResource(m_pSpecularTex);
				m_pReduceAlpha->SetRawValue(&_litTex.reduceAlpha, 0, sizeof(float));
				
				if (_eTech == TECH_LITTEX_REFLECT)
				{
					m_pDX10_Renderer->ApplyReflectionStates();
					// TO DO CAL
					D3DXMATRIX matReflection = CreateReflectionMatrix(_litTex.plane);
					D3DXVECTOR3 reflectedCamPos;
					D3DXVec3TransformCoord(&reflectedCamPos, &m_pDX10_Renderer->GetEyePos(), &matReflection);

					D3DXMATRIX matReflectedView;
					D3DXMatrixMultiply(&matReflectedView, &matReflection, m_pDX10_Renderer->GetViewMatrix());

					m_pEyePos->SetRawValue(reflectedCamPos, 0, sizeof(D3DXVECTOR3));
					m_pMatView->SetMatrix((float*)matReflectedView);
					m_pPlane->SetRawValue((float*)&_litTex.plane, 0, sizeof(D3DXPLANE));
				}
				if (_eTech == TECH_LITTEX_BLENDTEX2)
				{
					m_pMapDiffuse2->SetResource(_litTex.pTex2);
				}
				if (_eTech == TECH_LITTEX_REFRACT)
				{
					m_pPlane->SetRawValue((float*)&_litTex.plane, 0, sizeof(D3DXPLANE));
				}

				m_pCurrentTech->GetPassByIndex(p)->Apply(0);
				_litTex.pMesh->Render();			
			}
		}
		m_pDX10_Renderer->RestoreDefaultDrawStates();
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
		VALIDATE(m_pDX10_Renderer->BuildFX("litTex.fx", "FadeTech", m_pFX, m_pTech_Fade));
		VALIDATE(m_pDX10_Renderer->BuildFX("litTex.fx", "BlendTex2Tech", m_pFX, m_pTech_BlendTex2));
		VALIDATE(m_pDX10_Renderer->BuildFX("litTex.fx", "ReflectionTech", m_pFX, m_pTech_Reflect));
		VALIDATE(m_pDX10_Renderer->BuildFX("litTex.fx", "RefractionTech", m_pFX, m_pTech_Refract));

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
		m_pReduceAlpha = m_pFX->GetVariableByName("g_reduceAlpha")->AsScalar();
		m_pPlane = m_pFX->GetVariableByName("g_plane");

		// Globals
		m_pMapDiffuse = m_pFX->GetVariableByName("g_mapDiffuse")->AsShaderResource();
		m_pMapDiffuse2 = m_pFX->GetVariableByName("g_mapDiffuse2")->AsShaderResource();
		m_pMapSpecular = m_pFX->GetVariableByName("g_mapSpec")->AsShaderResource();

		VALIDATE(m_pLight != 0);
		VALIDATE(m_pEyePos != 0);
		VALIDATE(m_pMatView != 0);
		VALIDATE(m_pMatProj != 0);
		VALIDATE(m_pMatWorld != 0);
		VALIDATE(m_pMatTex != 0);
		VALIDATE(m_pReduceAlpha != 0);
		VALIDATE(m_pPlane != 0);
		VALIDATE(m_pMapDiffuse != 0);
		VALIDATE(m_pMapDiffuse2 != 0);
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
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Fade, m_pVertexLayout_Fade);
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_BlendTex2, m_pVertexLayout_BlendTex2);
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Reflect, m_pVertexLayout_Reflect);
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Refract, m_pVertexLayout_Refract);
	
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
			case TECH_LITTEX_FADE:
			{
				m_pCurrentVertexLayout = m_pVertexLayout_Fade;
				m_pCurrentTech = m_pTech_Fade;
			}
			break;
			case TECH_LITTEX_BLENDTEX2:
			{
				m_pCurrentVertexLayout = m_pVertexLayout_BlendTex2;
				m_pCurrentTech = m_pTech_BlendTex2;
			}
			break;
			case TECH_LITTEX_REFLECT:
			{
				m_pCurrentVertexLayout = m_pVertexLayout_Reflect;
				m_pCurrentTech = m_pTech_Reflect;
			}
			break;
			case TECH_LITTEX_REFRACT:
			{
				m_pCurrentVertexLayout = m_pVertexLayout_Refract;
				m_pCurrentTech = m_pTech_Refract;
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
	ID3D10InputLayout* m_pVertexLayout_Fade;
	ID3D10InputLayout* m_pVertexLayout_BlendTex2;
	ID3D10InputLayout* m_pVertexLayout_Reflect;
	ID3D10InputLayout* m_pVertexLayout_Refract;

	ID3D10EffectTechnique* m_pCurrentTech;
	ID3D10EffectTechnique* m_pTech_Standard;
	ID3D10EffectTechnique* m_pTech_Fade;
	ID3D10EffectTechnique* m_pTech_BlendTex2;
	ID3D10EffectTechnique* m_pTech_Reflect;
	ID3D10EffectTechnique* m_pTech_Refract;

	DX10_Renderer*						m_pDX10_Renderer;

	ID3D10EffectVariable*				m_pLight;
	ID3D10EffectVariable*				m_pEyePos;
	ID3D10EffectMatrixVariable*			m_pMatView;
	ID3D10EffectMatrixVariable*			m_pMatProj;

	ID3D10EffectMatrixVariable*			m_pMatWorld;
	ID3D10EffectMatrixVariable*			m_pMatTex;
	ID3D10EffectVariable*				m_pReduceAlpha;
	ID3D10EffectVariable*				m_pPlane;

	ID3D10EffectShaderResourceVariable* m_pMapDiffuse;
	ID3D10EffectShaderResourceVariable* m_pMapDiffuse2;
	ID3D10EffectShaderResourceVariable* m_pMapSpecular;
};

#endif	// __DX10_SHADER_LITTEX_H__