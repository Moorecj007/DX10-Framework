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
	m_hWnd = _hWnd;
	m_hInstance = _hInstance;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Create and Initialise the Backbuffer
	m_pBackBuffer = new CBackBuffer();
	m_pBackBuffer->Initialise(m_hWnd, m_clientWidth, m_clientHeight);

	return true;
}

void GDI_Renderer::RenderPolygon(v2float* _pPoints, COLORREF _color, int _size)
{
	HDC hdc = m_pBackBuffer->GetBFDC();

	POINT* pPoints = new POINT[_size];
	for (int i = 0; i < _size; i++)
	{
		pPoints[i] = { (LONG)_pPoints[i].x, (LONG)_pPoints[i].y };
	}

	HBRUSH brush = CreateSolidBrush(_color);
	SelectObject(hdc, brush);

	// Draw the Object with a solid fill color
	Polygon(hdc, pPoints, _size);
	DeleteObject(brush);

	HPEN hPenOld;
	HPEN hLinePen;

	// Draw a black outline onto the object
	hLinePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	Polygon(hdc, pPoints, _size);

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);

	ReleasePtr(pPoints);
}

void GDI_Renderer::RenderEllipse(v2float _center, COLORREF _color, float _radius)
{
	HDC hdc = m_pBackBuffer->GetBFDC();

	HBRUSH brush = CreateSolidBrush(_color);
	SelectObject(hdc, brush);

	int ellipseLeft = (int)(_center.x - _radius);
	int ellipseTop = (int)(_center.y - _radius);
	int ellipseRight = (int)(_center.x + _radius);
	int ellipseBottom = (int)(_center.y + _radius);

	// Draw the Object with a solid fill color
	Ellipse(hdc, ellipseLeft, ellipseTop, ellipseRight, ellipseBottom);
	DeleteObject(brush);

	HPEN hPenOld;
	HPEN hLinePen;

	// Draw a black outline onto the object
	hLinePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	Ellipse(hdc, ellipseLeft, ellipseTop, ellipseRight, ellipseBottom);
	
	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

void GDI_Renderer::RenderLine(v2float _posA, v2float _posB, COLORREF _color)
{
	HDC hdc = m_pBackBuffer->GetBFDC();
	HPEN hPenOld;
	HPEN hLinePen;

	// Draw a black outline onto the object
	hLinePen = CreatePen(PS_SOLID, 3, _color);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	LineTo(hdc, (int)_posA.x, (int)_posA.y);
	MoveToEx(hdc, (int)_posB.x, (int)_posB.y, NULL);

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