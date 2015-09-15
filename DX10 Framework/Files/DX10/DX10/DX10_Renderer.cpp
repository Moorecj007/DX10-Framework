/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Renderer.cpp
* Description : Holds the DirectX10 Device and Renders all Objects created by the DirectX10 device
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_Renderer.h"

DX10_Renderer::DX10_Renderer()
{
	m_pDX10Device = 0;
	m_pDX10SwapChain = 0;
	m_pRenderTargetView = 0;
}

bool DX10_Renderer::Initialise(int _clientWidth, int _clientHeight, HWND _hWND)
{
	// Seed time
	srand((UINT)time(NULL));

	// Save Window Variables
	m_hWnd = _hWND;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	VALIDATE(InitialiseDeviceAndSwapChain());

	m_clearColor = YELLOW;

	//Initialise the ID Keys for the Maps
	m_nextEffectID = 0;
	m_nextTechniqueID = 0;
	m_nextInputLayoutID = 0;
	m_nextBufferID = 0;
	m_nextTextureID = 0;

	m_activeLight.dir = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	m_activeLight.ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	m_activeLight.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_activeLight.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

DX10_Renderer::~DX10_Renderer()
{
}

void DX10_Renderer::ShutDown()
{
	if (m_fullScreen == true)
	{
		m_pDX10SwapChain->SetFullscreenState(true, NULL);
	}

	// Delete the Graphics memory stored as DX10 Textures
	std::map<UINT, ID3D10ShaderResourceView*>::iterator iterTexture = m_texturesByID.begin();
	while (iterTexture != m_texturesByID.end())
	{
		ReleaseCOM(iterTexture->second);
		iterTexture++;
	}

	// Delete the Graphics memory stored as DX10 InputLayers
	std::map<UINT, ID3D10InputLayout*>::iterator iterInputLayout = m_inputLayouts.begin();
	while (iterInputLayout != m_inputLayouts.end())
	{
		ReleaseCOM(iterInputLayout->second);
		iterInputLayout++;
	}

	// Delete the Graphics memory stored as DX10 Effects
	std::map<UINT, ID3D10Effect*>::iterator iterFX = m_effectsByID.begin();
	while (iterFX != m_effectsByID.end())
	{
		ReleaseCOM(iterFX->second);
		iterFX++;
	}

	// Delete the Graphics memory stored as Buffers
	std::map<UINT, DX10_Buffer*>::iterator iterBuffers = m_buffers.begin();
	while (iterBuffers != m_buffers.end())
	{
		ReleasePtr(iterBuffers->second);
		iterBuffers++;
	}
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilBuffer);
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDX10SwapChain);
	ReleaseCOM(m_pRasterizerState);
	if (m_pDX10Device != 0)
	{
		m_pDX10Device->ClearState();
	}
	ReleaseCOM(m_pDX10Device);
}

bool DX10_Renderer::InitialiseDeviceAndSwapChain()
{
	// State the Driver to be of type HAL
	m_dx10DriverType = D3D10_DRIVER_TYPE_HARDWARE;

	// Fill out the DXGI Swap Chain Description structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = m_clientWidth;
	swapChainDesc.BufferDesc.Height = m_clientHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// multi sampling per pixel ( 4 sample only) and High quality
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 4;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Add Debug Info to the flags when run in Debug mode
	UINT createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif
	
	// Create the Device and Swap Chain
	VALIDATEHR(	D3D10CreateDeviceAndSwapChain(
				0,                 //default adapter
				m_dx10DriverType,
				0,                 // no software device
				createDeviceFlags,
				D3D10_SDK_VERSION,
				&swapChainDesc,
				&m_pDX10SwapChain,
				&m_pDX10Device));

	m_rasterizerDesc.CullMode = D3D10_CULL_BACK; 
	m_rasterizerDesc.FillMode = D3D10_FILL_SOLID;
	m_rasterizerDesc.FrontCounterClockwise = false;
	m_rasterizerDesc.DepthBias = true;
	m_rasterizerDesc.DepthBiasClamp = 0;
	m_rasterizerDesc.SlopeScaledDepthBias = 0;
	m_rasterizerDesc.DepthClipEnable = true;
	m_rasterizerDesc.ScissorEnable = false;
	m_rasterizerDesc.MultisampleEnable = false;
	m_rasterizerDesc.AntialiasedLineEnable = true;

	m_pDX10Device->CreateRasterizerState(&m_rasterizerDesc, &m_pRasterizerState);
	m_pDX10Device->RSSetState(m_pRasterizerState);
	
	// Invoke functionality that deals with changing size of the window
	VALIDATE(onResize());

	return true;
}

