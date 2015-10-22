/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_LitTex.h
* Description : 3D Lit Texture Object for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_Obj_LitTex.h"

DX10_Obj_LitTex::DX10_Obj_LitTex()
{
	DX10_Obj_Generic::BaseInitialise();

	// Nullify Pointers
	m_pShader = 0;
	m_pTextures = 0;
}

DX10_Obj_LitTex::~DX10_Obj_LitTex()
{
	ReleasePtr(m_pTextures);
}

bool DX10_Obj_LitTex::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, std::vector<std::string>* _pTexNames, float _animationSpeed)
{
	if (_pRenderer == 0 || _pMesh == 0 || _pShader == 0 || _pTexNames == 0 || _animationSpeed <= 0)
	{
		// If any pointers are NULL, Object cannot be initialized
		return false;
	}

	if (_pTexNames->size() == 0)
	{
		// No file names for textures attached
		return false;
	}

	// Assign Member Variables
	m_pRenderer = _pRenderer;
	m_pMesh = _pMesh;
	m_pShader = _pShader;
	m_texIndex = 0;
	m_animationSpeed = _animationSpeed;
	m_animating = true;

	// Create each Texture as a ShaderResourceViews
	m_pTextures = new std::vector < ID3D10ShaderResourceView* > ;
	for (UINT i = 0; i < _pTexNames->size(); i++)
	{
		ID3D10ShaderResourceView* pTempTex = 0;
		VALIDATE(m_pRenderer->CreateTexture((*_pTexNames)[i], pTempTex));
		m_pTextures->push_back(pTempTex);
	}
	// Delete allocated memory
	ReleasePtr(_pTexNames);

	return true;
}

bool DX10_Obj_LitTex::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, std::string _texName)
{
	if (_pRenderer == 0 || _pMesh == 0 || _pShader == 0)
	{
		// If any pointers are NULL, Object cannot be initialized
		return false;
	}

	// Assign Member Variables
	m_pRenderer = _pRenderer;
	m_pMesh = _pMesh;
	m_pShader = _pShader;

	// Create the Texture as a ShaderResourceView
	m_pTextures = new std::vector < ID3D10ShaderResourceView* > ;
	ID3D10ShaderResourceView* pTempTex = 0;
	VALIDATE(m_pRenderer->CreateTexture(_texName, pTempTex));
	m_pTextures->push_back(pTempTex);

	return true;
}

void DX10_Obj_LitTex::Process(float _dt)
{
	BaseProcess(_dt);
}

void DX10_Obj_LitTex::Render(eTech_LitTex _tech, D3DXPLANE _plane)
{
	// Create a Structure to store Object orientated variables for the shader
	TLitTex litTex;

	// Add the variables
	litTex.pMesh = m_pMesh;
	litTex.pTexBase = (*m_pTextures)[m_texIndex];
	litTex.plane = _plane;

	D3DXMATRIX* pMatWorldTemp = new D3DXMATRIX();
	if (_tech == TECH_LITTEX_REFLECT)
	{
		// Apply Rasterizer state for the Reflection Technique
		m_pRenderer->ApplyRasterizerState(RS_REFLECTION);

		// Create a reflection matrix based of the input plane
		D3DXMATRIX matReflection = CreateReflectionMatrix(_plane);

		// Reflect the  objects world matrix by the reflection matrix to get a reflected World matrix
		*pMatWorldTemp = m_matWorld * matReflection;
		litTex.pMatWorld = pMatWorldTemp;
	}
	else
	{
		litTex.pMatWorld = &m_matWorld;
	}

	// Pass in the struct and Render the Object using the Shader
	m_pShader->Render(litTex, _tech);

	// Release created memory
	ReleasePtr(pMatWorldTemp);
}
