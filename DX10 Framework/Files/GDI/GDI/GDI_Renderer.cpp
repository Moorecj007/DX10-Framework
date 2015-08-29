/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Renderer.cpp
* Description : A renderer using GDI for 2D graphics
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Renderer.h"

CGDI_Renderer::CGDI_Renderer()
{
}

CGDI_Renderer::~CGDI_Renderer()
{
	ReleasePtr(m_pBackBuffer);
}

bool CGDI_Renderer::Initialise(HWND _hWnd, HINSTANCE _hInstance, int _clientWidth, int _clientHeight)
{
	m_hWnd = _hWnd;
	m_hInstance = _hInstance;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Create and Initialise the Backbuffer
	m_pBackBuffer = new CBackBuffer();
	m_pBackBuffer->Initialise(m_hWnd, m_clientWidth, m_clientHeight);

	return true;
}

void CGDI_Renderer::Render(POINT* _pPoints, COLORREF _color, int _size)
{
	HDC hdc = m_pBackBuffer->GetBFDC();

	HBRUSH brush = CreateSolidBrush(_color);
	SelectObject(hdc, brush);

	// Draw the Object with a solif fill color
	Polygon(hdc, _pPoints, _size);
	DeleteObject(brush);

	HPEN hPenOld;
	// Draw a line
	HPEN hLinePen;
	
	// Draw a black outline onto the object
	hLinePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);
	
	for (int i = 0; i < _size; i++)
	{
		if (i == _size - 1)
		{
			MoveToEx(hdc, (int)_pPoints[i].x, (int)_pPoints[i].y, NULL);
			LineTo(hdc, (int)_pPoints[0].x, (int)_pPoints[0].y);
		}
		else
		{
			MoveToEx(hdc, (int)_pPoints[i].x, (int)_pPoints[i].y, NULL);
			LineTo(hdc, (int)_pPoints[i + 1].x, (int)_pPoints[i + 1].y);
		}
	}
	
	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

void CGDI_Renderer::BeginRender()
{
	m_pBackBuffer->Clear();
}

void CGDI_Renderer::EndRender()
{
	m_pBackBuffer->Present();
}