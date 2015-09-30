/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_Water.h
* Description : 3D Lit Texture Water Object for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_Obj_Water.h"

DX10_Obj_Water::DX10_Obj_Water()
{
	DX10_Obj_Generic::BaseInitialise();

	// Nullify Pointers
	m_pShader = 0;

	m_reflectRefractScale = 0.5f;
}

DX10_Obj_Water::~DX10_Obj_Water()
{
	ReleasePtr(m_pTextures);
}

bool DX10_Obj_Water::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_Water* _pShader, std::vector<std::string>* _pTexNames, float _animationSpeed)
{
	if (_pRenderer == 0 || _pMesh == 0 || _pShader == 0 || _pTexNames == 0|| _animationSpeed <= 0)
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

	m_pTextures = new std::vector<ID3D10ShaderResourceView*>;
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

bool DX10_Obj_Water::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_Water* _pShader, std::string _texName)
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

	m_pTextures = new std::vector<ID3D10ShaderResourceView*>;
	ID3D10ShaderResourceView* pTempTex = 0;
	VALIDATE(m_pRenderer->CreateTexture(_texName, pTempTex));
	m_pTextures->push_back(pTempTex);

	return true;
}

void DX10_Obj_Water::Process(float _dt)
{
	BaseProcess(_dt);
}

void DX10_Obj_Water::Render(D3DXPLANE _reflectionPlane, eTech_Water _tech)
{
	TWater water;
	water.pMesh = m_pMesh;
	water.pMatWorld = &m_matWorld;
	water.pMapNormal = (*m_pTextures)[m_texIndex];
	water.pMatTexTranslation = &m_matTexTranslation;
	water.reflectRefractScale = m_reflectRefractScale;
	water.reflectionPlane = _reflectionPlane;

	m_pShader->Render(water, _tech);
}