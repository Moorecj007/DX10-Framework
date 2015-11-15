/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_Blur.h
* Description : Shader Instructions for a the Blur shader
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_BLUR_H__
#define __DX10_SHADER_BLUR_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

// TO DO CAL
enum eTech_Blur
{
	TECH_BLUR_HORIZONTAL,
	TECH_BLUR_VERTICAL
};

class DX10_Shader_Blur
{
public:

	/***********************
	* DX10_Shader_Blur: Default Constructor for Blur Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_Blur()
	{
	}

	/***********************
	* ~DX10_Shader_Blur: Default Destructor for Blur Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_Blur()
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
	// TO DO CAL
	* @return: void
	********************/
	void Render(ID3D10ShaderResourceView* _pSRV, DX10_Buffer* _pBuff, eTech_Blur _eTech)
	{	
		SetCurrentPtrs(_eTech);
		m_pDX10_Renderer->SetInputLayout(m_pVertexLayout_Current);

		D3D10_TECHNIQUE_DESC techDesc;
		m_pTech_Current->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			m_pTexture->SetResource(_pSRV);

			m_pTech_Current->GetPassByIndex(i)->Apply(0);
			_pBuff->Render();
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
		VALIDATE(m_pDX10_Renderer->BuildFX("Blur.fx", "BlurHorizontalTech", m_pFX, m_pTech_Horizontal));
		VALIDATE(m_pDX10_Renderer->BuildFX("Blur.fx", "BlurVerticalTech", m_pFX, m_pTech_Vertical));

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
		m_pTextureWidth = m_pFX->GetVariableByName("g_textureWidth")->AsScalar();
		m_pTextureHeight = m_pFX->GetVariableByName("g_textureHeight")->AsScalar();

		// Globals
		m_pTexture = m_pFX->GetVariableByName("g_texture")->AsShaderResource();

		VALIDATE(m_pTextureWidth != 0);
		VALIDATE(m_pTextureHeight != 0);
		VALIDATE(m_pTexture != 0);

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
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT elementNum = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Horizontal, m_pVertexLayout_Horizontal);
		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Vertical, m_pVertexLayout_Vertical);

		return true;
	}

	/***********************
	* SetCurrentPtrs: Set the Current Vertex Layout and Technique pointers
	* @author: Callan Moore
	* @parameter: _tech: Enumerator to determine which set of pointers to use as current
	* @return: void
	********************/
	void SetCurrentPtrs(eTech_Blur _tech)
	{
		switch (_tech)
		{
			case TECH_BLUR_HORIZONTAL:
			{
				m_pVertexLayout_Current = m_pVertexLayout_Horizontal;
				m_pTech_Current = m_pTech_Horizontal;
			}
			break;
			case TECH_BLUR_VERTICAL:
			{
				m_pVertexLayout_Current = m_pVertexLayout_Vertical;
				m_pTech_Current = m_pTech_Vertical;
			}
			break;
			default:
			{
				m_pVertexLayout_Current = 0;
				m_pTech_Current = 0;
			}
		}	// End Switch
	}

private:
	DX10_Renderer*						m_pDX10_Renderer;
	ID3D10Effect*						m_pFX;

	ID3D10InputLayout*					m_pVertexLayout_Current;
	ID3D10InputLayout*					m_pVertexLayout_Horizontal;
	ID3D10InputLayout*					m_pVertexLayout_Vertical;

	ID3D10EffectTechnique*				m_pTech_Current;
	ID3D10EffectTechnique*				m_pTech_Horizontal;
	ID3D10EffectTechnique*				m_pTech_Vertical;

	ID3D10EffectScalarVariable* m_pTextureWidth;
	ID3D10EffectScalarVariable* m_pTextureHeight;
	ID3D10EffectShaderResourceVariable* m_pTexture;
};

#endif	// __DX10_SHADER_BLUR_H__