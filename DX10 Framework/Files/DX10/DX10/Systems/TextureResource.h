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

	bool Initialize(ID3D10Device*, int, int, float, float);
	void Shutdown();

	void SetRenderTarget();
	void ClearRenderTarget(float, float, float, float);
	ID3D10ShaderResourceView* GetShaderResourceView();
	D3DXMATRIX GetProjectionMatrix() { return m_projectionMatrix; };
	D3DXMATRIX GetOrthoMatrix(){ return m_orthoMatrix; };
	D3DXMATRIX GetViewMatrix(){ return m_viewMatrix; };
	D3DXMATRIX GetWorldMatrix(){ return m_worldMatrix; };

private:
	ID3D10Device* m_pDevice;

	ID3D10Texture2D* m_renderTargetTexture;
	ID3D10RenderTargetView* m_renderTargetView;
	ID3D10ShaderResourceView* m_shaderResourceView;
	ID3D10Texture2D* m_depthStencilBuffer;
	ID3D10DepthStencilView* m_depthStencilView;
	D3D10_VIEWPORT m_viewport;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_worldMatrix;
};

#endif	// __TEXTURE_RESOURCE_H__

