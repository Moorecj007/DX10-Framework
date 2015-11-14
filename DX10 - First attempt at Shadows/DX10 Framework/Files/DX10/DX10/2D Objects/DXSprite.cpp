// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	DXSprite.cpp
// Description	:	The implementation file.
// Author(s)	:	Juran Griffith.
// Mail			:	juran.griffith@mediadesignschool.com
//

// Library Includes

// Local Includes
#include "DXSprite.h"

DXSprite::DXSprite()
	: m_pDX10_Renderer(0)
	, m_pTex(0)
	, m_scale(1.0f)
{
	m_offsetScreenWidthPrev = 0.0f;
	m_offsetScreenHeightPrev = 0.0f;
}

DXSprite::~DXSprite()
{
}

bool DXSprite::Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_Sprite* _pShader, std::string _filename, UINT _imageWidth, UINT _imageHeight, UINT _sliceWidth, UINT _sliceHeight)
{
	// Save the pointers
	m_pDX10_Renderer = _pDX10_Renderer;
	m_pShader_Sprite = _pShader;

	m_strFilename = _filename;

	m_sliceWidth = _sliceWidth;
	m_sliceHeight = _sliceHeight;

	m_index = 0;
	m_indexPrev = 0;
	m_animationLooped = false;

	POINT temp;
	for (int r = 0; r < m_sliceHeight; ++r)
	{
		for (int c = 0; c < m_sliceWidth; ++c)
		{
			temp.x = c + 1;
			temp.y = r + 1;
			m_imageIndexList.push_back(temp);
		}
	}

	//GetPngSize();

	// Save the size of the image
	m_realImageWidth = _imageWidth;
	m_realImageHeight = _imageHeight;

	m_offsetImageWidth = static_cast<float>(_imageWidth) / static_cast<float>(m_sliceWidth);
	m_offsetImageHeight = static_cast<float>(_imageHeight) / static_cast<float>(m_sliceHeight);

	m_imageWidth = (UINT)m_offsetImageWidth;
	m_imageHeight = (UINT)m_offsetImageHeight;

	m_offsetU = 1.0f / static_cast<float>(m_sliceWidth);
	m_offsetV = 1.0f / static_cast<float>(m_sliceHeight);

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	if (!InitialiseBuffers())
		return false;

	VALIDATE(m_pDX10_Renderer->CreateTexture(m_strFilename, m_pTex));

	m_previousPosition = v2float(-1.0f, -1.0f);
	m_position = v2float(0.0f, 0.0f);

	return true;
}

void DXSprite::SetPosition(float _x, float _y)
{
	// Re-build the dynamic vertex buffer for rendering to a different location on the screen.
	m_position.x = _x;
	m_position.y = _y;
	//UpdateBuffers();
}

int DXSprite::GetIndexCount()
{
	return m_indexCount;
}

int DXSprite::GetSliceWidth()
{
	return m_sliceWidth;
}

int DXSprite::GetSliceHeight()
{
	return m_sliceHeight;
}

float DXSprite::GetWidth()
{
	return (float)m_imageWidth;
}

float DXSprite::GetHeight()
{
	return (float)m_imageHeight;
}

float DXSprite::GetImageWidth()
{
	return static_cast<float>(m_realImageWidth);
}

float DXSprite::GetImageHeight()
{
	return static_cast<float>(m_realImageHeight);
}

v2float DXSprite::GetPosition()
{
	return m_position;
}

void DXSprite::SetImageIndex(UINT _index)
{
	if (!m_imageIndexList.empty() && _index < m_imageIndexList.size())
	{
		m_index = _index;
	}
}

void DXSprite::SetSize(float _width, float _height)
{
	m_imageWidth = (UINT)_width;
	m_imageHeight = (UINT)_height;

	//m_offsetImageWidth = _width / static_cast<float>(m_sliceWidth);
	//m_offsetImageHeight = _height / static_cast<float>(m_sliceHeight);
}

void DXSprite::SetLooped(bool _looped)
{
	m_animationLooped = _looped;
}

void DXSprite::SetScale(float _scale)
{
	m_scale = _scale;
	SetSize(m_imageWidth * _scale, m_imageHeight * _scale);
}

void DXSprite::IncrementIndex()
{
	if (!m_imageIndexList.empty() && (m_index + 1) < m_imageIndexList.size())
	{
		m_index++;
	}
	else if (m_animationLooped)
	{
		m_index = 0;
	}
}

