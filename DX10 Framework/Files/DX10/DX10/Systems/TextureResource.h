/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : TextureResource.h
* Description : Texture Resource for use on the device and Shaders
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __TEXTURE_RESOURCE_H__
#define __TEXTURE_RESOURCE_H__

// Local Includes
#include "../DX10_Utilities.h"

class TextureResource
{
public:

	/***********************
	* TextureResource: Default Constructor for Texture Resource class
	* @author: Callan Moore
	********************/
	TextureResource();

	/***********************
	* ~TextureResource: Default Destructor for Texture Resource class
	* @author: Callan Moore
	********************/
	~TextureResource();

	/***********************
	* Initialise: Initialise the Texture Resource for use
	* @author: Callan Moore
	* @parameter: _pDX10Device: The graphics device
	* @parameter: _pDepthStencilView: Pointer to the Depth Stencil View that the Renderer uses
	* @parameter: _clearColor: The clear color of the Renderer
	* @parameter: _pTexture: The Texture 2D created fro this resource
	* @parameter: _pRenderTargetView: RenderTargetView of the Texture
	* @parameter: _pShaderResourceView: ShaderResourceView of the Texture
	* @return: bool: Successful or not
	********************/
	bool Initialise(ID3D10Device* _pDX10Device, ID3D10DepthStencilView* _pDepthStencilView, D3DXCOLOR _clearColor, ID3D10Texture2D* _pTexture, ID3D10RenderTargetView* _pRenderTargetView, ID3D10ShaderResourceView* _pShaderResourceView);
	
	/***********************
	* SetRenderTarget: Set the Texture Resource as the Render Target of the Graphics Device
	* @author: Callan Moore
	* @return: void
	********************/
	void SetRenderTarget();
	
	/***********************
	* ClearRenderTarget: Clear the Render Target of this Texture Resource to the clear color
	* @author: Callan Moore
	* @return: void
	********************/
	void ClearRenderTarget();

	/***********************
	* GetShaderResource: Retrieve the ShaderResourceView of the Texture Resource
	* @author: Callan Moore
	* @return: ID3D10ShaderResourceView*: Shader Resource View to input to a shader
	********************/
	ID3D10ShaderResourceView* GetShaderResource() { return m_pShaderResourceView; };

private:
	ID3D10Device* m_pDX10Device;
	ID3D10DepthStencilView* m_pDepthStencilView;
	D3DXCOLOR m_clearColor;

	ID3D10Texture2D* m_pTexture;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10ShaderResourceView* m_pShaderResourceView;
};

#endif	// __TEXTURE_RESOURCE_H__

