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

// TO DO CAL
enum eTexResourceType
{
	TRT_COLOR,
	TRT_SHADOW
};

class DX10_TextureResource
{
public:

	/***********************
	* ~TextureResource: Default Constructor for Texture Resource class
	* @author: Callan Moore
	********************/
	DX10_TextureResource();

	/***********************
	* ~TextureResource: Default Destructor for Texture Resource class
	* @author: Callan Moore
	********************/
	~DX10_TextureResource();

	/***********************
	* Initialise: Initialise the Texture Resource for use
	* @author: Callan Moore
	* @parameter: _pDX10Device: The graphics device
	* @parameter: _pDepthStencilView: Pointer to the Depth Stencil View that the Renderer uses
	* @parameter: _width: Width to make the texture resource
	* @parameter: _height: Height to make the texture resource
	* @parameter: _type: The type of texture resource to make
	* @return: bool: Successful or not
	********************/
	bool Initialise(ID3D10Device* _pDX10Device, ID3D10DepthStencilView* _pDepthStencilView, int _width, int _height, eTexResourceType _type);
	
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
	* GetDiffuseMap: Retrieve the ShaderResourceView of the Texture Resource diffuse map
	* @author: Callan Moore
	* @return: ID3D10ShaderResourceView*: Shader Resource View of the diffuse map to input to a shader
	********************/
	ID3D10ShaderResourceView* GetDiffuseMap() { return m_pSRV_Diffuse; };

	/***********************
	* GetDepthMap: Retrieve the ShaderResourceView of the Texture Resource depth map
	* @author: Callan Moore
	* @return: ID3D10ShaderResourceView*: Shader Resource View of the depth map to input to a shader
	********************/
	ID3D10ShaderResourceView* GetDepthMap() { return m_pSRV_Depth; };
	
	/***********************
	* StartRender: Sets the graphics device to render to this texture
	* @author: Callan Moore
	* @return: void
	********************/
	void StartRender();
	
	/***********************
	* EndRender: Rendering to this texture is complete, Do post rendering processes
	* @author: Callan Moore
	* @return: void
	********************/
	void EndRender();

private:

	/***********************
	* BuildDiffuseMap: Build the texture and views for the diffuse map
	* @author: Callan Moore
	* @return: bool Successful or not
	********************/
	bool BuildDiffuseMap();

	/***********************
	* BuildDepthMap: Build the texture and views for the depth map
	* @author: Callan Moore
	* @return: bool Successful or not
	********************/
	bool BuildDepthMap();

private:
	ID3D10Device* m_pDX10Device;
	ID3D10DepthStencilView* m_pDepthStencilView;
	D3D10_VIEWPORT m_viewport;
	D3DXCOLOR m_clearColor;

	UINT m_width;
	UINT m_height;

	// Diffuse map views
	ID3D10RenderTargetView* m_pRTV_Diffuse;
	ID3D10ShaderResourceView* m_pSRV_Diffuse;

	// Depth map views
	ID3D10ShaderResourceView* m_pSRV_Depth;
	ID3D10DepthStencilView* m_pDSV_Depth;
};

#endif	// __TEXTURE_RESOURCE_H__

