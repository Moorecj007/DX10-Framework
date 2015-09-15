/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Renderer.h
* Description : Holds the DirectX10 Device and Renders all Objects created by the DirectX10 device
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_RENDERER_H__
#define __DX10_RENDERER_H__

// Library Includes
#include <map>
#include <typeinfo> 
#include <time.h>

// Local Includes
#include "DX10_Utilities.h"
#include "../../Utility/Utilities.h"
#include "DX10_Vertex.h"
#include "Systems/DX10_Buffer.h"

class DX10_Renderer
{
public:

	/***********************
	* CDX10Renderer: Default Constructor for DX10 Renderer class
	* @author: Callan Moore
	********************/
	DX10_Renderer();

	/***********************
	* Initialise: Initialise the DX10 Renderer
	* @author: Callan Moore
	* @Parameter: _clientWidth: Width of the client window
	* @Parameter: _clientHeight: Height of the client window
	* @Parameter: _hWND: Handle to the Application window
	* @return: bool: Successful or not
	********************/
	bool Initialise(int _clientWidth, int _clientHeight, HWND _hWND);

	/***********************
	* ~CDX10Renderer: Default Destructor for DX10 Renderer class
	* @author: Callan Moore
	********************/
	~DX10_Renderer();

	/***********************
	* ShutDown: Shuts down and releases all memory created by DX10 Renderer
	* @author: Callan Moore
	* @return: void
	********************/
	void ShutDown();

	/***********************
	* InitialiseDeviceAndSwapChain: Initialise the DX10 Device and Swap Chain
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool InitialiseDeviceAndSwapChain();

	/***********************
	* onResize: Recreate Depth and Render Views when resizing the application
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool onResize();

	/***********************
	* ClearScreen: Clears the screen to clear color
	* @author: Callan Moore
	* @return: void
	********************/
	void ClearScreen();

	/***********************
	* ToggleFullscreen: Toggle Full screen on and off
	* @author: Callan Moore
	* @return: void
	********************/
	void ToggleFullscreen();

	/***********************
	* ToggleFillMode: Toggle fill mode between solid and wireframe
	* @author: Callan Moore
	* @return: void
	********************/
	void ToggleFillMode();

	/***********************
	* BuildFX: Build a FX file and Technique and store on the Renderer
	* @author: Callan Moore
	* @Parameter: _fxFileName: Name of the Effects file to retrieve
	* @Parameter: _technique: Name of the Technique to Retrieve from the FX file
	* @Parameter: _fxID: Storage value to hold the created or found ID of the FX file
	* @Parameter: _techID: Storage value to hold the created or found ID of the Technique
	* @return: bool: Successful or not
	********************/
	bool BuildFX(std::string _fxFileName, std::string _technique, UINT* _pFXID, UINT* _pTechID);

	/***********************
	* GetFXVariable: Retrieve a FX Variable
	* @author: Callan Moore
	* @Parameter: _fxID: ID of the FX file to access
	* @Parameter: _techVar: Name of the variable to retrieve
	* @return: ID3D10EffectVariable*: The Retrieved Effect Variable
	********************/
	ID3D10EffectVariable* GetFXVariable(UINT _fxID, std::string _techVar);

	/***********************
	* CreateVertexLayout: Create the Vertex Layout for an Object
	* @author: Callan Moore
	* @parameter: _vertexDesc: Description of the Vertices's
	* @parameter: _elementCount: Number of elements in the Vertex Description
	* @parameter: _techID: Technique ID to base the layout on
	* @Parameter: _vertexLayoutID: Storage variable to hold the ID of the created Vertex Layout
	* @Parameter: _passNum: The pass number for the technique. Default to 0
	* @return: bool: Successful or not
	********************/
	bool CreateVertexLayout(D3D10_INPUT_ELEMENT_DESC* _vertexDesc, UINT _elementCount, UINT _techID, UINT* _pVertexLayoutID, UINT _passNum = 0);

