/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Obj_Color.cpp
* Description : 3D Color Object for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "DX10_Obj_Color.h"

DX10_Obj_Color::DX10_Obj_Color()
{
	DX10_Obj_Generic::BaseInitialise();
}

DX10_Obj_Color::~DX10_Obj_Color()
{
}

void DX10_Obj_Color::Process(float _dt)
{
	CalcWorldMatrix();
}

void DX10_Obj_Color::Render()
{
	//m_pRenderer->RestoreDefaultDrawStates();
	//m_pRenderer->SetInputLayout(m_vertexLayoutID);
	//ID3D10EffectTechnique* pTech = m_pRenderer->GetTechnique(m_techniqueID);
	//
	//if (pTech != NULL)
	//{
	//	D3D10_TECHNIQUE_DESC techDesc;
	//	pTech->GetDesc(&techDesc);
	//	for (UINT p = 0; p < techDesc.Passes; ++p)
	//	{
	//		m_pTechObjColor->SetFloatVector((float*)m_color);
	//		m_pTechMatWorld->SetMatrix((float*)m_matWorld);
	//		m_pTechMatView->SetMatrix((float*)*m_pRenderer->GetViewMatrix());
	//		m_pTechMatProj->SetMatrix((float*)*m_pRenderer->GetProjMatrix());
	//
	//		pTech->GetPassByIndex(p)->Apply(0);
	//		m_pMesh->Render();
	//	}
	//}
}