bool DX10_Renderer::onResize()
{
	// Release the old render target view before creating again
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilBuffer);

	// Resize the buffers of the Swap Chain and create the new render target view
	VALIDATEHR(m_pDX10SwapChain->ResizeBuffers(1, m_clientWidth, m_clientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Create a temporary Back buffer
	ID3D10Texture2D* pBackBuffer;

	// Retrieve the Back Buffer of the Swap Chain and create a Render Target View using the Back Buffer
	VALIDATEHR(m_pDX10SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
	VALIDATEHR(m_pDX10Device->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView));

	// Release the memory from the temporary Back Buffer
	ReleaseCOM(pBackBuffer);

	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_clientWidth;
	depthStencilDesc.Height = m_clientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 4; // multi sampling must match
	depthStencilDesc.SampleDesc.Quality = 4; // swap chain values.
	depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	VALIDATEHR(m_pDX10Device->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer));
	VALIDATEHR(m_pDX10Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView));
	
	// Bind the Render Target View to the output-merger stage of the pipeline
	m_pDX10Device->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	
	// Set the View Port for the Device
	D3D10_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = m_clientWidth;
	viewPort.Height = m_clientHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	
	// Binds the View port to the Rasterizer stage of the pipeline
	m_pDX10Device->RSSetViewports(1, &viewPort);

	// Calculate the new Projection Matrix
	CalcProjMatrix();

	

	return true;
}

void DX10_Renderer::ClearScreen()
{
	// Color Fill the entire Render Target View(Back Buffer) with the Renderers clear color
	m_pDX10Device->ClearRenderTargetView(m_pRenderTargetView, m_clearColor);
	
	// Present the Back Buffer with no synchronization
	m_pDX10SwapChain->Present(0, 0);
}

void DX10_Renderer::ToggleFullscreen()
{
	m_fullScreen = !m_fullScreen;
	m_pDX10SwapChain->SetFullscreenState(m_fullScreen, NULL);
	onResize();
}

void DX10_Renderer::ToggleFillMode()
{
	if (m_rasterizerDesc.FillMode == D3D10_FILL_SOLID)
	{
		m_rasterizerDesc.FillMode = D3D10_FILL_WIREFRAME;
	}
	else
	{
		m_rasterizerDesc.FillMode = D3D10_FILL_SOLID;
	}

	ReleaseCOM(m_pRasterizerState);
	m_pDX10Device->CreateRasterizerState(&m_rasterizerDesc, &m_pRasterizerState);
	m_pDX10Device->RSSetState(m_pRasterizerState);
}

