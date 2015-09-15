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
	m_pTextureID = 0;
	m_texIndex = 0;
	m_texTimer = 0;
}

DX10_Obj_LitTex::~DX10_Obj_LitTex()
{
	ReleasePtr(m_pTextureID);
}

bool DX10_Obj_LitTex::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, std::vector<UINT>* _textureID, float _textureTime)
{
	if (_pRenderer == 0 || _pMesh == 0 || _pShader == 0 || _textureID == 0|| _textureTime < 0)
	{
		// If any pointers are NULL, Object cannot be initialized
		return false;
	}

	if (_textureTime == 0.0f && _textureID->size() != 1)
	{
		return false;
	}

	// Assign Member Variables
	m_pRenderer = _pRenderer;
	m_pMesh = _pMesh;
	m_pShader = _pShader;
	m_pTextureID = _textureID;
	m_texIndex = 0;
	m_texTimer = _textureTime;

	return true;
}

bool DX10_Obj_LitTex::Initialise(DX10_Renderer* _pRenderer, DX10_Mesh_Generic* _pMesh, DX10_Shader_LitTex* _pShader, UINT _textureID)
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
	m_pTextureID = new std::vector<UINT>;
	m_pTextureID->push_back(_textureID);

	return true;
}

void DX10_Obj_LitTex::Process(float _dt)
{
	BaseProcess(_dt);

	m_timeElapsed += _dt;
	m_texIndex = (int)(m_timeElapsed * (float)m_pTextureID->size() / m_texTimer);

	if (m_timeElapsed >= m_texTimer)
	{
		m_timeElapsed -= m_texTimer;
		m_texIndex = 0;
	}
}

void DX10_Obj_LitTex::Render(eTech_LitTex _tech)
{
	TLitTex litTex;
	litTex.pMesh = m_pMesh;
	litTex.pMatWorld = &m_matWorld;
	litTex.textureID = (*m_pTextureID)[m_texIndex];

	m_pShader->Render(litTex, _tech);
}