	/***********************
	* CreateBuffer: Creates a buffer that holds all information for Vertex and Index Buffers for an Mesh
	* @author: Callan Moore
	* @parameter: _pVertices: The array of Vertices
	* @parameter: _pIndices: The array of Indices
	* @parameter: _vertCount: Number of Vertices
	* @parameter: _indexCount: Number of Indices
	* @parameter: _stride: Stride size for each Vertex
	* @parameter: _pBufferID: Storage variable to hold the ID of the created buffer
	* @return: bool: Successful or not
	********************/
	template<typename TIndices, typename TVertices>
	bool CreateBuffer(typename TVertices* _pVertices, typename TIndices* _pIndices,
		UINT _vertCount, UINT _indexCount, UINT _stride, UINT* _pBufferID,
		D3D10_USAGE _vertexUsage = D3D10_USAGE_IMMUTABLE, D3D10_USAGE _indexUsage = D3D10_USAGE_IMMUTABLE)
	{
		*_pBufferID = ++m_nextBufferID;

		DX10_Buffer* buff = new DX10_Buffer(m_pDX10Device);
		if (buff->Initialise(_pVertices, _pIndices, _vertCount, _indexCount, _stride, _pBufferID, _vertexUsage, _indexUsage))
		{
			std::pair<UINT, DX10_Buffer*> bufferPair(m_nextBufferID, buff);
			VALIDATE(m_buffers.insert(bufferPair).second);

			return true;
		}
		else
		{
			// Delete the failed buffer memory
			_pBufferID = 0;
			delete buff;
			buff = 0;
			return false;
		}
	}

	/***********************
	* CreateTexture: Create a Texture from a file and store it on the Renderer
	* @author: Callan Moore
	* @parameter: _texFileName: The filename of the texture
	* @parameter: _pTexID: Storage variable to hold the ID of the created Texture
	* @return: bool: Successful or not
	********************/
	bool CreateTexture(std::string _texFileName, UINT* _pTexID);

	/***********************
	* RenderObject: Renders an Object to the screen
	* @author: Callan Moore
	* @parameter: _bufferID: The ID of the buffer stored on the Renderer
	* @return: bool: Successful or not
	********************/
	bool RenderMesh(UINT _bufferID);

	/***********************
	* RenderObject: Renders an Object to the screen
	* @author: Callan Moore
	* @parameter: _bufferID: The ID of the buffer stored on the Renderer
	* @return: bool: Successful or not
	********************/
	bool RenderSprite(UINT _bufferID);

	/***********************
	* StartRender: Clears the Back buffer ready for new frame
	* @author: Callan Moore
	* @return: void
	********************/
	void StartRender();

	/***********************
	* EndRender: Presents the Back buffer
	* @author: Callan Moore
	* @return: void
	********************/
	void EndRender();

	/***********************
	* RestoreDefaultDrawStates: Restore the default states for drawing to ensure correct states
	* @author: Callan Moore
	* @return: void
	********************/
	void RestoreDefaultDrawStates();

