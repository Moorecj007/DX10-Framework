/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Shader_Cloth.cpp
* Description : Shader Instructions for a the Cloth shader
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_SHADER_CLOTH_H__
#define __DX10_SHADER_CLOTH_H__

// Local Includes
#include "../../../Utility/Utilities.h"
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"

/***********************
* TCloth: Shader Variables for the Cloth FX
* @author: Callan Moore
********************/
struct TCloth
{
	DX10_Mesh* pMesh;
	D3DXMATRIX* pMatWorld;
};

enum eTech_Cloth
{
	TECH_CLOTH_STANDARD
};

class DX10_Shader_Cloth
{
public:

	/***********************
	* DX10_Shader_Cloth: Default Constructor for Cloth Shader class
	* @author: Callan Moore
	********************/
	DX10_Shader_Cloth()
	{
	}

	/***********************
	* ~DX10_Shader_Cloth: Default Destructor for Cloth Shader class
	* @author: Callan Moore
	********************/
	~DX10_Shader_Cloth()
	{
	}

	/***********************
	* Initialise: Initialise the shader for use
	* @author: Callan Moore
	* @parameter: _pDX10_Renderer: DX10 Renderer for this application
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pDX10_Renderer)
	{
		m_pDX10_Renderer = _pDX10_Renderer;

		VALIDATE(BuildFX());
		VALIDATE(CreateFXVarPointers());
		VALIDATE(CreateVertexLayout());

		return true;
	}

	/***********************
	* SetUpPerFrame: Setup the shader file with the variables used for the whole frame
	* @author: Callan Moore
	* @return: void
	********************/
	void SetUpPerFrame()
	{
		m_pEyePos->SetRawValue(m_pDX10_Renderer->GetEyePos(), 0, sizeof(D3DXVECTOR3));
		m_pMatView->SetMatrix((float*)m_pDX10_Renderer->GetViewMatrix());
		m_pMatProj->SetMatrix((float*)m_pDX10_Renderer->GetProjMatrix());
	}

	/***********************
	* Render: Ready the shader technique with object specific details and setting the objects mesh to render
	* @author: Callan Moore
	* @parameter: _cloth: Structure containing all details for a Cloth object
	* @parameter: _eTech: Technique Identifier to determine which technique to use
	* @return: void
	********************/
	void Render(TCloth _cloth, eTech_Cloth _eTech = TECH_CLOTH_STANDARD)
	{
		// Load the correct Technique onto the Current pointers
		SetCurrentPtrs(_eTech);

		// Set the Renderer Input layout and primitive topology to be the correct ones for this shader
		m_pDX10_Renderer->SetInputLayout(m_pCurrentVertexLayout);
		m_pDX10_Renderer->SetPrimitiveTopology(_cloth.pMesh->GetPrimTopology());

		if (m_pCurrentTech != NULL)
		{
			// Get the Current Technique and loop through each pass
			D3D10_TECHNIQUE_DESC techDesc;
			m_pCurrentTech->GetDesc(&techDesc);
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				D3DXMATRIX matWorld = *_cloth.pMatWorld;
				m_pMatWorld->SetMatrix((float*)&matWorld);

				// Apply the variables and send them to the GPU
				m_pCurrentTech->GetPassByIndex(p)->Apply(0);

				// Render the Objects Mesh using the loaded technique
				_cloth.pMesh->Render();
			}
		}

		// Restore the Default Rendering States in case any were changed
		m_pDX10_Renderer->RestoreDefaultRenderStates();
	}

private:

	/***********************
	* BuildFX: Build the FX file for the Shader
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool BuildFX()
	{
		VALIDATE(m_pDX10_Renderer->BuildFX("Cloth.fx", "StandardTech", m_pFX, m_pTech_Standard));

		return true;
	}

	/***********************
	* CreateFXVarPointers: Create the pointers to the FX memory on the graphics card
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool CreateFXVarPointers()
	{
		// Per Frame	

		m_pEyePos = m_pFX->GetVariableByName("g_eyePosW");
		m_pMatView = m_pFX->GetVariableByName("g_matView")->AsMatrix();
		m_pMatProj = m_pFX->GetVariableByName("g_matProj")->AsMatrix();

		// Per Object	
		m_pMatWorld = m_pFX->GetVariableByName("g_matWorld")->AsMatrix();

		VALIDATE(m_pEyePos != 0);
		VALIDATE(m_pMatView != 0);
		VALIDATE(m_pMatProj != 0);
		VALIDATE(m_pMatWorld != 0);

		return true;
	}

	/***********************
	* CreateVertexLayout: Create the vertex layout for this shader
	* @author: Callan Moore
	* @return: bool: Successful or not
	********************/
	bool CreateVertexLayout()
	{
		// Vertex Desc for a basic vertex with Normals and UV coordinates
		D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT elementNum = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

		m_pDX10_Renderer->CreateVertexLayout(vertexDesc, elementNum, m_pTech_Standard, m_pVertexLayout_Standard);

		return true;
	}

	/***********************
	* SetCurrentPtrs: Set the Current Vertex Layout and Technique pointers
	* @author: Callan Moore
	* @parameter: _tech: Enumerator to determine which set of pointers to use as current
	* @return: void
	********************/
	void SetCurrentPtrs(eTech_Cloth _tech)
	{
		switch (_tech)
		{
		case TECH_CLOTH_STANDARD:
		{
			m_pCurrentVertexLayout = m_pVertexLayout_Standard;
			m_pCurrentTech = m_pTech_Standard;
		}
		break;
		default:
		{
			m_pCurrentVertexLayout = 0;
			m_pCurrentTech = 0;
		}
		}	// End Switch
	}

private:

	DX10_Renderer* m_pDX10_Renderer;
	ID3D10Effect* m_pFX;

	ID3D10InputLayout* m_pCurrentVertexLayout;
	ID3D10InputLayout* m_pVertexLayout_Standard;

	ID3D10EffectTechnique* m_pCurrentTech;
	ID3D10EffectTechnique* m_pTech_Standard;

	ID3D10EffectVariable*				m_pEyePos;
	ID3D10EffectMatrixVariable*			m_pMatView;
	ID3D10EffectMatrixVariable*			m_pMatProj;
	ID3D10EffectMatrixVariable*			m_pMatWorld;
};

#endif	// __DX10_SHADER_CLOTH__