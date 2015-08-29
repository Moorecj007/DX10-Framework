/*
* Bachelor of Software Engineering 
* Media Design School 
* Auckland 
* New Zealand 
 
* (c) 2005 - 2015 Media Design School 
 
* File Name : GDI_Backbuffer.h
* Description : Backbuffer for GDI operations
* Author :	Callan Moore
* Mail :	Callan.moore@mediadesign.school.nz
*/

#pragma once
#ifndef __GDI_BACKBUFFER_H__
#define __GDI_BACKBUFFER_H__

// Local Includes
#include "../../Utility/Utilities.h"

class CBackBuffer
{
public:

	/***********************
	* CBackBuffer: Default Constructor for BackBuffer class
	* @author: Callan Moore
	********************/
	CBackBuffer();

	/***********************
	* ~CBackBuffer: Default Destructor for BackBuffer class
	* @author: Callan Moore
	********************/
	~CBackBuffer();

	/***********************
	* Initialise: Initialise the Backbuffer for use
	* @author: Callan Moore
	* @parameter: _hWnd: Handle to the client window
	* @parameter: _clientWidth: Width of the client window
	* @parameter: _clientHeight: Height of the client window
	* @return: bool: Successful or not
	********************/
	bool Initialise(HWND _hWnd, int _clientWidth, int _clientHeight);
	
	/***********************
	* Clear: Clears the Backbuffer to an empty state
	* @author: Callan Moore
	* @return: void
	********************/
	void Clear();
	
	/***********************
	* Present: Presents the Backbuffer for display in the main window
	* @author: Callan Moore
	* @return: void
	********************/
	void Present();

	/***********************
	* GetBFDC: Retrieves the Backbuffers Handle to the Device Context (HDC)
	* @author: Callan Moore
	* @return: HDC: The HDC of the Backbuffer
	********************/
	HDC GetBFDC() const;
	
	/***********************
	* GetWidth: Retrieve the Width of the Backbuffer
	* @author: Callan Moore
	* @return: int: Width of the Backbuffer
	********************/
	int GetWidth() const;
	/***********************
	* GetHeight: Retrieve the Height of the Backbuffer
	* @author: Callan Moore
	* @return: int: Height of the Backbuffer
	********************/
	int GetHeight() const;
	
	
private:
	// Disallowing copies
	CBackBuffer(const CBackBuffer& _kr);
	CBackBuffer& operator= (const CBackBuffer& _kr);


private:
	// Client window variables
	HWND m_hWnd;
	HDC m_hDC;
	int m_iWidth;
	int m_iHeight;

	HBITMAP m_hSurface;
	HBITMAP m_hOldObject;
};

#endif // __GDI_BACKBUFFER_H__