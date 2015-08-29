/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_StaticBuffer.cpp
* Description : Wrapper class to hold all information for Vertex and Index Buffers for use in DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_StaticBuffer.h"

DX10_StaticBuffer::DX10_StaticBuffer(ID3D10Device* _pDX10Device)
{
	m_pDX10Device = _pDX10Device;
}

DX10_StaticBuffer::~DX10_StaticBuffer()
{
	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pIndexBuffer);
}

void DX10_StaticBuffer::Render()
{
	UINT offset = 0;
	m_pDX10Device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &offset);
	m_pDX10Device->IASetIndexBuffer(m_pIndexBuffer, m_indexFormat, 0);
	m_pDX10Device->DrawIndexed(m_indexCount, 0, 0);
}
