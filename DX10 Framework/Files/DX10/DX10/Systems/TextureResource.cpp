/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : TextureResource.cpp
* Description : Texture Resource for use on the device and Shaders
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "TextureResource.h"

TextureResource::TextureResource()
{
	// Initialise Pointers to NULL
	m_pDX10Device = 0;
	m_pDepthStencilView = 0;
	m_pTexture = 0;
	m_pRenderTargetView = 0;
	m_pShaderResourceView = 0;
}

TextureResource::~TextureResource()
{
	ReleaseCOM(m_pTexture);
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pShaderResourceView);
}

bool TextureResource::Initialise(ID3D10Device* _pDX10Device, ID3D10DepthStencilView* _pDepthStencilView, D3DXCOLOR _clearColor, ID3D10Texture2D* _pTexture, ID3D10RenderTargetView* _pRenderTargetView, ID3D10ShaderResourceView* _pShaderResourceView)
{

	if (_pDX10Device == 0 ||
		_pDepthStencilView == 0 ||
		_pTexture == 0 ||
		_pRenderTargetView == 0 ||
		_pShaderResourceView == 0)
	{
		// If any pointer is NULL then the initialize is failed
		return false;
	}

	// Assign Member Variables
	m_pDX10Device = _pDX10Device;
	m_pDepthStencilView = _pDepthStencilView;
	m_clearColor = _clearColor;
	m_pTexture = _pTexture;
	m_pRenderTargetView = _pRenderTargetView;
	m_pShaderResourceView = _pShaderResourceView;	

	return true;
}

void TextureResource::SetRenderTarget()
{
	// Set the Texture Resource Render Target View as the devices current Render Target
	m_pDX10Device->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void TextureResource::ClearRenderTarget()
{
	// Clear the Render Target (back buffer) and the Depth Buffer of the Texture Resources Texture
	m_pDX10Device->ClearRenderTargetView(m_pRenderTargetView, m_clearColor);
	m_pDX10Device->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}