bool DX10_Renderer::BuildFX(std::string _fxFileName, std::string _technique, UINT* _pFXID, UINT* _pTechID)
{	
	ID3D10Effect* pFX = 0;
	UINT fxID;
	UINT techID;

	// Checking if the Effects file is already loaded
	std::map<std::string, UINT>::iterator fxCheck;
	fxCheck = m_effectIDs.find(_fxFileName);

	// Check if the FX file has already been created
	if (fxCheck != m_effectIDs.end())
	{
		// FX file already exists, save the ID
		fxID = fxCheck->second;
		pFX = m_effectsByID.find(fxID)->second;
	}
	else
	{
		// Set the shader flags to enforce strictness
		DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
		#if defined( DEBUG ) || defined( _DEBUG )
			// Addition shader flags for information in DEBUG mode only
			shaderFlags |= D3D10_SHADER_DEBUG;
			shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
		#endif

		ID3D10Blob* compilationErrors;

		// Add the directory path from the DX10Render file to the stored FX files
		std::string fxfilePath = "Resources/FX/" + _fxFileName;

		// Create the FX file from the input file string
		VALIDATEHR(D3DX10CreateEffectFromFileA(fxfilePath.c_str(), 0, 0,
					"fx_4_0", shaderFlags, 0, m_pDX10Device, 0, 0, &pFX, &compilationErrors, 0));
		ReleaseCOM(compilationErrors);
		
		// Create the pairs to be inserted into the appropriate FX Maps
		fxID = ++m_nextEffectID;
		std::pair<std::string, UINT> fxPair(_fxFileName, fxID);
		std::pair<UINT, ID3D10Effect*> fxPairByID(fxID, pFX);

		// Insert the pairs into their respective Maps
		VALIDATE(m_effectIDs.insert(fxPair).second);
		VALIDATE(m_effectsByID.insert(fxPairByID).second);
	}

	// Adding the Technique to the Map

	// Retrieve the Technique IDs map using the FX ID to get a Map of all techniques for that particular FX file
	std::map<UINT, std::map<std::string, UINT>>::iterator fxIDCheck;
	fxIDCheck = m_techniqueIDs.find(fxID);

	// Check if the FX file already has techniques stored for it
	if (fxIDCheck != m_techniqueIDs.end())
	{
		// Search the Inner Map (of Techs by FX ID) to check if the Technique has already been created
		std::map<std::string, UINT>::iterator techIDCheck;
		techIDCheck = fxIDCheck->second.find(_technique);

		if (techIDCheck != fxIDCheck->second.end())
		{
			// Technique already exists, save the ID
			techID = fxCheck->second;
		}
		else
		{
			// Technique has not been created so Retrieve the Tech from the FX file
			ID3D10EffectTechnique* pTech = pFX->GetTechniqueByName(_technique.c_str());

			if (pTech == NULL)
			{
				// technique was not found
				return false;
			}

			// Create pairs to store in the Technique Maps
			techID = ++m_nextTechniqueID;
			std::pair<std::string, UINT> techPair(_technique, techID);
			std::pair<UINT, ID3D10EffectTechnique*> techPairByID(techID, pTech);

			// Insert the pairs into their respective Maps
			VALIDATE((&fxIDCheck->second)->insert(techPair).second);
			VALIDATE(m_techniquesByID.insert(techPairByID).second);
		}
	}
	else
	{
		// Technique has not been created so Retrieve the Tech from the FX file
		ID3D10EffectTechnique* pTech = pFX->GetTechniqueByName(_technique.c_str());

		// Create pairs to store in the Technique Maps
		techID = ++m_nextTechniqueID;
		std::map<std::string, UINT> innerTechMap;
		std::pair<std::string, UINT> innerTechPair(_technique, techID);
		VALIDATE(innerTechMap.insert(innerTechPair).second);

		std::pair<UINT, std::map<std::string, UINT>> outerTechMap(fxID, innerTechMap);
		VALIDATE(m_techniqueIDs.insert(outerTechMap).second);

		std::pair<UINT, ID3D10EffectTechnique*> techByIDPair(techID, pTech);
		VALIDATE(m_techniquesByID.insert(techByIDPair).second);
	}

	// Save the FX and Technique IDs in the memory passed by the Object.
	*_pFXID = fxID;
	*_pTechID = techID;
	return true;
}

ID3D10EffectVariable* DX10_Renderer::GetFXVariable(UINT _fxID, std::string _techVar)
{
	// Retrieve the FX pointer
	ID3D10Effect* pFX = m_effectsByID.find(_fxID)->second;

	// Retrieve the pointer to the variable and return it to the calling object
	return pFX->GetVariableByName(_techVar.c_str());
}

bool DX10_Renderer::CreateVertexLayout(D3D10_INPUT_ELEMENT_DESC* _vertexDesc, UINT _elementNum, UINT _techID, UINT* _vertexLayoutID, UINT _passNum)
{
	// Find the Technique using the ID
	ID3D10EffectTechnique* pTech = m_techniquesByID.find(_techID)->second;
	ID3D10InputLayout* pVertexLayout;

	// Create the input layout
	D3D10_PASS_DESC passDesc;
	pTech->GetPassByIndex(_passNum)->GetDesc(&passDesc);

	VALIDATEHR(m_pDX10Device->CreateInputLayout(_vertexDesc, _elementNum, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &pVertexLayout));

	// Add the Vertex Layout to the Map
	UINT inputLayerID = ++m_nextInputLayoutID;
	std::pair<UINT, ID3D10InputLayout*> inputLayerPair(inputLayerID, pVertexLayout);
	VALIDATE(m_inputLayouts.insert(inputLayerPair).second);

	// Store the ID in for the calling object
	*_vertexLayoutID = inputLayerID;
	return true;
}

