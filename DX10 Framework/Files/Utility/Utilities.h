/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Utilities.h
* Description : Extra functionality
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

//Inclusion Guards
#pragma once
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

// Library Includes
#include <sstream>

// Defines
#define ReleasePtr(x) { if(x){ delete x; x = 0;} }
#define ReleasePtrArray(x) { if(x){ delete[] x; x = 0;} }

// Handy Define to determine that a function was successful
#if defined(DEBUG) | defined(_DEBUG)
#define VALIDATE(x) {											\
	if (!x) {													\
		std::ostringstream strStream;							\
		strStream << "Validate Failed: \n";						\
		strStream << "File: " << __FILE__ << "\n";				\
		strStream << "Function: " << __FUNCTION__ << "\n";		\
		strStream << "Line: " << __LINE__ << "\n";				\
		OutputDebugStringA(strStream.str().c_str());				\
		return (false);											\
	}															\
}
#else
	#define VALIDATE(x) if(!x) return false
#endif

// Library Includes
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <vector>

// Local Includes
#include "Utility_Math.h"

/***********************
* Increment: Increment an input value by the given amount and return the new value
* @author: Callan Moore
* @parameter: _value: Value to increment
* @parameter: _amount: Increment amount
* @return: T: The result of the increment as the same type as given
********************/
template<typename T>
inline T Increment(T* _value, T _amount)
{
	*_value += _amount;
	return *_value;
}

// Color constants in DWORD formatting
namespace color
{
	const DWORD BLACK(RGB(0, 0, 0));
	const DWORD WHITE(RGB(255, 255, 255));
	const DWORD RED(RGB(255, 0, 0));
	const DWORD GREEN(RGB(0, 255, 0));
	const DWORD CYAN(RGB(0, 255, 255));
	const DWORD YELLOW(RGB(255, 255, 0));
	const DWORD PURPLE(RGB(128, 0, 128));
	const DWORD DARKBLUE(RGB(0, 0, 139));
	const DWORD GREY(RGB(50, 50, 50));
	const DWORD PINK(RGB(199, 21, 133));
}

#endif // __UTILITIES_H__

