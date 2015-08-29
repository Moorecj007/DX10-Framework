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

DX10_Obj_LitTex::DX10_Obj_LitTex(DX10_Shader_LitTex* _pShader, std::vector<UINT>* _textureID, float _textureTime)
{
	DX10_Obj_Generic::BaseInitialise();

	m_pShader = _pShader;
	m_pTextureID = _textureID;
	m_texIndex = 0;
	m_texTimer = _textureTime;
}

DX10_Obj_LitTex::~DX10_Obj_LitTex()
{
	ReleasePtr(m_pTextureID);
}

void DX10_Obj_LitTex::Process(float _dt)
{
	CalcWorldMatrix();

	m_timeElapsed += _dt;
	m_texIndex = (int)(m_timeElapsed * (float)m_pTextureID->size() / m_texTimer);

	if (m_timeElapsed >= m_texTimer)
	{
		m_timeElapsed -= m_texTimer;
		m_texIndex = 0;
	}
}

void DX10_Obj_LitTex::Render()
{
	TLitTex litTex;
	litTex.pMesh = m_pMesh;
	litTex.pMatWorld = &m_matWorld;
	litTex.textureID = (*m_pTextureID)[m_texIndex];

	m_pShader->Render(litTex);
}