bool DX10_Renderer::CreateTexture(std::string _texFileName, UINT* _pTexID)
{
	ID3D10Effect* pTexture = 0;
	UINT texID;

	// Look for the texture by name to see if it is already loaded
	std::map<std::string, UINT>::iterator texCheck;
	texCheck = m_textureIDs.find(_texFileName);

	// Check if the Texture exists
	if (texCheck != m_textureIDs.end())
	{
		// Texture is already loaded. Save its ID
		texID = texCheck->second;
	}
	else
	{
		// Texture is new, create and save.
		texID = ++m_nextTextureID;

		ID3D10ShaderResourceView* pTexture;
		std::string filePath = "Resources/Textures/";
		filePath.append(_texFileName);

		VALIDATEHR(D3DX10CreateShaderResourceViewFromFileA(m_pDX10Device,
			filePath.c_str(), 0, 0, &pTexture, 0));

		std::pair<std::string, UINT> texNameID(_texFileName, texID);
		std::pair<UINT, ID3D10ShaderResourceView*> texIDResource(texID, pTexture);

		VALIDATE(m_textureIDs.insert(texNameID).second);
		VALIDATE(m_texturesByID.insert(texIDResource).second);
	}

	*_pTexID = texID;
	return true;
}

bool DX10_Renderer::RenderMesh(UINT _bufferID)
{
	// Retrieve the Buffer
	std::map<UINT, DX10_Buffer*>::iterator iterBuffer = m_buffers.find(_bufferID);
	if (iterBuffer == m_buffers.end())
	{
		return false;
	}
	iterBuffer->second->Render();
	return true;
}

bool DX10_Renderer::RenderSprite(UINT _bufferID)
{
	

	return true;
}

void DX10_Renderer::StartRender()
{
	m_pDX10Device->ClearRenderTargetView(m_pRenderTargetView, m_clearColor);
	m_pDX10Device->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
}

void DX10_Renderer::EndRender()
{
	m_pDX10SwapChain->Present(0, 0);
}

void DX10_Renderer::RestoreDefaultDrawStates()
{
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDX10Device->OMSetDepthStencilState(0, 0);
	m_pDX10Device->OMSetBlendState(0, blendFactors, 0xFFFFFFFF);	
}

void DX10_Renderer::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY _primitiveType)
{
	m_pDX10Device->IASetPrimitiveTopology(_primitiveType);
}

bool DX10_Renderer::SetInputLayout(UINT _vertexLayoutID)
{
	// Retrieve the Vertex Layout
	std::map<UINT, ID3D10InputLayout*>::iterator iterVLayout = m_inputLayouts.find(_vertexLayoutID);
	if (iterVLayout == m_inputLayouts.end())
	{
		return false;
	}
	ID3D10InputLayout* pVertexLayout = iterVLayout->second;

	m_pDX10Device->IASetInputLayout(pVertexLayout);
	return true;
}

void DX10_Renderer::SetViewMatrix(D3DXMATRIX _view)
{
	m_matView = _view;
}

ID3D10Buffer* DX10_Renderer::GetVertexBuffer(UINT _buffID)
{
	// Retrieve the Technique
	std::map<UINT, DX10_Buffer*>::iterator iterBuff = m_buffers.find(_buffID);
	if (iterBuff == m_buffers.end())
	{
		return NULL;
	}
	return iterBuff->second->GetVertexBuffer();
}

ID3D10EffectTechnique* DX10_Renderer::GetTechnique(UINT _techID)
{
	// Retrieve the Technique
	std::map<UINT, ID3D10EffectTechnique*>::iterator iterTech = m_techniquesByID.find(_techID);
	if (iterTech == m_techniquesByID.end())
	{
		return NULL;
	}
	return iterTech->second;
}

ID3D10ShaderResourceView* DX10_Renderer::GetTexture(UINT _texID)
{
	// Retrieve the Texture
	std::map<UINT, ID3D10ShaderResourceView*>::iterator iterTex = m_texturesByID.find(_texID);
	if (iterTex == m_texturesByID.end())
	{
		return NULL;
	}
	return iterTex->second;
}

void DX10_Renderer::CalcProjMatrix()
{
	float aspect = float(m_clientWidth) / m_clientHeight;
	D3DXMatrixPerspectiveFovLH(&m_matProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}
