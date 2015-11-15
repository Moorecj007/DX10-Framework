/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_ShadowMap.cpp
* Description : Shadow Map for shadowing techniques
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_ShadowMap.h"

DX10_ShadowMap::DX10_ShadowMap()
{
	// Ensure the memory is clear
	ZeroMemory(&mViewport, sizeof(D3D10_VIEWPORT));

	// Initialise all pointer to NULL
	m_pDX10Device = 0;
	m_ShaderResourceView = 0;
	m_pDepthStencilView = 0;

	m_width = 0;
	m_height = 0;
}

DX10_ShadowMap::~DX10_ShadowMap()
{
	// Release allocated memory
	ReleaseCOM(m_ShaderResourceView);
	ReleaseCOM(m_pDepthStencilView);
}

bool DX10_ShadowMap::Initialise(ID3D10Device* device, UINT width, UINT height)
{
	if (device == 0)
	{
		// If the device is not valid then the shadow map cannot be created
		return false;
	}

	// Assign Member variables
	m_pDX10Device = device;
	m_width = width;
	m_height = height;

	// Create the view port 
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = width;
	mViewport.Height = height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	
	//Create a texture description and clear its memory
	D3D10_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// Create a 2D Texture
	ID3D10Texture2D* pTexture = 0;
	VALIDATEHR(m_pDX10Device->CreateTexture2D(&texDesc, 0, &pTexture));

	// Create a depth stencil view of the texture
	D3D10_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	VALIDATEHR(m_pDX10Device->CreateDepthStencilView(pTexture, &dsvDesc, &m_pDepthStencilView));

	// Create a shader resource view of the texture 
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	VALIDATEHR(m_pDX10Device->CreateShaderResourceView(pTexture, &srvDesc, &m_ShaderResourceView));

	// Release the reference to the texture as it is saved in the views
	ReleaseCOM(pTexture);

	return true;
}

ID3D10ShaderResourceView* DX10_ShadowMap::GetShaderResourceView()
{
	return m_ShaderResourceView;
}

void DX10_ShadowMap::StartRender()
{
	// Set the Render target to NULL (only using the depth stencil) using an array of null elements
	ID3D10RenderTargetView* renderTargets[1] = { 0 };
	m_pDX10Device->OMSetRenderTargets(1, renderTargets, m_pDepthStencilView);
	m_pDX10Device->RSSetViewports(1, &mViewport);

	m_pDX10Device->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

void DX10_ShadowMap::EndRender()
{

}