bool DXSprite::IsAtLastFrame()
{
	return m_index == m_imageIndexList.size() - 1;
}

void DXSprite::Render()
{
	UpdateBuffers();
	// Set the type of primitive
	m_pDX10_Renderer->SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pShader_Sprite->Render(m_pBuff, m_pTex);
}

bool DXSprite::InitialiseBuffers()
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

	VALIDATE(m_pDX10_Renderer->CreateBuffer(pVertices, pIndices, m_vertexCount, m_indexCount, (UINT)sizeof(TVertexUV), m_pBuff, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

	// Clean up
	delete[] pVertices;
	pVertices = 0;

	delete[] pIndices;
	pIndices = 0;

	return true;
}

bool DXSprite::UpdateBuffers()
{
	// If the position has not changed then don't update the vertex buffer
	if ((m_position == m_previousPosition) && (m_indexPrev == m_index) && (m_offsetScreenWidthPrev == m_offsetScreenWidth) && (m_offsetScreenHeightPrev == m_offsetScreenHeight))
		return true;

	// Calculate offsets
	m_offsetScreenWidth = (static_cast<float>(m_pDX10_Renderer->GetWidth()) / 2.0f) * -1.0f;
	m_offsetScreenHeight = (static_cast<float>(m_pDX10_Renderer->GetHeight()) / 2.0f);

	m_offsetScreenWidthPrev = m_offsetScreenWidth;
	m_offsetScreenHeightPrev = m_offsetScreenHeight;

	float left, right, top, bottom;
	TVertexUV* vertices;
	void* verticesPtr;

	// Update the position
	m_previousPosition = m_position;

	m_indexPrev = m_index;

	// Calculate the screen coordinates of the left side of the image.
	left = m_offsetScreenWidth + m_position.x;

	// Calculate the screen coordinates of the right side of the image.
	//right = left + m_offsetImageWidth;
	right = left + m_imageWidth;

	// Calculate the screen coordinates of the top of the image.
	top = m_offsetScreenHeight - m_position.y;

	// Calculate the screen coordinates of the bottom of the image.
	//bottom = top - m_offsetImageHeight;
	bottom = top - m_imageHeight;

	// Create the vertex array.
	vertices = new TVertexUV[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Load the vertex array with data.

	// First triangle.
	vertices[0].pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].uv = v2float((m_imageIndexList[m_index].x - 1) * m_offsetU, (m_imageIndexList[m_index].y - 1) * m_offsetV);

	vertices[1].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].uv = v2float(m_imageIndexList[m_index].x * m_offsetU, m_imageIndexList[m_index].y * m_offsetV);

	vertices[2].pos = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].uv = v2float((m_imageIndexList[m_index].x - 1) * m_offsetU, m_imageIndexList[m_index].y * m_offsetV);
	
	// Second triangle.
	vertices[3].pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].uv = v2float((m_imageIndexList[m_index].x - 1) * m_offsetU, (m_imageIndexList[m_index].y - 1) * m_offsetV);

	vertices[4].pos = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].uv = v2float(m_imageIndexList[m_index].x * m_offsetU, (m_imageIndexList[m_index].y - 1) * m_offsetV);

	vertices[5].pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].uv = v2float(m_imageIndexList[m_index].x * m_offsetU, m_imageIndexList[m_index].y * m_offsetV);

	// Initialize the vertex buffer pointer.
	verticesPtr = 0;

	// Lock the vertex buffer.
	ID3D10Buffer* pVertexBuff = m_pBuff->GetVertexBuffer();
	VALIDATEHR(pVertexBuff->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr));

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(TVertexUV) * m_vertexCount));

	// Unlock the vertex buffer.
	pVertexBuff->Unmap();

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool DXSprite::GetPngSize()
{
	std::string filename = TEXTUREFILEPATH;
	filename.append(m_strFilename);
	std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);
	//std::ifstream file(filename);

	if (!file.is_open() || !file) {
		file.close();
		return false;
	}

	// Skip chucks
	file.seekg(16, std::ios_base::cur);

	UINT width, height;

	file.read((char*)&width, 4);
	file.read((char*)&height, 4);

	//width = ntohl(width); // Testing
	//height = ntohl(height);

	//m_imageWidth = static_cast<float>(width);
	//m_imageHeight = static_cast<float>(height);

	file.close();

	return true;
}