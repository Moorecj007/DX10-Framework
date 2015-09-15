/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_Utilities.h
* Description : Extra functionality for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

//Inclusion Guards
#pragma once
#ifndef __DX10_UTILITIES_H__
#define __DX10_UTILITIES_H__
#pragma message("Included DX10_Utilities.h")

// Library Includes
#include <d3d10.h>
#include <d3dx10.h>
#include <dxerr.h>

// Local Includes
#include "../../Utility/Utilities.h"

// Defines
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// Macros
#define ReleaseCOM(x) { if (x){ x->Release(); x = 0; } }

// Useful Macro for Validating a HRESULT returning 'OK'
#if defined(DEBUG) | defined(_DEBUG)
#ifndef VALIDATEHR
#define VALIDATEHR(x)		                                   \
	{														   \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
			return false;									   \
		}                                                      \
	}
#endif
#else
	#ifndef VALIDATEHR
	#define VALIDATEHR(x) (x)
	#endif
#endif 

// Constants
const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);

// Structs
struct Light
{
	Light()
	{
		// Ensure the Memory is zeroed on creation
		ZeroMemory(this, sizeof(Light));
	}

	D3DXVECTOR3 pos;
	float pad1;      // not used
	D3DXVECTOR3 dir;
	float pad2;      // not used
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 attenuation;
	float spotPower;
};


#endif // __DX10_UTILITIES_H__