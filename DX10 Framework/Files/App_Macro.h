/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : App_Macro.h
* Description : The Defines and includes for the start of the Application
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __APP_MACRO_H__
#define __APP_MACRO_H__

// Library Files
#pragma comment(lib, "Winmm.lib")

// Defines and Macros
#define WIN32_LEAN_AND_MEAN
#define WINDOW_CLASS_NAME L"DX10 FRAMEWORK"

#ifdef _DEBUG
	// Visual Leak Detector to be run only if in DEBUG mode
	#pragma comment(lib, "vld.lib")
	#include <vld.h>
	#define D3D_DEBUG_INFO
#endif // _DEBUG

#endif	// __APP_MACRO_H__