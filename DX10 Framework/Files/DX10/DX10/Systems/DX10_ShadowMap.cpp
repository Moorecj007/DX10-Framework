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
	m_ShaderResourceView_Depth = 0;
	m_pDepthStencilView = 0;

	m_width = 0;
	m_height = 0;
}

DX10_ShadowMap::~DX10_ShadowMap()
{
	// Release allocated memory
	ReleaseCOM(m_ShaderResourceView_Depth);
	ReleaseCOM(m_pDepthStencilView);

	ReleasePtr(m_pBuff);
}

bool DX10_ShadowMap::Initialise(ID3D10Device* _pDevice, UINT _width, UINT _height)
{
	if (_pDevice == 0)
	{
		// If the device is not valid then the shadow map cannot be created
		return false;
	}

	// Assign Member variables
	m_pDX10Device = _pDevice;
	m_width = _width;
	m_height = _height;

	// Create the view port 
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = _width;
	mViewport.Height = _height;
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
	VALIDATEHR(m_pDX10Device->CreateDepthStencilView(pTexture, &dsvDesc, &m_pDepthStencilView_Blurred));

	// Create a shader resource view of the texture 
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	VALIDATEHR(m_pDX10Device->CreateShaderResourceView(pTexture, &srvDesc, &m_ShaderResourceView_Depth));

	// Release the reference to the texture as it is saved in the views
	ReleaseCOM(pTexture);

	// Recreate the texture description for a render target
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	VALIDATEHR(m_pDX10Device->CreateTexture2D(&texDesc, 0, &pTexture));

	// Create the Shader Resource View
	VALIDATEHR(m_pDX10Device->CreateRenderTargetView(pTexture, 0, &m_pRenderTargetView_Blurred));
	VALIDATEHR(m_pDX10Device->CreateShaderResourceView(pTexture, 0, &m_ShaderResourceView_Blurred));

	VALIDATE(InitialiseBuffer())

	return true;
}

ID3D10ShaderResourceView* DX10_ShadowMap::GetShaderResourceView()
{
	return m_ShaderResourceView_Depth;
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
	// Set the device ready for blurring the image
	ID3D10RenderTargetView* renderTargets[1] = { m_pRenderTargetView_Blurred };
	m_pDX10Device->OMSetRenderTargets(1, renderTargets, m_pDepthStencilView_Blurred);
	m_pDX10Device->RSSetViewports(1, &mViewport);
	m_pDX10Device->ClearDepthStencilView(m_pDepthStencilView_Blurred, D3D10_CLEAR_DEPTH, 1.0f, 0);

	m_pDX10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool DX10_ShadowMap::InitialiseBuffer()
{
	TVertexUV* pVertices;
	unsigned long* pIndices;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	pVertices = new TVertexUV[m_vertexCount];
	if (!pVertices)
	{
		return false;
	}

	// Create the index array.
	pIndices = new unsigned long[m_indexCount];
	if (!pIndices)
	{
		return false;
	}

	// Initialize vertex array to zeros.
	memset(pVertices, 0, (sizeof(TVertexUV) * m_vertexCount));

	// Load the index array with data.
	for (UINT i = 0; i < m_indexCount; ++i)
	{
		pIndices[i] = i;
	}

	m_pBuff = new DX10_Buffer(m_pDX10Device);
	VALIDATE(m_pBuff->Initialise(pVertices, pIndices, m_vertexCount, m_indexCount, (UINT)sizeof(TVertexUV), D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

	// Clean up
	delete[] pVertices;
	pVertices = 0;

	delete[] pIndices;
	pIndices = 0;

	return true;
}

void DX10_ShadowMap::RenderBuffer()
{
	m_pBuff->Render();
}