	/***********************
	* SetPrimitiveTopology: Sets the primitive topology for a Mesh before drawing
	* @author: Callan Moore
	* @parameter: _primitiveType: The Primitive type/topology for the Mesh
	* @return: void
	********************/
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _primitiveType);

	/***********************
	* SetInputLayout: Set the Vertex Layout as the Input Layout on the Renderer
	* @author: Callan Moore
	* @parameter: _vertexLayoutID: Vertex Layout ID
	* @return: bool: Successful or not
	********************/
	bool SetInputLayout(UINT _vertexLayoutID);

	/***********************
	* SetViewMatrix: Set the View Matrix for use in Renderering
	* @author: Callan Moore
	* @parameter: _view: The new View matrix
	* @return: void
	********************/
	void SetViewMatrix(D3DXMATRIX _view);

	/***********************
	* SetEyePosition: Sets the Eye of the view (Camera position)
	* @author: Callan Moore
	* @parameter: D3DXVECTOR3 _eyePos: The position of the eye (Camera)
	* @return: void
	********************/
	void SetEyePosition(D3DXVECTOR3 _eyePos) { m_eyePos = _eyePos; };

	/***********************
	* GetTechnique: Retrieve the Technique for the given ID
	* @author: Callan Moore
	* @parameter: _techID: ID of the Technique
	* @return: ID3D10EffectTechnique*: DX10 Technique
	********************/
	ID3D10EffectTechnique* GetTechnique(UINT _techID);

	/***********************
	* SetTexture: Retrieve the Texture from the ID
	* @author: Callan Moore
	* @parameter: _texID: Texture ID
	* @return: ID3D10ShaderResourceView*: The Texture
	********************/
	ID3D10ShaderResourceView* GetTexture(UINT _texID);

	/***********************
	* GetVertexBuffer: Retrieve the vertex buffer
	* @author: Callan Moore
	* @parameter: _buffID: The ID to the package buffer
	* @return: ID3D10Buffer*: The vertex buffer linked to the ID
	********************/
	ID3D10Buffer* GetVertexBuffer(UINT _buffID);

	/***********************
	* CalcProjMatrix: Calculate the Projection Matrix for use in Renderering
	* @author: Callan Moore
	* @return: void
	********************/
	void CalcProjMatrix();
	
	/***********************
	* GetViewMatrix: Retrieve the View Matrix
	* @author: Callan Moore
	* @return: D3DXMATRIX*: The View Matrix
	********************/
	D3DXMATRIX* GetViewMatrix() { return &m_matView; };
	
	/***********************
	* GetProjMatrix: Retrieve the Projection Matrix
	* @author: Callan Moore
	* @return: D3DXMATRIX*: The Projection Matrix
	********************/
	D3DXMATRIX* GetProjMatrix() { return &m_matProj; };

	/***********************
	* GetFullScreenState: Retrieve the state of full screen
	* @author: Callan Moore
	* @return: bool: True if full screen is active, false otherwise
	********************/
	bool GetFullScreenState() { return m_fullScreen; }

	/***********************
	* GetEyePos: Retrieve the current Eye Position
	* @author: Callan Moore
	* @return: D3DXVECTOR3; The Eye Position
	********************/
	D3DXVECTOR3 GetEyePos() { return m_eyePos; };
	
	/***********************
	* GetActiveLight: Retrieve the current active light in the scene
	* @author: Callan Moore
	* @return: Light*: The current active light
	********************/
	Light* GetActiveLight() { return &m_activeLight; };

private:
	// Window Variables
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;
	bool m_fullScreen;

	// Matrices for Rendering
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_eyePos;

	// DX10 Variables
	ID3D10Device*    m_pDX10Device;
	IDXGISwapChain*  m_pDX10SwapChain;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	ID3D10Texture2D* m_pDepthStencilBuffer;
	D3D10_RASTERIZER_DESC m_rasterizerDesc;
	ID3D10RasterizerState* m_pRasterizerState;
	D3D10_DRIVER_TYPE m_dx10DriverType;
	D3DXCOLOR m_clearColor;

	// Map of FX files
	UINT m_nextEffectID;
	std::map<std::string, UINT> m_effectIDs;
	std::map<UINT, ID3D10Effect*> m_effectsByID;

	// Map of Techniques
	UINT m_nextTechniqueID;
	std::map<UINT, std::map<std::string, UINT>> m_techniqueIDs;
	std::map<UINT, ID3D10EffectTechnique*> m_techniquesByID;

	// Map of Input Layouts
	UINT m_nextInputLayoutID;
	std::map<UINT, ID3D10InputLayout*> m_inputLayouts;

	// Map of all Buffers
	UINT m_nextBufferID;
	std::map<UINT, DX10_Buffer*> m_buffers;

	// Map of all Textures
	UINT m_nextTextureID;
	std::map<std::string, UINT> m_textureIDs;
	std::map<UINT, ID3D10ShaderResourceView*> m_texturesByID;

	// Lighting
	Light m_activeLight;


};

#endif // __DX10_RENDERER_H__

