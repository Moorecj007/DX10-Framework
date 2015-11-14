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

private:
	// Disallowing copies
	DX10_ShadowMap(const DX10_ShadowMap& rhs);
	DX10_ShadowMap& operator=(const DX10_ShadowMap& rhs);

private:
	ID3D10Device* m_pDX10Device;
	D3D10_VIEWPORT mViewport;

	ID3D10ShaderResourceView* m_ShaderResourceView;
	ID3D10DepthStencilView* m_pDepthStencilView;

	UINT m_width;
	UINT m_height;	
};

#endif // __DX10_SHADOWMAP_H__