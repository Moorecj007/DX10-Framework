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
	DX10_Mesh() 
	{
		m_initialisedDS = false;
	}

	/***********************
	* ~DX10_Mesh: Default Destructor for Mesh class
	* @author: Callan Moore
	********************/
	virtual ~DX10_Mesh() 
	{
		ReleasePtr(m_pVertexBuffer);
		ReleasePtr(m_pVertexBufferCloth);
		ReleasePtr(m_pIndexBuffer);
		ReleasePtr(m_pReadCornerIndices);
		ReleasePtr(m_pWriteCornerIndices);
		ReleasePtrArray(m_pVertexCalculated);
	}

	/***********************
	* Initialise: Initialise a new mesh
	* @author: Callan Moore
	* @parameter: _pRenderer: The renderer for the mesh
	* @parameter: _meshType: The type for mesh to create
	* @parameter: _scale: 3D scalar for the mesh
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, eMeshType _meshType, v3float _scale)
	{
		if (_pRenderer == 0)
		{
			return false;
		}
	
		// Save the renderer on the Rectangular Prism
		m_pRenderer = _pRenderer;
		m_scale = _scale;
		m_primTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		int stride = sizeof(TVertexNormalUV);
	
		// Load the Mesh File
		std::string fileName = GetFilePath(_meshType);
		VALIDATE(m_pRenderer->LoadMeshObj(fileName, m_pVertexBuffer, m_pIndexBuffer, &m_vertexCount, &m_indexCount, m_scale));
	
		// Create the buffer
		VALIDATE(m_pRenderer->CreateBuffer(m_pVertexBuffer, m_pIndexBuffer, m_vertexCount, m_indexCount, stride, m_pBuffer, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));
	
		return true;
	}
	
	/***********************
	* InitialiseCloth: Initialise a Mesh to be used as a cloth
	* @author: Callan Moore
	* @parameter: _pRenderer: The renderer for the mesh
	* @parameter: _pVertices: The Vertices list
	* @parameter: _pIndices: The Indices list
	* @parameter: _vertexCount: The number of vertices
	* @parameter: _indexCount: The number of indices
	* @parameter: _stride: The stride of the vertices
	* @parameter: _topology: The topology to be used
	* @parameter: _vertexUsage: The usage flag for creating the vertex buffer
	* @parameter: _indexUsage: The usage flag for creating the index buffer
	* @return: bool: Successful or not
	********************/
	template<typename TIndices, typename TVertices>
	bool InitialiseCloth(DX10_Renderer* _pRenderer, TVertices* _pVertices, TIndices* _pIndices, UINT _vertexCount, UINT _indexCount, UINT _stride, D3D10_PRIMITIVE_TOPOLOGY _topology, D3D10_USAGE _vertexUsage = D3D10_USAGE_IMMUTABLE, D3D10_USAGE _indexUsage = D3D10_USAGE_IMMUTABLE)
	{
		if (_pRenderer == 0 || _pVertices == 0 || _pIndices == 0)
		{
			return false;
		}

		m_pRenderer = _pRenderer;
		m_primTopology = _topology;
		int stride = _stride;
		m_indexCount = _indexCount;
		m_vertexCount = _vertexCount;
		m_pVertexBufferCloth = _pVertices;
		m_pIndexBuffer = _pIndices;

		// Create the buffer
		VALIDATE(m_pRenderer->CreateBuffer(m_pVertexBufferCloth, m_pIndexBuffer, m_vertexCount, m_indexCount, stride, m_pBuffer, _vertexUsage, _indexUsage));

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
	bool InitialisePlane(DX10_Renderer* _pRenderer, int _size, v3float _scale)
	{
		if (_pRenderer == 0)
		{
			return false;
		}

		m_pRenderer = _pRenderer;
		m_scale = _scale;
		m_primTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		m_size = _size;

		m_vertexCount = (int)pow(_size, 2);
		m_pVertexBuffer = new TVertexNormalUV[m_vertexCount];
		int stride = sizeof(TVertexNormalUV);
	
		for (int i = 0; i < m_vertexCount; i++)
		{
			int row = (int)(i / _size);
			int col = i % _size;

			m_pVertexBuffer[i].pos = D3DXVECTOR3(((col - (float)(_size - 1) / 2.0f) * m_scale.x), 0.0f, ((row - (float)(_size - 1) / 2.0f) * m_scale.z));
			m_pVertexBuffer[i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_pVertexBuffer[i].uv = v2float(((float)col / (float)_size), ((float)row / (float)_size));

		}

		m_indexCount = (_size * 2) * (_size - 1) + (_size - 2);
		m_pIndexBuffer = new DWORD[m_indexCount];
		int index = 0;

		// Cycle through all rows except last one
		for (int row = 0; row < _size - 1; row++)
		{
			if (row % 2 == 0)	// Even Row
			{
				// Even Row go Right to Left for Clockwise winding order
				for (int col = (int)(_size - 1); col >= 0; col--)
				{
					m_pIndexBuffer[index++] = (col + (row * _size));
					m_pIndexBuffer[index++] = (col + ((row + 1) * _size));
				}

				// Add Degenerate triangle at end of each Row
				if (row != _size - 2)
				{
					m_pIndexBuffer[index++] = (0 + ((row + 1) * _size));
				}
			}
			else	// Odd Row
			{
				// Even Row go Left to Right for Clockwise winding order
				for (int col = 0; col < _size; col++)
				{
					m_pIndexBuffer[index++] = (col + (row * _size));
					m_pIndexBuffer[index++] = (col + ((row + 1) * _size));
				}

				// Add Degenerate triangle at end of each Row
				if (row != _size - 2)
				{
					m_pIndexBuffer[index++] = ((_size - 1) + ((row + 1)  * _size));
				}
			}
		}

		// Create the buffer
		VALIDATE(m_pRenderer->CreateBuffer(m_pVertexBuffer, m_pIndexBuffer, m_vertexCount, m_indexCount, stride, m_pBuffer, D3D10_USAGE_DYNAMIC, D3D10_USAGE_DEFAULT));

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
	
	/***********************
	* GetVertexBuffer: Retrieve the Vertex Buffer of the Mesh
	* @author: Callan Moore
	* @return: TVertexNormalUV*: The array of vertices that make up the vertex buffer
	********************/
	TVertexNormalUV* GetVertexBuffer() { return m_pVertexBuffer; };

	/***********************
	* GetVertexBufferCloth: Retrieve the Vertex Buffer for Cloth of the Mesh
	* @author: Callan Moore
	* @return: TVertexNormalUV*: The array of vertices that make up the vertex buffer for Cloth
	********************/
	TVertexColor* GetVertexBufferCloth() { return m_pVertexBufferCloth; };

	/***********************
	* GetIndexBuffer: Retrieve the index Buffer of the Mesh
	* @author: Callan Moore
	* @return: TVertexNormalUV*: The array of indices that make up the Index buffer
	********************/
	DWORD* GetIndexBuffer() { return m_pIndexBuffer; };

	/***********************
	* DiamondSquareInit: Initialise the Mesh to be able to handle Diamond Square algorithm
	* @author: Callan Moore
	* @return: void
	********************/
	void DiamondSquareInit()
	{
		// Create the two vectors needed to calculate Diamond Square
		m_pWriteCornerIndices = new std::vector<UINT>;
		m_pReadCornerIndices = new std::vector<UINT>;

		m_pVertexCalculated = new bool[m_vertexCount];
		memset(m_pVertexCalculated, false, m_vertexCount);

		// Seed the four corners to half of the plane size
		m_pVertexBuffer[0].pos.y = (float)(m_size - 1) * m_scale.y / 2.0f;
		m_pVertexBuffer[m_size - 1].pos.y = (float)(m_size - 1) * m_scale.y / 2.0f;
		m_pVertexBuffer[m_vertexCount - m_size].pos.y = (float)(m_size - 1) * m_scale.y / 2.0f;
		m_pVertexBuffer[m_vertexCount - 1].pos.y = (float)(m_size - 1) * m_scale.y / 2.0f;

		// Push the First 4 corners into the Read Vector
		m_pReadCornerIndices->push_back(0);
		m_pReadCornerIndices->push_back(m_size - 1);
		m_pReadCornerIndices->push_back(m_vertexCount - m_size);
		m_pReadCornerIndices->push_back(m_vertexCount - 1);

		// Update the Buffer to reflect the changes
		UpdateBuffer();

		// Calculate the maximum amount of iterations based on the size
		m_iterationsLeft = (int)(log((float)m_size - 1.0f) / log(2.0f));
	}

	/***********************
	* DiamondSquare: Calculates the next step of the Diamond-Square algorithm
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool DiamondSquare()
	{
		// Check if the Initialization for the DS algorithm has happened
		if (m_initialisedDS == false)
		{
			DiamondSquareInit();
			m_initialisedDS = true;
			return true;
		}

		// Run only if they max iterations has not happened
		if (m_iterationsLeft > 0)
		{
			// Calculate how many squares need to be calculated each step
			int squares = m_pReadCornerIndices->size() / 4;

			// Calculate the current iteration
			float iters = (log((float)squares) / log(4.0f)) + 1.0f;

			// Calculate the roughness factor
			float roughness = ((1.0f / (iters)) * (((float)m_size - 1.0f) / 2.0f) * m_scale.y) * 0.90f;

			for (int i = 0; i < squares; i++)
			{
				// Read the next four corners from the Read Vector
				int offset = i * 4;
				UINT topLeft = (*m_pReadCornerIndices)[offset];
				UINT bottomLeft = (*m_pReadCornerIndices)[offset + 1];
				UINT topRight = (*m_pReadCornerIndices)[offset + 2];
				UINT bottomRight = (*m_pReadCornerIndices)[offset + 3];

				// Calculate the Centre	point	
				UINT centre = (topLeft + topRight + bottomLeft + bottomRight) / 4;

				// Calculate the side point
				UINT top = (topLeft + topRight) / 2;
				UINT left = (topLeft + bottomLeft) / 2;
				UINT bottom = (bottomLeft + bottomRight) / 2;
				UINT right = (topRight + bottomRight) / 2;

				// Update Y pos values
				m_pVertexBuffer[centre].pos.y += (m_pVertexBuffer[topLeft].pos.y + m_pVertexBuffer[topRight].pos.y + m_pVertexBuffer[bottomLeft].pos.y + m_pVertexBuffer[bottomRight].pos.y) / 4;

				//Check if the Left middle position of the square has already been calculated
				if (m_pVertexCalculated[left] == false)
				{
					float random = (float)(rand() % 1000 - 500) / 500;
					float modifier = random * roughness;

					m_pVertexBuffer[left].pos.y += (m_pVertexBuffer[topLeft].pos.y + m_pVertexBuffer[topRight].pos.y) / 2 + modifier;
					m_pVertexCalculated[left] = true;
				}
				//Check if the top middle position of the square has already been calculated
				if (m_pVertexCalculated[top] == false)
				{
					float random = (float)(rand() % 1000 - 500) / 500;
					float modifier = random * roughness;

					m_pVertexBuffer[top].pos.y += (m_pVertexBuffer[topLeft].pos.y + m_pVertexBuffer[bottomLeft].pos.y) / 2 + modifier;
					m_pVertexCalculated[top] = true;
				}
				//Check if the right middle position of the square has already been calculated
				if (m_pVertexCalculated[right] == false)
				{
					float random = (float)(rand() % 1000 - 500) / 500;
					float modifier = random * roughness;

					m_pVertexBuffer[right].pos.y += (m_pVertexBuffer[bottomLeft].pos.y + m_pVertexBuffer[bottomRight].pos.y) / 2 + modifier;
					m_pVertexCalculated[right] = true;
				}
				//Check if the bottom middle position of the square has already been calculated
				if (m_pVertexCalculated[bottom] == false)
				{
					float random = (float)(rand() % 1000 - 500) / 500;
					float modifier = random * roughness;

					m_pVertexBuffer[bottom].pos.y += (m_pVertexBuffer[topRight].pos.y + m_pVertexBuffer[bottomRight].pos.y) / 2 + modifier;
					m_pVertexCalculated[bottom] = true;
				}

				// Update the Buffer to reflect the changes
				UpdateBuffer();

				// Add all new points to Write Vector
				// New Square One
				m_pWriteCornerIndices->push_back(topLeft);
				m_pWriteCornerIndices->push_back(left);
				m_pWriteCornerIndices->push_back(top);
				m_pWriteCornerIndices->push_back(centre);

				// New Square Two
				m_pWriteCornerIndices->push_back(top);
				m_pWriteCornerIndices->push_back(centre);
				m_pWriteCornerIndices->push_back(topRight);
				m_pWriteCornerIndices->push_back(right);

				// New Square Three
				m_pWriteCornerIndices->push_back(left);
				m_pWriteCornerIndices->push_back(bottomLeft);
				m_pWriteCornerIndices->push_back(centre);
				m_pWriteCornerIndices->push_back(bottom);

				// New Square Four
				m_pWriteCornerIndices->push_back(centre);
				m_pWriteCornerIndices->push_back(bottom);
				m_pWriteCornerIndices->push_back(right);
				m_pWriteCornerIndices->push_back(bottomRight);
			}
			// Swap read and write vectors
			ReleasePtr(m_pReadCornerIndices);
			m_pReadCornerIndices = m_pWriteCornerIndices;

			// Recreate write vector variables
			m_pWriteCornerIndices = 0;
			m_pWriteCornerIndices = new std::vector < UINT > ;

			m_iterationsLeft--;
		}
		else
		{
			return false;
		}

		return true;
	}

	/***********************
	* UpdateBuffer: Update the Meshes Vertex buffer for the GPU
	* @author: Callan Moore
	* @return: void
	********************/
	void UpdateBuffer()
	{
		// retrieve the current Vertex Buffer used by the GPU
		ID3D10Buffer* pVertexBuff = m_pBuffer->GetVertexBuffer();
		void* verticesPtr;

		// Lock the memory
		pVertexBuff->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);

		//Copy the new Vertex Buffer into the pointer to the GPU Vertex Buffer
		memcpy(verticesPtr, (void*)m_pVertexBuffer, sizeof(TVertexNormalUV) * m_vertexCount);

		// Unlock the memory
		pVertexBuff->Unmap();
	}

	/***********************
	* UpdateBufferCloth: Update the Meshes Vertex buffer for Cloth for the GPU
	* @author: Callan Moore
	* @return: void
	********************/
	void UpdateBufferCloth()
	{
		// Retrieve the current Vertex Buffer used by the GPU
		ID3D10Buffer* pVertexBuff = m_pBuffer->GetVertexBuffer();
		void* verticesPtr;

		// Lock the memory
		pVertexBuff->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);

		//Copy the new Vertex Buffer into the pointer to the GPU Vertex Buffer
		memcpy(verticesPtr, (void*)m_pVertexBufferCloth, sizeof(TVertexColor) * m_vertexCount);

		// Unlock the memory
		pVertexBuff->Unmap();

		// Retrieve the current Index Buffer used by the GPU
		ID3D10Buffer* pIndexBuff = m_pBuffer->GetIndexBuffer();
		void* indicesPtr;

		// Lock the memory
		pIndexBuff->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&indicesPtr);

		//Copy the new Vertex Buffer into the pointer to the GPU Vertex Buffer
		memcpy(indicesPtr, (void*)m_pIndexBuffer, sizeof(DWORD) * m_indexCount);

		// Unlock the memory
		pIndexBuff->Unmap();
	}

	/***********************
	* ResetPlane: Reset a plane mesh to default values
	* @author: Callan Moore
	* @return: void
	********************/
	void ResetPlane()
	{
		for (int i = 0; i < m_vertexCount; i++)
		{
			m_pVertexBuffer[i].pos.y = 0.0f;
			m_pVertexBuffer[i].normal = { 0.0f, 1.0f, 0.0f };
		}
		UpdateBuffer();

		ReleasePtr(m_pWriteCornerIndices);
		ReleasePtr(m_pReadCornerIndices);
		ReleasePtrArray(m_pVertexCalculated);
		m_initialisedDS = false;
	}
	
	/***********************
	* Smooth: Apply the Smoothing algorithm to the vertex buffer and update it
	* @author: Callan Moore
	* @return: void
	********************/
	void Smooth()
	{
		// Calculates the average value of a vertex based on the (up to) 8 vertices around it and itself
		for (int i = 0; i < m_vertexCount; i++)
		{
			float additive = 0;
			int denom = 0;
			bool up = false;
			bool left = false;
			bool right = false;
			bool down = false;

			// Up
			if (i % m_size != 0)
			{
				// Add the vertex value above the current vertex
				additive += m_pVertexBuffer[i - 1].pos.y;
				up = true;
				denom++;
			}

			// Left
			if (i - m_size >= 0)
			{
				// Add the vertex value to the left of the current vertex
				additive += m_pVertexBuffer[i - m_size].pos.y;
				left = true;
				denom++;
			}

			// Right
			if (i + m_size <= (m_vertexCount - 1))
			{
				// Add the vertex value to the right of the current vertex
				additive += m_pVertexBuffer[i + m_size].pos.y;
				right = true;
				denom++;
			}

			// Down
			if (i % m_size != (m_size - 1))
			{
				// Add the vertex value below the current vertex
				additive += m_pVertexBuffer[i + 1].pos.y;
				down = true;
				denom++;
			}

			// Up and Left (Diagonal)
			if (up == true && left == true)
			{
				// Add the vertex value above and to the left of the current vertex
				additive += m_pVertexBuffer[i - m_size - 1].pos.y;
				denom++;
			}

			// Up and Right (Diagonal)
			if (up == true && right == true)
			{
				// Add the vertex value above and to the right of the current vertex
				additive += m_pVertexBuffer[i + m_size - 1].pos.y;
				denom++;
			}

			// Down and Left (Diagonal)
			if (down == true && left == true)
			{
				// Add the vertex value below and to the left of the current vertex
				additive += m_pVertexBuffer[i - m_size + 1].pos.y;
				denom++;
			}

			// Down and right (Diagonal)
			if (down == true && right == true)
			{
				// Add the vertex value below and to the right of the current vertex
				additive += m_pVertexBuffer[i + m_size + 1].pos.y;
				denom++;
			}

			// Add Self
			additive += m_pVertexBuffer[i].pos.y;
			denom++;

			// Calculate new Vertex Y pos
			m_pVertexBuffer[i].pos.y = additive / (float)denom;
		}

		CalculateNormals();
	}

	/***********************
	* CalculateNormals: Calculate the normal for each vertex
	* @author: Callan Moore
	* @return: void
	********************/
	void CalculateNormals()
	{
		D3DXVECTOR3* normalBuffer = new D3DXVECTOR3[m_vertexCount];
		memset(normalBuffer, 0, sizeof(D3DXVECTOR3) * m_vertexCount);

		for (int i = 2; i < m_indexCount; i++)
		{
			// Get the 3 vertices that make up the polygon
			D3DXVECTOR3 vert1 = m_pVertexBuffer[m_pIndexBuffer[i - 2]].pos;
			D3DXVECTOR3 vert2 = m_pVertexBuffer[m_pIndexBuffer[i - 1]].pos;
			D3DXVECTOR3 vert3 = m_pVertexBuffer[m_pIndexBuffer[i]].pos;

			// Get the direction vectors of the triangle
			D3DXVECTOR3 vec1 = vert1 - vert3;
			D3DXVECTOR3 vec2 = vert2 - vert3;

			// Check that the triangle is not degenerate
			if (D3DXVec3Length(&vec1) != 0.0f && D3DXVec3Length(&vec2) != 0.0f)
			{
				D3DXVECTOR3 normal;
				// Dependent on the direction of the triangle strip (based on column) determine the correct order for cross product
				if ((m_pIndexBuffer[i] / m_size) % 2 == 0)	// Column is even
				{
					D3DXVec3Cross(&normal, &vec1, &vec2);
				}
				else // Column is odd
				{
					D3DXVec3Cross(&normal, &vec2, &vec1);
				}
				D3DXVec3Normalize(&normal, &normal);

				// Add the normal to the normal collection for each vertex that makes this polygon
				normalBuffer[m_pIndexBuffer[i - 2]].x += normal.x;
				normalBuffer[m_pIndexBuffer[i - 2]].y += normal.y;
				normalBuffer[m_pIndexBuffer[i - 2]].z += normal.z;

				normalBuffer[m_pIndexBuffer[i - 1]].x += normal.x;
				normalBuffer[m_pIndexBuffer[i - 1]].y += normal.y;
				normalBuffer[m_pIndexBuffer[i - 1]].z += normal.z;

				normalBuffer[m_pIndexBuffer[i]].x += normal.x;
				normalBuffer[m_pIndexBuffer[i]].y += normal.y;
				normalBuffer[m_pIndexBuffer[i]].z += normal.z;
			}
		}

		// Normalise each normal collection and store on the Vertices
		for (int i = 0; i < m_vertexCount; i++)
		{
			D3DXVec3Normalize(&m_pVertexBuffer[i].normal, &normalBuffer[i]);
		}

		// Update the Meshes buffer to reflect changes
		UpdateBuffer();
		ReleasePtrArray(normalBuffer);
	}

private:
	DX10_Renderer* m_pRenderer;
	DX10_Buffer* m_pBuffer;
	D3D_PRIMITIVE_TOPOLOGY m_primTopology;
	v3float m_scale;
	int m_vertexCount;
	int m_indexCount;

	TVertexNormalUV* m_pVertexBuffer;
	TVertexColor* m_pVertexBufferCloth;
	DWORD* m_pIndexBuffer;
	int m_size;
	std::vector<UINT>* m_pWriteCornerIndices;
	std::vector<UINT>* m_pReadCornerIndices;
	bool* m_pVertexCalculated;
	bool m_initialisedDS;
	int m_iterationsLeft;
};

#endif	// __DX10_MESH_GENERIC_H__

