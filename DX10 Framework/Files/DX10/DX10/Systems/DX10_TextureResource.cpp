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
#include "DX10_TextureResource.h"

DX10_TextureResource::DX10_TextureResource()
{
	// Initialise Pointers to NULL
	m_pDX10Device = 0;
	m_pDepthStencilView = 0;
	ZeroMemory(&m_viewport, sizeof(m_viewport));
	m_clearColor = d3dxColors::Black;

	m_pRTV_Diffuse = 0;
	m_pSRV_Diffuse = 0;

	m_pDSV_Depth = 0;
	m_pSRV_Depth = 0;

	m_width = 0;
	m_height = 0;
}

DX10_TextureResource::~DX10_TextureResource()
{
	// Release the color map views
	ReleaseCOM(m_pRTV_Diffuse);
	ReleaseCOM(m_pSRV_Diffuse);

	// Release the depth map views
	ReleaseCOM(m_pDSV_Depth);
	ReleaseCOM(m_pSRV_Depth);
}

bool DX10_TextureResource::Initialise(ID3D10Device* _pDX10Device, ID3D10DepthStencilView* _pDepthStencilView, int _width, int _height, eTexResourceType _type)
{
	if (_pDX10Device == 0 || _pDepthStencilView == 0)
	{
		// If any pointer is NULL then the initialize is failed
		return false;
	}

	// Assign Member Variables
	m_pDX10Device = _pDX10Device;	
	m_pDepthStencilView = _pDepthStencilView;
	m_width = _width;
	m_height = _height;

	// Build the maps based on what type of resource this is
	switch (_type)
	{
		case TRT_COLOR:
		{
			BuildDiffuseMap();
		}
		break;
		case TRT_SHADOW:	// Fall Through
		default: break;
	}	// End Switch

	BuildDepthMap();

	// Set up the view port
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = m_width;
	m_viewport.Height = m_height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	return true;
}

void DX10_TextureResource::SetRenderTarget()
{
	// Set the Texture Resource Render Target View as the devices current Render Target
	m_pDX10Device->OMSetRenderTargets(1, &m_pRTV_Diffuse, m_pDepthStencilView);
}

void DX10_TextureResource::ClearRenderTarget()
{
	// Clear the Render Target (back buffer) and the Depth Buffer of the Texture Resources Texture
	m_pDX10Device->ClearRenderTargetView(m_pRTV_Diffuse, m_clearColor);
	m_pDX10Device->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

void DX10_TextureResource::StartRender()
{
	// Create a array of RTV so that the passed in parameter is never NULL
	// The element of the diffuse map can be NULL and this is ok
	ID3D10RenderTargetView* pRenderTargets[1];
	pRenderTargets[0] = m_pRTV_Diffuse;
	m_pDX10Device->OMSetRenderTargets(1, pRenderTargets, m_pDSV_Depth);

	// Set the texture resource view port as the current view port for the Rasterizer
	m_pDX10Device->RSSetViewports(1, &m_viewport);

	if (m_pRTV_Diffuse != 0)
	{
		// Clear the render target view only if the diffuse map exists
		m_pDX10Device->ClearRenderTargetView(m_pRTV_Diffuse, m_clearColor);
	}

	// Clear the Depth stecil view of the depth map
	m_pDX10Device->ClearDepthStencilView(m_pDSV_Depth, D3D10_CLEAR_DEPTH, 1.0f, 0);;

}

void DX10_TextureResource::EndRender()
{
	// Ensure that the diffuse map exists
	if (m_pSRV_Diffuse != 0)
	{
		// Generate the mipmaps after the rendering is complete
		m_pDX10Device->GenerateMips(m_pSRV_Diffuse);
	}
}

bool DX10_TextureResource::BuildDiffuseMap()
{
	ID3D10Texture2D* pTexture;

	// Initialise the Texture 2D description
	D3D10_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_UNKNOWN;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

	// Create the Texture 2D from the Description
	VALIDATEHR(m_pDX10Device->CreateTexture2D(&textureDesc, NULL, &pTexture));

	VALIDATEHR(m_pDX10Device->CreateRenderTargetView(pTexture, 0, &m_pRTV_Diffuse));
	VALIDATEHR(m_pDX10Device->CreateShaderResourceView(pTexture, 0, &m_pSRV_Diffuse));

	// Release the texture as the reference is saved by the view
	ReleaseCOM(pTexture);

	return true;
}

bool DX10_TextureResource::BuildDepthMap()
{
	ID3D10Texture2D* pTexture;

	// Initialise the Texture 2D description
	D3D10_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the Texture 2D from the Description
	VALIDATEHR(m_pDX10Device->CreateTexture2D(&textureDesc, NULL, &pTexture));

	// Initialise the depth stencil view description for the depth map
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	VALIDATEHR(m_pDX10Device->CreateDepthStencilView(pTexture, &depthStencilViewDesc, &m_pDSV_Depth));

	// Initialise the Shader Resource View Description for the depth map
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = textureDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MipLevels = 0;
	
	// Create the Shader Resource View from the Texture 2D and the Description
	VALIDATEHR(m_pDX10Device->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &m_pSRV_Depth));

	// Release the texture as the reference is saved by the view
	ReleaseCOM(pTexture);

	return true;
}