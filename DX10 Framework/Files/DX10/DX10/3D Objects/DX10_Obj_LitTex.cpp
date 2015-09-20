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

	m_pShader = 0;
	m_pTextures = 0;
	m_texIndex = 0;
	m_animationSpeed = 0;
	m_animationTimer = 0;
	m_scrollSpeed = 0;
	m_scrollTimer = 0;

	m_animation = false;
	m_scrolling = false;
	D3DXMatrixTranslation(&m_matTexTranslation, 0, 0, 0);
}

DX10_Obj_LitTex::~DX10_Obj_LitTex()
{
	ReleasePtr(m_pTextures);
}

bool DX10_Obj_LitTex::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh* _pMesh, DX10_Shader_LitTex* _pShader, std::vector<std::string>* _pTexNames, float _animationSpeed)
{
	if (_pRenderer == 0 || _pMesh == 0 || _pShader == 0 || _pTexNames == 0|| _animationSpeed <= 0)
	{
		// If any pointers are NULL, Object cannot be initialized
		return false;
	}

	if (_pTexNames->size() == 0)
	{
		return false;
	}

	if (_pTexNames->size() > 1)
	{
		m_animation = true;
	}

	// Assign Member Variables
	m_pRenderer = _pRenderer;
	m_pMesh = _pMesh;
	m_pShader = _pShader;
	m_texIndex = 0;
	m_animationSpeed = _animationSpeed;

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

	m_pTextures = new std::vector<ID3D10ShaderResourceView*>;
	ID3D10ShaderResourceView* pTempTex = 0;
	VALIDATE(m_pRenderer->CreateTexture(_texName, pTempTex));
	m_pTextures->push_back(pTempTex);

	return true;
}

void DX10_Obj_LitTex::Process(float _dt)
{
	BaseProcess(_dt);

	if (m_animation == true)
	{
		m_animationTimer += _dt;
		m_texIndex = (int)(m_animationTimer * (float)m_pTextures->size() / m_animationSpeed);

		if (m_animationTimer >= m_animationSpeed)
		{
			m_animationTimer -= m_animationSpeed;
			m_texIndex = 0;
		}
	}

	if (m_scrolling == true)
	{
		m_scrollTimer += _dt;
		D3DXMatrixTranslation(&m_matTexTranslation, m_scrollTimer / m_scrollSpeed, 0, 0);

		if (m_scrollTimer >= m_scrollSpeed)
		{
			m_scrollTimer -= m_scrollSpeed;
		}
	}
}

void DX10_Obj_LitTex::Render(eTech_LitTex _tech)
{
	TLitTex litTex;
	litTex.pMesh = m_pMesh;
	litTex.pMatWorld = &m_matWorld;
	litTex.pTexBase = (*m_pTextures)[m_texIndex];
	litTex.pTexTranslation = &m_matTexTranslation;

	m_pShader->Render(litTex, _tech);
}
