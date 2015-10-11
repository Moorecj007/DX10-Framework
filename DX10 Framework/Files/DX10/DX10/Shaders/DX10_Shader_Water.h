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
#ifndef __DX10_SHADER_WATER_H__
#define __DX10_SHADER_WATER_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

/***********************
* TWater: Shader Variables for the Water FX
* @author: Callan Moore
********************/
struct TWater
{
	DX10_Mesh* pMesh;
	D3DXMATRIX* pMatWorld;
	ID3D10ShaderResourceView* pTexNormal;
	ID3D10ShaderResourceView* pTexRefraction;
	ID3D10ShaderResourceView* pTexReflection;
	D3DXVECTOR2 texTranslation;
	float rippleScale = 0.01f;
};

/***********************
* eTech_Water: Enum for the different Techniques available for this shader
* @author: Callan Moore
********************/
enum eTech_Water
{
	TECH_WATER_STANDARD
};

class DX10_Shader_Water
{
public:

	/***********************
	* DX10_Shader_Water: Default Constructor for Water Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_Water()
	{
	}

	/***********************
	* ~DX10_Shader_Water: Default Destructor for Water Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_Water()
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
		m_pMatProj->SetMatrix((float*)m_pDX10_Renderer->GetProjMatrix());
	}

	/***********************
	* Render: Ready the shader technique with object specific details and setting the objects mesh to render
	* @author: Callan Moore
	* @parameter: _litTex: Structure containing all details for a object using the Water Shader
	* @parameter: _eTech: Technique Identifier to determine which technique to use
	* @return: void
	********************/
	void Render(TWater _water, eTech_Water _eTech)
	{
		// Load the correct Technique onto the Current pointers
		SetCurrentPtrs(_eTech);

		// Set the Renderer Input layout and primitive topology to be the correct ones for this shader
		m_pDX10_Renderer->SetInputLayout(m_pCurrentVertexLayout);
		m_pDX10_Renderer->SetPrimitiveTopology(_water.pMesh->GetPrimTopology());

		if (m_pCurrentTech != NULL)
		{
			// Get the Current Technique and loop through each pass
			D3D10_TECHNIQUE_DESC techDesc;
			m_pCurrentTech->GetDesc(&techDesc);
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				D3DXMATRIX matWorld = *_water.pMatWorld;
				D3DXMATRIX matView = *m_pDX10_Renderer->GetViewMatrix();

				// Prepare the Input Variables to be sent to the GPU
				m_pMatWorld->SetMatrix((float*)&matWorld);
				m_pMatView->SetMatrix((float*)&matView);
				m_pTexTranslation->SetFloatVector((float*)&_water.texTranslation);
				m_pRippleScale->SetFloat(_water.rippleScale);
				m_pMapNormal->SetResource(_water.pTexNormal);
				m_pMapRefraction->SetResource(_water.pTexRefraction);
				m_pMapReflection->SetResource(_water.pTexReflection);

				// Apply the variables and send them to the GPU
				m_pCurrentTech->GetPassByIndex(p)->Apply(0);

				// Render the Objects Mesh using the loaded technique
				_water.pMesh->Render();

				// Remove the Texture Resources from being Inputs so there are no read errors
				m_pMapRefraction->SetResource(NULL);
				m_pMapReflection->SetResource(NULL);
				m_pCurrentTech->GetPassByIndex(p)->Apply(0);
			}
		}

		// Restore the Default Rendering States in case any were changed
		m_pDX10_Renderer->RestoreDefaultRenderStates();
	}

private:

	/***********************
	* BuildFX: Build the FX file for the Shader
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool BuildFX()
	{
		VALIDATE(m_pDX10_Renderer->BuildFX("Water.fx", "StandardTech", m_pFX, m_pTech_Standard));

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
		m_pMatProj = m_pFX->GetVariableByName("g_matProj")->AsMatrix();

		// Per Object
		m_pMatWorld = m_pFX->GetVariableByName("g_matWorld")->AsMatrix();
		m_pMatView = m_pFX->GetVariableByName("g_matView")->AsMatrix();
		m_pTexTranslation = m_pFX->GetVariableByName("g_texTranslation")->AsVector();
		m_pRippleScale = m_pFX->GetVariableByName("g_rippleScale")->AsScalar();

		// Globals
		m_pMapNormal = m_pFX->GetVariableByName("g_mapNormal")->AsShaderResource();
		m_pMapRefraction = m_pFX->GetVariableByName("g_mapRefractTex")->AsShaderResource();
		m_pMapReflection = m_pFX->GetVariableByName("g_mapReflectTex")->AsShaderResource();
	
		VALIDATE(m_pMatProj != 0);

		VALIDATE(m_pMatWorld != 0);
		VALIDATE(m_pMatView != 0);
		VALIDATE(m_pTexTranslation != 0);
		VALIDATE(m_pRippleScale != 0);

		VALIDATE(m_pMapNormal != 0);
		VALIDATE(m_pMapRefraction != 0);
		VALIDATE(m_pMapReflection != 0);

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
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elementNum = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Standard, m_pVertexLayout_Standard);

		return true;
	}

	/***********************
	* SetCurrentPtrs: Set the Current Vertex Layout and Technique pointers
	* @author: Callan Moore
	* @parameter: _tech: Enumerator to determine which set of pointers to use as current
	* @return: void
	********************/
	void SetCurrentPtrs(eTech_Water _tech)
	{
		switch (_tech)
		{
		case TECH_WATER_STANDARD:
		{
			m_pCurrentVertexLayout = m_pVertexLayout_Standard;
			m_pCurrentTech = m_pTech_Standard;
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

	DX10_Renderer*m_pDX10_Renderer;

	ID3D10Effect* m_pFX;

	ID3D10InputLayout* m_pCurrentVertexLayout;
	ID3D10InputLayout* m_pVertexLayout_Standard;

	ID3D10EffectTechnique* m_pCurrentTech;
	ID3D10EffectTechnique* m_pTech_Standard;

	ID3D10EffectMatrixVariable*			m_pMatProj;

	ID3D10EffectMatrixVariable*			m_pMatWorld;
	ID3D10EffectMatrixVariable*			m_pMatView;
	ID3D10EffectVectorVariable*			m_pTexTranslation;
	ID3D10EffectScalarVariable*			m_pRippleScale;

	ID3D10EffectShaderResourceVariable* m_pMapNormal;
	ID3D10EffectShaderResourceVariable* m_pMapReflection;
	ID3D10EffectShaderResourceVariable* m_pMapRefraction;

	
};

#endif	// __DX10_SHADER_WATER_H__