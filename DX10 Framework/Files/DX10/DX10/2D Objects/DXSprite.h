//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	DXBitmap.h
// Description	:	This wraps all the bitmap functions and variables
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

#pragma once
#ifndef __DX_SPRITE_H__
#define __DX_SPRITE_H__

// Library Includes

// Local Includes
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"
#include "../Shaders/FXTexture.h"

class DXSprite
{
	// Functions
	public:
		/*******************
		-> Default constructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		DXSprite();

		/*******************
		-> Default destructor.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	...
		********************/
		~DXSprite();

		/*******************
		-> Intializes the object
		@author:	Juran Griffith.
		@parameter: _pDX10_Renderer		- The renderer for this application
		@parameter: _pHWnd				- The handler to the window.
		@parameter: _filename			- The filename of the texture (include the filepath here too if needed).
		@parameter: _bitmapWidth		- The bitmap width.
		@parameter: _bitmapHeight		- The bitmap height.
		@parameter: _sliceWidth			- How many slices along the width of the image to do to split the image up into sections. (For sprite sheets etc...)
		@parameter: _sliceHeight		- How many slices along the height of the image to do to split the image up into sections. (For sprite sheets etc...)
		@return:	bool		- Returns true if the intialization succeeds otherwise the error on why it failed.
		********************/
		bool Initialize(DX10_Renderer* _pDX10_Renderer, FXTexture* _pShader, std::string _filename, float _bitmapWidth, float _bitmapHeight, int _sliceWidth = 1, int _sliceHeight = 1);

		/*******************
		-> Prepares the object for drawing.
		@author:	Juran Griffith.
		@parameter:	_positionX	- The x screen position to draw the object.
		@parameter:	_positionY	- The y screen position to draw the object.
		@return:	void.
		********************/
		void Process(float _positionX, float _positionY);

		/*******************
		-> Gets the number of indexes for the 2D image.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	int		- The number of indexes for the 2D image
		********************/
		int GetIndexCount();

		/*******************
		-> Gets the number of slices in the bitmap for rendering different parts of the bitmap.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	int		- The number of indexes for the 2D image
		********************/
		int GetSliceWidth();

		/*******************
		-> Gets the number of slices in the bitmap for rendering different parts of the bitmap.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	int		- The number of indexes for the 2D image
		********************/
		int GetSliceHeight();

		/*******************
		-> Gets the texture for this object so that it can be used for rendering by shaders.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	ID3D10ShaderResourceView*		- Returns a pointer to the texture resource.
		********************/
		ID3D10ShaderResourceView* GetTexture();

		/*******************
		-> Sets the uv to point to different section of the bitmap based on the amount of splicing done.
		@author:	Juran Griffith.
		@parameter: _index	- The image index to render.
		@return:	void.
		********************/
		void SetImageIndex(int _index);

		/*******************
		-> Sets the size of the bitmap
		@author:	Juran Griffith.
		@parameter:	_width	- The width of the bitmap.
		@parameter:	_height	- The height of the bitmap.
		@return:	void.
		********************/
		void SetSize(float _width, float _height);
	protected:
	private:
		/*******************
		-> This builds the vertex and index buffer that will be used to draw the 2D image
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	bool		- Returns true if the intialization succeeds otherwise the error on why it failed.
		********************/
		bool InitializeBuffers();

		/*******************
		-> Update the contents of the dynamic vertex buffer to re-position the 2D bitmap image on the screen if need be
		@author:	Juran Griffith.
		@parameter:	_positionX	- The x screen position to draw the object.
		@parameter:	_positionY	- The y screen position to draw the object.
		@return:	void.
		********************/
		bool UpdateBuffers(float _positionX, float _positionY);

		/*******************
		-> Prepare the vertex and index buffers on the gpu to be drawn by the shader.
		@author:	Juran Griffith.
		@parameter:	None.
		@return:	void.
		********************/
		void Render();

	// Variables
	public:
	protected:
	private:
		HWND*			m_pHWnd;
		UINT			m_vertexCount;
		UINT			m_indexCount;
		UINT			m_stride;
		UINT			m_offset;

		std::string		m_strFilename;

		float			m_previousPosX;
		float			m_previousPosY;

		int				m_screenWidth;
		int				m_screenHeight;
		float			m_bitmapWidth;
		float			m_bitmapHeight;

		// Sprite sheet variables
		std::vector<POINT> m_imageIndexList;
		int				m_index;
		int				m_indexPrev;
		int				m_sliceWidth;
		int				m_sliceHeight;
		float			m_offsetScreenWidth;
		float			m_offsetScreenHeight;
		float			m_offsetBitmapWidth;
		float			m_offsetBitmapHeight;
		float			m_offsetU;
		float			m_offsetV;

		DX10_Renderer*  m_pDX10_Renderer;
		UINT			m_texID;
		UINT			m_buffID;
		FXTexture*		m_pShader;
};

#endif