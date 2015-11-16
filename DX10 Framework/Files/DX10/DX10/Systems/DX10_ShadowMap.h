/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_ShadowMap.h
* Description : Shadow Map for shadowing techniques
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADOWMAP_H__
#define __DX10_SHADOWMAP_H__

// Local Includes
#include "../DX10_Utilities.h"
#include "../DX10_Vertex.h"
#include "DX10_Buffer.h"
#include "TextureResource.h"

// TO DO CAL
struct TWindow
{
	int vertexCount;
	int indexCount;
	DX10_Buffer* pBuff;

	TWindow(){};
	~TWindow()
	{
		ReleasePtr(pBuff);
	}

	bool Initialise(ID3D10Device* _pDevice, float _width, float _height)
	{
		// Set the number of vertices in the vertex array.
		vertexCount = 6;
		indexCount = vertexCount;

		// Create the vertex array.
		TVertexUV* pVertices = new TVertexUV[vertexCount];
		// Create the index array.
		DWORD* pIndices = new DWORD[indexCount];

		for (int i = 0; i < indexCount; i++)
		{
			pIndices[i] = i;
		}	

		// Calculate the screen coordinates of the left side of the window.
		float left = ((_width / 2) * -1);

		// Calculate the screen coordinates of the right side of the window.
		float right = left + _width;

		// Calculate the screen coordinates of the top of the window.
		float top = (_height / 2);

		// Calculate the screen coordinates of the bottom of the window.
		float bottom = top - _height;

		// First triangle.
		pVertices[0].pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		pVertices[0].uv = v2float(0.0f, 0.0f);

		pVertices[1].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		pVertices[1].uv = v2float(1.0f, 1.0f);

		pVertices[2].pos = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
		pVertices[2].uv = v2float(0.0f, 1.0f);

		// Second triangle.
		pVertices[3].pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		pVertices[3].uv = v2float(0.0f, 0.0f);

		pVertices[4].pos = D3DXVECTOR3(right, top, 0.0f);  // Top right.
		pVertices[4].uv = v2float(1.0f, 0.0f);

		pVertices[5].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		pVertices[5].uv = v2float(1.0f, 1.0f);

		pBuff = new DX10_Buffer(_pDevice);
		VALIDATE(pBuff->Initialise(pVertices, pIndices, vertexCount, indexCount, sizeof(TVertexUV), D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

		// Clean up
		ReleasePtrArray(pVertices);
		ReleasePtrArray(pIndices);

		return true;
	}
};

class DX10_ShadowMap
{
public:

	/***********************
	* DX10_ShadowMap: Default Constructor for ShadowMap class
	* @author: Callan Moore
	********************/
	DX10_ShadowMap();

	/***********************
	* ~DX10_ShadowMap: Default Destructor for ShadowMap class
	* @author: Callan Moore
	********************/
	~DX10_ShadowMap();

	/***********************
	* Initialise: Initialise the Shadow Map for use
	* @author: Callan Moore
	* @parameter: _pDX10_Device: The graphics device
	* @parameter: _width: The width to make the shadow map
	* @parameter: _height: The height to make the shadow map
	* @return: bool: Successful or not
	********************/
	bool Initialise(ID3D10Device* _pDX10_Device, UINT _width, UINT _height, float _downSizeScalar);
	
	/***********************
	* GetShaderResourceView: Retrieve the shader resource view of the shadow map
	* @author: Callan Moore
	* @return: ID3D10ShaderResourceView*: The shader resource view of the shadow map
	********************/
	ID3D10ShaderResourceView* GetShaderResourceView();
	
	/***********************
	* StartRender: Start rendering to the shadow map by switching the render target
	* @author: Callan Moore
	* @return: void
	********************/
	void StartRender();
	
	/***********************
	* EndRender: Stop Rendering to the shadow map
	* @author: Callan Moore
	* @return: void
	********************/
	void EndRender();

	// TO DO CAL
	UINT GetWidth() { return m_width; };
	UINT GetHeight() { return m_height; };
	float GetDownsizeScalar() { return m_downSizeScalar; };
	TWindow* GetScreenWindow() { return m_pScreenWindow; };
	TWindow* GetHalfWindow() { return m_pHalfWindow; };
	TextureResource* GetShadowedTex() { return m_pShadowedTex; };
	TextureResource* GetBlurTex_Horizontal() { return m_pBlurTex_Horizontal; };
	TextureResource* GetBlurTex_Vertical() { return m_pBlurTex_Vertical; };
	TextureResource* GetBlurTex_DownSized() { return m_pBlurTex_DownSized; };
	TextureResource* GetBlurTex_UpSized() { return m_pBlurTex_UpSized; };

private:
	// Disallowing copies
	DX10_ShadowMap(const DX10_ShadowMap& rhs);
	DX10_ShadowMap& operator=(const DX10_ShadowMap& rhs);

	// TO DO CAL
	bool BuildShadowMap();
	bool BuildShadowedTex();
	bool BuildBlurTex_DownSized();
	bool BuildBlurTex_Horizontal();
	bool BuildBlurTex_Vertical();
	bool BuildBlurTex_UpSized();

private:
	ID3D10Device* m_pDX10Device;
	D3D10_VIEWPORT mViewport;

	ID3D10ShaderResourceView* m_ShaderResourceView;
	ID3D10DepthStencilView* m_pDepthStencilView;

	UINT m_width;
	UINT m_height;

	TWindow* m_pHalfWindow;
	TWindow* m_pScreenWindow;

	TextureResource* m_pShadowedTex;
	TextureResource* m_pBlurTex_DownSized;
	TextureResource* m_pBlurTex_Horizontal;
	TextureResource* m_pBlurTex_Vertical;
	TextureResource* m_pBlurTex_UpSized;

	float m_downSizeScalar;
};

#endif // __DX10_SHADOWMAP_H__