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

GDI_Renderer::GDI_Renderer()
{
}

GDI_Renderer::~GDI_Renderer()
{
	ReleasePtr(m_pBackBuffer);
}

bool GDI_Renderer::Initialise(HWND _hWnd, HINSTANCE _hInstance, int _clientWidth, int _clientHeight)
{
	// Assign member variables
	m_hWnd = _hWnd;
	m_hInstance = _hInstance;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Create and Initialise the Backbuffer
	m_pBackBuffer = new CBackBuffer();
	VALIDATE(m_pBackBuffer->Initialise(m_hWnd, m_clientWidth, m_clientHeight));

	return true;
}

void GDI_Renderer::RenderPolygon(v2float* _pPoints, COLORREF _colorFill, COLORREF _colorOutline, int _size)
{
	HDC hdc = m_pBackBuffer->GetBFDC();

	// Convert v2floats into POINTs for use with GDI
	POINT* pPoints = new POINT[_size];
	for (int i = 0; i < _size; i++)
	{
		pPoints[i] = { (LONG)_pPoints[i].x, (LONG)_pPoints[i].y };
	}

	// Create a solid brush object with the objects specified color
	HBRUSH brush = CreateSolidBrush(_colorFill);
	SelectObject(hdc, brush);

	// Draw the object with the solid brush object
	Polygon(hdc, pPoints, _size);
	DeleteObject(brush);

	HPEN hPenOld;
	HPEN hLinePen;

	// Create a pen using the objects specified outline color
	hLinePen = CreatePen(PS_SOLID, 3, _colorOutline);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Draw a outline onto the object using the pen
	Polygon(hdc, pPoints, _size);

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);

	ReleasePtr(pPoints);
}

void GDI_Renderer::RenderEllipse(v2float _center, COLORREF _colorFill, COLORREF _colorOutline, float _radius)
{
	HDC hdc = m_pBackBuffer->GetBFDC();

	// Create a solid brush object with the objects specified color
	HBRUSH brush = CreateSolidBrush(_colorFill);
	SelectObject(hdc, brush);

	// Create top, bottom, left and right points to define an ellipse from the radius of the ellipse
	int ellipseLeft = (int)(_center.x - _radius);
	int ellipseTop = (int)(_center.y - _radius);
	int ellipseRight = (int)(_center.x + _radius);
	int ellipseBottom = (int)(_center.y + _radius);

	// Draw the object with the solid brush object
	Ellipse(hdc, ellipseLeft, ellipseTop, ellipseRight, ellipseBottom);
	DeleteObject(brush);

	HPEN hPenOld;
	HPEN hLinePen;

	// Create a pen using the objects specified outline color
	hLinePen = CreatePen(PS_SOLID, 2, _colorOutline);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Draw a outline onto the object using the pen
	Ellipse(hdc, ellipseLeft, ellipseTop, ellipseRight, ellipseBottom);
	
	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

void GDI_Renderer::RenderLine(v2float _posA, v2float _posB, COLORREF _color)
{
	HDC hdc = m_pBackBuffer->GetBFDC();
	HPEN hPenOld;
	HPEN hLinePen;

	// Create a pen using the objects specified outline color
	hLinePen = CreatePen(PS_SOLID, 4, _color);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	// Draw a outline onto the object using the pen
	MoveToEx(hdc, (int)_posA.x, (int)_posA.y, NULL);
	LineTo(hdc, (int)_posB.x, (int)_posB.y);
	
	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

void GDI_Renderer::BeginRender()
{
	m_pBackBuffer->Clear();
}

void GDI_Renderer::EndRender()
{
	m_pBackBuffer->Present();
}

void GDI_Renderer::WriteLine(std::string _line, int _startingWidth, int _startingHeight, COLORREF _color, bool _bkgrndTransparent)
{
	// Set the Background transparency for the Text
	if (_bkgrndTransparent == true)
	{
		SetBkMode(m_pBackBuffer->GetBFDC(), TRANSPARENT);
	}
	else
	{
		SetBkMode(m_pBackBuffer->GetBFDC(), OPAQUE);
	}

	// Set the Text Color
	SetTextColor(m_pBackBuffer->GetBFDC(), _color);

	// Write the Text
	TextOutA(m_pBackBuffer->GetBFDC(), _startingWidth, _startingHeight, _line.c_str(), _line.size());	
}