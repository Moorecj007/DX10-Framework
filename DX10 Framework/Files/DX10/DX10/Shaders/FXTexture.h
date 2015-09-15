//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	FXTexture.h
// Description	:	This wraps all the texture shader functions and variables
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

// Inclusion Guards
#pragma once
#ifndef __FX_TEXTURE_H__
#define __FX_TEXTURE_H__

// Local Includes
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

class FXTexture
{
	// Functions
	public:
		/*******************
		-> Default constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		FXTexture()
		{
			m_pDX10_Renderer = 0;
			m_fxID = 0;
			m_techID = 0;
			m_vertexLayoutID = 0;

			m_pEmvView = 0;
			m_pEmvProjection = 0;
			m_pEmvWorld = 0;
			m_pEsrvTexture = 0;
		}

		/*******************
		-> Copy constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		//FXTexture(const FXTexture&);

		/*******************
		-> Default destructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		~FXTexture()
		{
			m_pEmvView = 0;
			m_pEmvProjection = 0;
			m_pEmvWorld = 0;
			m_pEsrvTexture = 0;
		}

		/*******************
		-> Intializes the object
		@author:	Juran Griffith.
		@parameter: _pD3DDevice	- The graphics device.
		@parameter: _pHWnd		- The handler to the window.
		@return:	bool		- Returns true if the initialization succeeds otherwise the error on why it failed.
		********************/
		bool Initialize(DX10_Renderer*_pDX10_Renderer, HWND* _pHWnd)
		{
			m_pDX10_Renderer = _pDX10_Renderer;
			m_pDX10_Renderer->BuildFX("Sprite", "Render", &m_fxID, &m_techID);
			
			// Get pointers to the three matrices inside the shader so we can update them from this class.
			m_pEmvWorld = m_pDX10_Renderer->GetFXVariable(m_fxID, "World")->AsMatrix();
			m_pEmvView = m_pDX10_Renderer->GetFXVariable(m_fxID, "View")->AsMatrix();
			m_pEmvProjection = m_pDX10_Renderer->GetFXVariable(m_fxID, "Projection")->AsMatrix();		

			// Get the pointer to the time variable inside the shader
			m_pEvDeltaTime = m_pDX10_Renderer->GetFXVariable(m_fxID, "DeltaTime");

			// Get pointer to the texture resource inside the shader.
			m_pEsrvTexture = m_pDX10_Renderer->GetFXVariable(m_fxID, "Texture")->AsShaderResource();

			// Now setup the layout of the data that goes into the shader.
			D3D10_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
			};

			UINT layoutElements = sizeof(layout) / sizeof(layout[0]);

			m_pDX10_Renderer->CreateVertexLayout(layout, layoutElements, m_techID, &m_vertexLayoutID);

			return true;
		}

		/*******************
		-> This updates the world, view and projection constant buffer variables on the graphics for this effect. 
		@author:	Juran Griffith.
		@parameter: _worldMatrix		- The world matrix of the application.
		@parameter: _viewMatrix			- The view matrix (which camera).
		@parameter: _projectionMatrix	- The projection matrix.
		@parameter: _delta				- The current delta time.
		@return:	void
		********************/
		void Update(D3DXMATRIX _worldMatrix, D3DXMATRIX _viewMatrix, D3DXMATRIX _projectionMatrix, float _delta)
		{
			// Set the world matrix variable inside the shader.
			m_pEmvWorld->SetMatrix((float*)&_worldMatrix);

			// Set the view matrix variable inside the shader.
			m_pEmvView->SetMatrix((float*)&_viewMatrix);

			// Set the projection matrix variable inside the shader.
			m_pEmvProjection->SetMatrix((float*)&_projectionMatrix);

			// Set the delta time inside the shader
			m_pEvDeltaTime->SetRawValue(&_delta, 0, sizeof(float));
		}

		/*******************
		-> Draws the 2D texture
		@author:	Juran Griffith.
		@parameter: _buffID	- Buffer ID of the object
		@parameter: _texID	- Texture ID for the texture to be used
		@return:	void
		********************/
		void Render(UINT _buffID, UINT _texID)
		{
			// Bind the texture.
			m_pEsrvTexture->SetResource(m_pDX10_Renderer->GetTexture(_texID));

			// Set the input layout.
			m_pDX10_Renderer->SetInputLayout(m_vertexLayoutID);

			// Retrieve the Technique
			ID3D10EffectTechnique* pTech = m_pDX10_Renderer->GetTechnique(m_techID);

			if (pTech != NULL)
			{
				D3D10_TECHNIQUE_DESC techDesc;
				pTech->GetDesc(&techDesc);
				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					pTech->GetPassByIndex(p)->Apply(0);
					m_pDX10_Renderer->RenderSprite(_buffID);
				}
			}
		}

	protected:
	private:

	// Variables
	public:
	protected:
	private:
		DX10_Renderer*						m_pDX10_Renderer;

		ID3D10EffectMatrixVariable*			m_pEmvView;
		ID3D10EffectMatrixVariable*			m_pEmvProjection;
		ID3D10EffectMatrixVariable*			m_pEmvWorld;
		ID3D10EffectVariable*				m_pEvDeltaTime;
		ID3D10EffectShaderResourceVariable* m_pEsrvTexture;

		UINT m_fxID;
		UINT m_techID;
		UINT m_vertexLayoutID;
};

#endif