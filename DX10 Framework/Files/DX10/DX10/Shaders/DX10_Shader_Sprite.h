//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	DX10_Shader_Sprite.h
// Description	:	This wraps all the texture shader functions and variables
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_SPRITE_H__
#define __DX10_SHADER_SPRITE_H__

// Local Includes
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

class DX10_Shader_Sprite
{
	// Functions
	public:
		/***********************
		* DX10_Shader_Sprite: Default constructor for the Shader Sprite class.
		* @author: Juran Griffith.
		********************/
		DX10_Shader_Sprite()
		{
			m_pDX10_Renderer = 0;
			m_pFX = 0;
			m_pTech = 0;
			m_pVertexLayout = 0;

			m_pEmvView = 0;
			m_pEmvProjection = 0;
			m_pEmvWorld = 0;
			m_pEsrvTexture = 0;

			D3DXMatrixIdentity(&m_matWorld);
		}

		/***********************
		* ~DX10_Shader_Sprite: Default destructor for the Shader Sprite class.
		* @author: Juran Griffith.
		********************/
		~DX10_Shader_Sprite()
		{
			m_pEmvView = 0;
			m_pEmvProjection = 0;
			m_pEmvWorld = 0;
			m_pEsrvTexture = 0;
		}

		/*******************
		* Initialise: Intialises the shader sprite class.
		* @author: Juran Griffith.
		* @parameter: _pDX10_Renderer: The graphics device.
		* @parameter: _pHWnd: The handler to the window.
		* @return: bool: Returns true if the initialization succeeds otherwise the error on why it failed.
		********************/
		bool Initialise(DX10_Renderer* _pDX10_Renderer, HWND* _pHWnd)
		{
			m_pDX10_Renderer = _pDX10_Renderer;
			m_pHWnd = _pHWnd;
			m_pDX10_Renderer->BuildFX("Sprite.fx", "Render", m_pFX, m_pTech);
			
			// Get pointers to the three matrices inside the shader so we can update them from this class.
			m_pEmvWorld = m_pFX->GetVariableByName("World")->AsMatrix();
			m_pEmvView = m_pFX->GetVariableByName("View")->AsMatrix();
			m_pEmvProjection = m_pFX->GetVariableByName("Projection")->AsMatrix();		

			// Get the pointer to the time variable inside the shader
			m_pEvDeltaTime = m_pFX->GetVariableByName("DeltaTime");

			// Get pointer to the texture resource inside the shader.
			m_pEsrvTexture = m_pFX->GetVariableByName("Texture")->AsShaderResource();

			// Now setup the layout of the data that goes into the shader.
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
			};

			UINT layoutElements = sizeof(layout) / sizeof(layout[0]);

			m_pDX10_Renderer->CreateVertexLayout(layout, layoutElements, m_pTech, m_pVertexLayout);

			// Save the screen size.
			/*RECT rect;
			if (GetClientRect(*_pHWnd, &rect))
			{
				// Create an orthographic projection matrix for 2D rendering.
				D3DXMatrixOrthoLH(&m_matOrtho, static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top), 0.1f, 100.0f);
			}*/

			// Set the view matrix. No need for a camera.
			D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0.0f, 0.0f, -1.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

			return true;
		}

		/*******************
		* Update: This updates the shader files constant buffer variables for the effect to use when rendering.
		* @author: Juran Griffith.
		* @parameter: _deltaTime: The current delta time.
		* @return: void.
		********************/
		void Update(float _deltaTime)
		{
			// Update the constant buffer variables on the shader
			m_pEmvView->SetMatrix((float*)&m_matView);
			//m_pEmvProjection->SetMatrix((float*)&m_matOrtho);
			m_pEmvProjection->SetMatrix((float*)m_pDX10_Renderer->GetOrthographicMatrix());
			m_pEmvWorld->SetMatrix((float*)&m_matWorld);
			m_pEvDeltaTime->SetRawValue(&_deltaTime, 0, sizeof(float));
		}

		/*******************
		* Render: Draws the 2D texture.
		* @author: Juran Griffith.
		* @parameter: _pBuff: Buffer of the object.
		* @parameter: _pTex: Texture to be used.
		* @return: void.
		********************/
		void Render(DX10_Buffer* _pBuff, ID3D10ShaderResourceView* _pTex)
		{
			// Bind the texture.
			m_pEsrvTexture->SetResource(_pTex);

			// Set the input layout.
			m_pDX10_Renderer->SetInputLayout(m_pVertexLayout);

			if (m_pTech != NULL)
			{
				D3D10_TECHNIQUE_DESC techDesc;
				m_pTech->GetDesc(&techDesc);
				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					m_pTech->GetPassByIndex(p)->Apply(0);
					m_pDX10_Renderer->RenderBuffer(_pBuff);
				}
			}
		}

		/*******************
		* GetHWnd: Gets the handler to the current window.
		* @author: Juran Griffith.
		* @return: HWND*: The pointer for the handler to the window.
		********************/
		HWND GetHWnd()
		{
			return *m_pHWnd;
		}
	protected:
	private:

	// Variables
	public:
	protected:
	private:
		DX10_Renderer*						m_pDX10_Renderer;
		HWND*								m_pHWnd;

		ID3D10EffectMatrixVariable*			m_pEmvView;
		ID3D10EffectMatrixVariable*			m_pEmvProjection;
		ID3D10EffectMatrixVariable*			m_pEmvWorld;
		ID3D10EffectVariable*				m_pEvDeltaTime;
		ID3D10EffectShaderResourceVariable* m_pEsrvTexture;

		ID3D10Effect*						m_pFX;
		ID3D10EffectTechnique*				m_pTech;
		ID3D10InputLayout*					m_pVertexLayout;
		D3DXMATRIX							m_matWorld;
		D3DXMATRIX							m_matView;
		D3DXMATRIX							m_matOrtho;
};

#endif	// __DX10_SHADER_SPRITE_H__