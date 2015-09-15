// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	DXBitmap.cpp
// Description	:	The implementation file.
// Author(s)	:	Juran Griffith.
// Mail			:	juran.griffith@mediadesignschool.com
//

// Library Includes

// Local Includes
#include "DXSprite.h"

DXSprite::DXSprite():
	m_previousPosX(-1),
	m_previousPosY(-1)
{
	m_pDX10_Renderer = 0;
	m_texID = 0;
}

DXSprite::~DXSprite()
{

}

bool DXSprite::Initialize(DX10_Renderer* _pDX10_Renderer, FXTexture* _pShader, std::string _filename, float _bitmapWidth, float _bitmapHeight, int _sliceWidth, int _sliceHeight)
{
	// Save the pointer to the device
	m_pDX10_Renderer = _pDX10_Renderer;
	m_pShader = _pShader;

	m_strFilename = _filename;

	m_sliceWidth = _sliceWidth;
	m_sliceHeight = _sliceHeight;

	m_index = 0;
	m_indexPrev = 0;

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

	// Set the size to render the image
	m_bitmapWidth = _bitmapWidth;
	m_bitmapHeight = _bitmapHeight;

	// Save the screen size.
	RECT rect;
	if (GetClientRect(*m_pHWnd, &rect))
	{
		m_screenWidth = rect.right - rect.left;
		m_screenHeight = rect.bottom - rect.top;
	}

	// Calculate offsets to reduce the amount of divides in the update call.
	m_offsetScreenWidth = static_cast<float>((m_screenWidth / 2) * -1);
	m_offsetScreenHeight = static_cast<float>(m_screenHeight / 2);

	m_offsetBitmapWidth = static_cast<float>(m_bitmapWidth / m_sliceWidth);
	m_offsetBitmapHeight = static_cast<float>(m_bitmapHeight / m_sliceHeight);

	m_offsetU = static_cast<float>(1.0f / m_sliceWidth);
	m_offsetV = static_cast<float>(1.0f / m_sliceHeight);

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	if (!InitializeBuffers())
		return false;

	VALIDATE(m_pDX10_Renderer->CreateTexture(m_strFilename, &m_texID));

	return true;
}

void DXSprite::Process(float _positionX, float _positionY)
{
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	if (!UpdateBuffers(_positionX, _positionY))
		return;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	Render();
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

void DXSprite::SetImageIndex(int _index)
{
	if (!m_imageIndexList.empty() && static_cast<UINT>(_index) < m_imageIndexList.size())
	{
		m_index = _index;
	}
}

void DXSprite::SetSize(float _width, float _height)
{
	m_bitmapWidth = _width;
	m_bitmapHeight = _height;

	m_offsetBitmapWidth = static_cast<float>(_width / m_sliceWidth);
	m_offsetBitmapHeight = static_cast<float>(_height / m_sliceHeight);
}

bool DXSprite::InitializeBuffers()
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

	VALIDATE(m_pDX10_Renderer->CreateBuffer(pVertices, pIndices, m_vertexCount, m_indexCount, (UINT)sizeof(TVertexUV), &m_buffID, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

	// Clean up
	delete[] pVertices;
	pVertices = 0;

	delete[] pIndices;
	pIndices = 0;

	return true;
}

bool DXSprite::UpdateBuffers(float _positionX, float _positionY)
{
	float left, right, top, bottom;
	TVertexUV* vertices;
	void* verticesPtr;

	// If the position has not changed then don't update the vertex buffer
	if ((_positionX == m_previousPosX) && (_positionY == m_previousPosY) && (m_indexPrev == m_index))
		return true;

	// Update the position
	m_previousPosX = _positionX;
	m_previousPosY = _positionY;

	m_indexPrev = m_index;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = m_offsetScreenWidth + static_cast<float>(_positionX);

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + m_offsetBitmapWidth;

	// Calculate the screen coordinates of the top of the bitmap.
	top = m_offsetScreenHeight - static_cast<float>(_positionY);

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - m_offsetBitmapHeight;

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
	ID3D10Buffer* pVertexBuff = m_pDX10_Renderer->GetVertexBuffer(m_buffID);
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

void DXSprite::Render()
{
	// Set the type of primitive
	m_pDX10_Renderer->SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pShader->Render(m_buffID, m_texID);
}