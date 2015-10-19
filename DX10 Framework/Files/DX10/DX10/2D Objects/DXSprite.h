//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	:	DXSprite.h
// Description	:	This wraps all the Sprite functions and variables
// Author		:	Juran Griffith
// Mail			:	juran.griffith@mediadesignschool.com
//

#pragma once
#ifndef __DX_SPRITE_H__
#define __DX_SPRITE_H__

// Library Includes
#include <fstream>      // std::ifstream

// Local Includes
#include "../DX10_Utilities.h"
#include "../DX10_Renderer.h"
#include "../Shaders/DX10_Shader_Sprite.h"

class DXSprite
{
	// Functions
	public:
		/***********************
		* DXSprite: Default constructor for the DXSprite class.
		* @author: Juran Griffith.
		********************/
		DXSprite();

		/***********************
		* Menu: Default destructor for the DXSprite class.
		* @author: Juran Griffith.
		********************/
		~DXSprite();

		/*******************
		* Initialise: Intialise the DXSprite class.
		* @author: Juran Griffith.
		* @parameter: _pDX10_Renderer: The renderer for this application
		* @parameter: _pShader: The shader object to use for rendering the sprite.
		* @parameter: _filename: The filename of the texture. (make sure the file is in the textures folder)
		* @parameter: _imageWidth: The image width.
		* @parameter: _imageHeight: The image height.
		* @parameter: _sliceWidth: How many slices along the width of the image to do to split the image up into sections. (For sprite sheets etc...).
		* @parameter: _sliceHeight: How many slices along the height of the image to do to split the image up into sections. (For sprite sheets etc...).
		* @return: bool: Returns true if the intialization succeeds otherwise the error on why it failed.
		********************/
		bool Initialise(DX10_Renderer* _pDX10_Renderer, DX10_Shader_Sprite* _pShader, std::string _filename, UINT _imageWidth, UINT _imageHeight, UINT _sliceWidth = 1, UINT _sliceHeight = 1);

		/*******************
		* SetPosition: Prepares the object for drawing.
		* @author:	Juran Griffith.
		* @parameter: _positionX: The x screen position to draw the object.
		* @parameter: _positionY: The y screen position to draw the object.
		* @return:	void.
		********************/
		void SetPosition(float _positionX, float _positionY);

		/*******************
		* GetIndexCount: Gets the number of indexes for the 2D image.
		* @author:	Juran Griffith.
		* @return:	int: The number of indexes for the 2D image.
		********************/
		int GetIndexCount();

		/*******************
		* GetSliceWidth: Gets the number of slices in the image for rendering different parts of the image.
		* @author: Juran Griffith.
		* @parameter: None.
		* @return: int: The number of indexes for the 2D image.
		********************/
		int GetSliceWidth();

		/*******************
		* GetSliceHeight: Gets the number of slices in the image for rendering different parts of the image.
		* @author: Juran Griffith.
		* @parameter: None.
		* @return: int: The number of indices for the 2D image.
		********************/
		int GetSliceHeight();

		/*******************
		* GetWidth: Gets the width of the sprite object.
		* @author: Juran Griffith.
		* @return:	float: The width of the sprite object.
		********************/
		float GetWidth();

		/*******************
		* GetHeight: Gets the height of the sprite object.
		* @author: Juran Griffith.
		* @return: UINT: The height of the sprite object.
		********************/
		float GetHeight();

		/*******************
		* GetImageWidth: Gets the true width of the actual image.
		* @author: Juran Griffith.
		* @return:	float: The width of the actual image.
		********************/
		float GetImageWidth();

		/*******************
		* GetImageHeight: Gets the true height of the actual image.
		* @author: Juran Griffith.
		* @return:	float: The height of the actual image.
		********************/
		float GetImageHeight();

		/*******************
		* GetPosition: Gets the position of the sprite.
		* @author: Juran Griffith.
		* @return: v2float: The 2D position of the sprite on the screen.
		********************/
		v2float GetPosition();

		/*******************
		* GetTexture: Gets the texture for this object so that it can be used for rendering by shaders.
		* @author: Juran Griffith.
		* @return: ID3D10ShaderResourceView*: Returns a pointer to the texture resource.
		********************/
		ID3D10ShaderResourceView* GetTexture();

		/*******************
		* SetImageIndex: Changes to a different section of the image. (based on the amount of splicing done.)
		* @author: Juran Griffith.
		* @parameter: _index: The image index to render.
		* @return: void.
		********************/
		void SetImageIndex(UINT _index);

		/*******************
		* SetSize: Sets the size of the image.
		* @author: Juran Griffith.
		* @parameter: _width: The width of the image.
		* @parameter: _height: The height of the image.
		* @return: void.
		********************/
		void SetSize(float _width, float _height);

		/*******************
		* SetLooped: Sets the loop flag for animation purposes.
		* @author: Juran Griffith.
		* @parameter: _looped: Set this to true to allow animation looping, otherwise it will play through once.
		* @return: void.
		********************/
		void SetLooped(bool _looped);

		/*******************
		* IsAtLastFrame: Checks to see if we have reached the last frame.
		* @author: Juran Griffith.
		* @return: bool: True if we are at the last frame.
		********************/
		bool IsAtLastFrame();

		/*******************
		* Render: Prepare the vertex and index buffers on the gpu to be drawn by the shader.
		* @author: Juran Griffith.
		* @return: void.
		********************/
		void Render();

		/*******************
		* IncrementIndex: Increments to the next image.
		* @author: Juran Griffith.
		* @parameter: _index: The image index to render.
		* @return: void.
		********************/
		void IncrementIndex();
	protected:
	private:
		/*******************
		* InitialiseBuffers: This builds the vertex and index buffer that will be used to draw the 2D image.
		* @author: Juran Griffith.
		* @return: bool: Returns true if the intialization succeeds otherwise the error on why it failed.
		********************/
		bool InitialiseBuffers();

		/*******************
		* UpdateBuffers: Update the contents of the dynamic vertex buffer to re-position the 2D image image on the screen if need be.
		* @author: Juran Griffith.
		* @return: void.
		********************/
		bool UpdateBuffers();

		/******************* (doesn't work) To Do - Juran
		* GetPngSize: Gets the PNG width and height of the image and updates this sprites width and height variables.
		* @author: Juran Griffith.
		* @return: bool: True if the file can be read and the variables are updated.
		********************/
		bool GetPngSize();

	// Variables
	public:
	protected:
	private:
		UINT					m_vertexCount;
		UINT					m_indexCount;

		std::string				m_strFilename;

		v2float					m_position;
		v2float					m_previousPosition;

		//int						m_screenWidth;
		//int						m_screenHeight;
		float					m_offsetScreenWidthPrev;
		float					m_offsetScreenHeightPrev;
		UINT					m_imageWidth;
		UINT					m_imageHeight;

		UINT					m_realImageWidth;
		UINT					m_realImageHeight;

		// Sprite sheet variables
		std::vector<POINT>		m_imageIndexList;
		UINT					m_index;
		UINT					m_indexPrev;
		int						m_sliceWidth;
		int						m_sliceHeight;
		float					m_offsetScreenWidth;
		float					m_offsetScreenHeight;
		float					m_offsetImageWidth;
		float					m_offsetImageHeight;
		float					m_offsetU;
		float					m_offsetV;

		DX10_Renderer*				m_pDX10_Renderer;
		ID3D10ShaderResourceView*	m_pTex;
		DX10_Buffer*				m_pBuff;
		DX10_Shader_Sprite*			m_pShader_Sprite;

		bool					m_animationLooped;
};

#endif