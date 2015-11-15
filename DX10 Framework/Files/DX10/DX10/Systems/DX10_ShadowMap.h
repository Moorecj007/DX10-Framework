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
#include "DX10_Buffer.h"
#include "../DX10_Vertex.h"

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
	bool Initialise(ID3D10Device* _pDX10_Device, UINT _width, UINT _height);
	
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
	bool InitialiseBuffer();
	void RenderBuffer();
	DX10_Buffer* GetBuffer() { return m_pBuff; };
	ID3D10ShaderResourceView* GetBlurredMap() { return m_ShaderResourceView_Blurred; };

private:
	// Disallowing copies
	DX10_ShadowMap(const DX10_ShadowMap& rhs);
	DX10_ShadowMap& operator=(const DX10_ShadowMap& rhs);

private:
	ID3D10Device* m_pDX10Device;
	D3D10_VIEWPORT mViewport;

	ID3D10ShaderResourceView* m_ShaderResourceView_Depth;
	ID3D10DepthStencilView* m_pDepthStencilView;

	ID3D10RenderTargetView* m_pRenderTargetView_Blurred;
	ID3D10ShaderResourceView* m_ShaderResourceView_Blurred;
	ID3D10DepthStencilView* m_pDepthStencilView_Blurred;

	UINT m_width;
	UINT m_height;	

	// Blurring Variables  TO DO CAL
	UINT m_vertexCount;
	UINT m_indexCount;
	DX10_Buffer* m_pBuff;
};

#endif // __DX10_SHADOWMAP_H__