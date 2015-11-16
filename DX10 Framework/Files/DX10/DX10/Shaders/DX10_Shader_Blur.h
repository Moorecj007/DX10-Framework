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
	void Render(DX10_ShadowMap* _pShadowMap)
	{	
		D3D10_TECHNIQUE_DESC techDesc;
		m_pDX10_Renderer->SetInputLayout(m_pVertexLayout_All);
		m_pDX10_Renderer->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pTech_Resize->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			// Resize image to half the size
			_pShadowMap->GetBlurTex_DownSized()->SetRenderTarget();
			_pShadowMap->GetBlurTex_DownSized()->ClearRenderTarget(0, 0, 0, 1);
			m_pMatWorld->SetMatrix((float*)&_pShadowMap->GetBlurTex_DownSized()->GetWorldMatrix());
			m_pMatView->SetMatrix((float*)&_pShadowMap->GetBlurTex_DownSized()->GetViewMatrix());
			m_pMatProj->SetMatrix((float*)&_pShadowMap->GetBlurTex_DownSized()->GetOrthoMatrix());

			m_pTexture->SetResource(_pShadowMap->GetShadowedTex()->GetShaderResourceView());

			m_pTech_Resize->GetPassByIndex(i)->Apply(0);
			_pShadowMap->GetHalfWindow()->pBuff->Render();

			m_pTexture->SetResource(NULL);
			m_pTech_Resize->GetPassByIndex(i)->Apply(0);
		}

		m_pTech_Horizontal->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			// Horizontal Blurring
			_pShadowMap->GetBlurTex_Horizontal()->SetRenderTarget();
			_pShadowMap->GetBlurTex_Horizontal()->ClearRenderTarget(0, 0, 0, 1);
			m_pMatWorld->SetMatrix((float*)&_pShadowMap->GetBlurTex_Horizontal()->GetWorldMatrix());
			m_pMatView->SetMatrix((float*)&_pShadowMap->GetBlurTex_Horizontal()->GetViewMatrix());
			m_pMatProj->SetMatrix((float*)&_pShadowMap->GetBlurTex_Horizontal()->GetOrthoMatrix());

			m_pTexture->SetResource(_pShadowMap->GetBlurTex_DownSized()->GetShaderResourceView());

			float width = (float)_pShadowMap->GetWidth() / _pShadowMap->GetDownsizeScalar();
			m_pTextureWidth->SetFloat(width);

			m_pTech_Horizontal->GetPassByIndex(i)->Apply(0);
			_pShadowMap->GetHalfWindow()->pBuff->Render();

			m_pTexture->SetResource(NULL);
			m_pTech_Horizontal->GetPassByIndex(i)->Apply(0);
		}

		m_pTech_Vertical->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			// Vertical Vertical
			_pShadowMap->GetBlurTex_Vertical()->SetRenderTarget();
			_pShadowMap->GetBlurTex_Vertical()->ClearRenderTarget(0, 0, 0, 1);
			m_pMatWorld->SetMatrix((float*)&_pShadowMap->GetBlurTex_Vertical()->GetWorldMatrix());
			m_pMatView->SetMatrix((float*)&_pShadowMap->GetBlurTex_Vertical()->GetViewMatrix());
			m_pMatProj->SetMatrix((float*)&_pShadowMap->GetBlurTex_Vertical()->GetOrthoMatrix());

			m_pTexture->SetResource(_pShadowMap->GetBlurTex_Horizontal()->GetShaderResourceView());

			float height = (float)_pShadowMap->GetHeight() / _pShadowMap->GetDownsizeScalar();
			m_pTextureHeight->SetFloat(height);

			m_pTech_Vertical->GetPassByIndex(i)->Apply(0);
			_pShadowMap->GetHalfWindow()->pBuff->Render();

			m_pTexture->SetResource(NULL);
			m_pTech_Vertical->GetPassByIndex(i)->Apply(0);
		}

		m_pTech_Resize->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			// Resize back to original size
			_pShadowMap->GetBlurTex_UpSized()->SetRenderTarget();
			_pShadowMap->GetBlurTex_UpSized()->ClearRenderTarget(0, 0, 0, 1);
			m_pMatWorld->SetMatrix((float*)&_pShadowMap->GetBlurTex_UpSized()->GetWorldMatrix());
			m_pMatView->SetMatrix((float*)&_pShadowMap->GetBlurTex_UpSized()->GetViewMatrix());
			m_pMatProj->SetMatrix((float*)&_pShadowMap->GetBlurTex_UpSized()->GetOrthoMatrix());

			m_pTexture->SetResource(_pShadowMap->GetBlurTex_Vertical()->GetShaderResourceView());

			m_pTech_Resize->GetPassByIndex(i)->Apply(0);
			_pShadowMap->GetScreenWindow()->pBuff->Render();

			m_pTexture->SetResource(NULL);
			m_pTech_Resize->GetPassByIndex(i)->Apply(0);
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
		VALIDATE(m_pDX10_Renderer->BuildFX("Blur.fx", "ResizeTech", m_pFX, m_pTech_Resize));

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

		m_pTextureWidth = m_pFX->GetVariableByName("screenWidth")->AsScalar();
		m_pTextureHeight = m_pFX->GetVariableByName("screenHeight")->AsScalar();

		// Globals
		m_pTexture = m_pFX->GetVariableByName("shaderTexture")->AsShaderResource();

		VALIDATE(m_pMatWorld != 0);
		VALIDATE(m_pMatView != 0);
		VALIDATE(m_pMatProj != 0);

		VALIDATE(m_pTexture != 0);
		VALIDATE(m_pTextureWidth != 0);
		VALIDATE(m_pTextureHeight != 0);
		

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
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT elementNum = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Horizontal, m_pVertexLayout_All);

		return true;
	}

private:
	DX10_Renderer*						m_pDX10_Renderer;
	ID3D10Effect*						m_pFX;

	ID3D10InputLayout*					m_pVertexLayout_All;

	ID3D10EffectTechnique*				m_pTech_Horizontal;
	ID3D10EffectTechnique*				m_pTech_Vertical;
	ID3D10EffectTechnique*				m_pTech_Resize;

	ID3D10EffectMatrixVariable* m_pMatWorld;
	ID3D10EffectMatrixVariable* m_pMatView;
	ID3D10EffectMatrixVariable* m_pMatProj;

	ID3D10EffectShaderResourceVariable* m_pTexture;

	ID3D10EffectScalarVariable* m_pTextureWidth;
	ID3D10EffectScalarVariable* m_pTextureHeight;
};

#endif	// __DX10_SHADER_BLUR_H__