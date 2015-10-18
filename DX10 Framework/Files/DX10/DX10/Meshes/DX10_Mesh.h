/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Mesh.h
* Description : A Generic Mesh (Abstract class)
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_MESH_H__
#define __DX10_MESH_H__

// Local Includes
#include "../DX10_Renderer.h"

enum eMeshType
{
	MT_FINITEPLANE,
	MT_RECTPRISM,
	MT_SPHERE,
	MT_EPICTERRAIN,
	MT_WHARF,
	MT_STARMAP,
	MT_5X5
};

class DX10_Mesh
{
public:
	/***********************
	* DX10_Mesh: Default Constructor for Mesh class
	* @author: Callan Moore
	********************/
	DX10_Mesh() {}

	/***********************
	* ~DX10_Mesh: Default Destructor for Mesh class
	* @author: Callan Moore
	********************/
	virtual ~DX10_Mesh() 
	{
		ReleasePtr(m_pVertexBuffer);
	}

	/***********************
	* Initialise: Initialise a new mesh
	* @author: Callan Moore
	* @parameter: _pRenderer: The renderer for the mesh
	* @parameter: _meshType: The type fo mesh to create
	* @parameter: _scale: 3D scalar for the mesh
	* @return: bool: Successful or not
	********************/
	bool DX10_Mesh::Initialise(DX10_Renderer* _pRenderer, eMeshType _meshType, v3float _scale)
	{
		// Save the renderer on the Rectangular Prism
		m_pRenderer = _pRenderer;
		m_scale = _scale;
		m_primTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		TVertexNormalUV* pVertexBuffer = 0;
		DWORD* pIndexBuffer = 0;	
		int stride = sizeof(TVertexNormalUV);

		// Load the Mesh File
		std::string fileName = GetFilePath(_meshType);
		VALIDATE(m_pRenderer->LoadMeshObj(fileName, pVertexBuffer, pIndexBuffer, &m_vertexCount, &m_indexCount, m_scale));

		// Create the buffer
		VALIDATE(m_pRenderer->CreateBuffer(pVertexBuffer, pIndexBuffer, m_vertexCount, m_indexCount, stride, m_pBuffer, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

		ReleasePtrArray(pVertexBuffer);
		ReleasePtrArray(pIndexBuffer);

		return true;
	}

	/***********************
	* InitialisePlane: Initialise a new mesh for a Plane
	* @author: Callan Moore
	* @parameter: _pRenderer: The renderer for the mesh
	* @parameter: _size: How many vertices to have on each side (Create a square plane)
	* @parameter: _scale: 3D scalar for the mesh
	* @return: bool: Successful or not
	********************/
	bool DX10_Mesh::InitialisePlane(DX10_Renderer* _pRenderer, int _size, v3float _scale)
	{
		m_pRenderer = _pRenderer;
		m_scale = _scale;
		m_primTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		m_vertexCount = (int)pow(_size, 2);
		m_pVertexBuffer = new TVertexNormalUV[m_vertexCount];
		int stride = sizeof(TVertexNormalUV);
	
		for (int i = 0; i < m_vertexCount; i++)
		{
			int row = (int)(i / _size);
			int col = i % _size;

			m_pVertexBuffer[i].pos = D3DXVECTOR3((((row * _size) - ((float)_size / 2.0f)) * m_scale.x), 0.0f, (((col * _size) - ((float)_size / 2.0f)) * m_scale.z));
			m_pVertexBuffer[i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_pVertexBuffer[i].uv = v2float(((float)row / (float)_size), ((float)col / (float)_size));
		}

		m_indexCount = (_size * 2) * (_size - 1) + (_size - 2);
		DWORD* pIndexBuffer = new DWORD[m_indexCount];
		int index = 0;

		// Cycle through all rows except last one
		for (int row = 0; row < _size - 1; row++)
		{
			if (row % 2 == 0)	// Even Row
			{
				// Even Rows go Right to Left for Clockwise winding order
				for (int col = (int)(_size - 1); col >= 0; col--)
				{
					pIndexBuffer[index++] = (col + (row * _size));
					pIndexBuffer[index++] = (col + ((row + 1) * _size));
				}

				// Add Degenerate triangle at end of each row
				if (row != _size - 2)
				{
					pIndexBuffer[index++] = (0 + ((row + 1) * _size));
				}
			}
			else	// Odd Row
			{
				// Even Rows go Left to Right for Clockwise winding order
				for (int col = 0; col < _size; col++)
				{
					pIndexBuffer[index++] = (col + (row * _size));
					pIndexBuffer[index++] = (col + ((row + 1) * _size));
				}

				// Add Degenerate triangle at end of each row
				if (row != _size - 2)
				{
					pIndexBuffer[index++] = ((_size - 1) + ((row + 1)  * _size));
				}
			}
		}

		// Create the buffer
		VALIDATE(m_pRenderer->CreateBuffer(m_pVertexBuffer, pIndexBuffer, m_vertexCount, m_indexCount, stride, m_pBuffer, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

		ReleasePtr(pIndexBuffer);

		return true;
	}

	/***********************
	* Render: Render the Mesh
	* @author: Callan Moore
	* @return: void
	********************/
	virtual void Render()
	{
		m_pRenderer->RenderBuffer(m_pBuffer);
	}
	
	/***********************
	* GetFilePath: Retrieve the file path of the mesh type
	* @author: Callan Moore
	* @parameter: _meshType: The type of mesh to retrieve the file path for
	* @return: std::string: File path
	********************/
	std::string GetFilePath(eMeshType _meshType)
	{
		switch (_meshType)
		{
			case MT_FINITEPLANE:
			{
				return "Resources/Meshes/Mesh_FinitePlane.txt";
			}
			break;
			case MT_RECTPRISM:
			{
				return "Resources/Meshes/Mesh_RectPrism.txt";
			}
			break;
			case MT_SPHERE:
			{
				return "Resources/Meshes/Mesh_Sphere.txt";
			}
			break;
			case MT_EPICTERRAIN:
			{
				return "Resources/Meshes/Mesh_EpicTerrain.txt";
			}
			break;
			case MT_WHARF:
			{
				return "Resources/Meshes/Mesh_Wharf.txt";
			}
			break;
			case MT_STARMAP:
			{
				return "Resources/Meshes/Mesh_StarMap.txt";
			}
				break;
			case MT_5X5:
			{
				return "Resources/Meshes/Mesh_5x5.txt";
			}
			break;
			default: return "";
		}	// End Switch
	}

	/***********************
	* GetPrimTopology: Retrieve the Primitive topology that the Mesh uses
	* @author: Callan Moore
	* @return: eVertexType: The Vertex Type
	********************/
	D3D_PRIMITIVE_TOPOLOGY GetPrimTopology() { return m_primTopology; };

	/***********************
	* GetScale: Gets the Scale of the Mesh
	* @author: Jc Fowles
	* @return: float: The Scale of the Mesh
	********************/
	v3float GetScale(){ return m_scale; };

	// TO DO CAL
	void DiamondSquare(int _iterNum)
	{
		if (_iterNum == 0)
		{
			ID3D10Buffer* pVertexBuff = m_pBuffer->GetVertexBuffer();
			void* verticesPtr;

			m_pVertexBuffer->pos.y = 20.0f;
			m_pVertexBuffer[2].pos.y = 30;

			HRESULT meh = pVertexBuff->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);
			//TVertexNormalUV* vert = new TVertexNormalUV();
			//vert->pos = { 20, 20, 20 };
			//vert->normal = { 0, 1, 0 };
		
			memcpy(verticesPtr, (void*)m_pVertexBuffer, sizeof(TVertexNormalUV) * m_vertexCount);
		
			pVertexBuff->Unmap();
		}
		else
		{
		
		}
	}


private:
	DX10_Renderer* m_pRenderer;
	DX10_Buffer* m_pBuffer;
	D3D_PRIMITIVE_TOPOLOGY m_primTopology;
	v3float m_scale;
	int m_vertexCount;
	int m_indexCount;

	TVertexNormalUV* m_pVertexBuffer;
};

#endif	// __DX10_MESH_GENERIC_H__

