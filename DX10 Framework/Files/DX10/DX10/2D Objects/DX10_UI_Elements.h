/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : DX10_UI_Elements.h
* Description : Holds all the structures for the different UI Elements types for DirectX 10
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __DX10_UI_ELEMENTS_H__
#define __DX10_UI_ELEMENTS_H__

// Local Includes
#include "GUI_Button.h"

/***********************
* TSliderBar: Structure to hold details about a SliderBar
* @author: Callan Moore
********************/
struct TSliderBar
{
	DX10_Renderer* pRenderer;
	DXSprite* sprite_slider;
	DXSprite* sprite_bar;
	GUI_Button* buttonSlider;
	float minX;
	float maxX;
	bool selected;

	/***********************
	* TSliderBar: Default constructor for a SliderBar
	* @author: Callan Moore
	********************/
	TSliderBar()
	{
		// Initialise pointers to NULL
		pRenderer = 0;
		sprite_slider = 0;
		sprite_bar = 0;
		buttonSlider = 0;

		minX = 0;
		maxX = 0;
		selected = false;
	}

	/***********************
	* ~TSliderBar: Default destructor for a SliderBar
	* @author: Callan Moore
	********************/
	~TSliderBar()
	{
		// Release allocated memory
		ReleasePtr(sprite_slider);
		ReleasePtr(sprite_bar);
		ReleasePtr(buttonSlider);
	}

	/***********************
	* Initialise: Initialise the slider bar for use
	* @author: Callan Moore
	* @parameter: DX10_Renderer * _pRenderer: The renderer for this application
	* @parameter: DX10_Shader_Sprite * _pShader: The shader to draw the slider bar
	* @parameter: v2float _centrePos: The center position for the slider bar
	* @parameter: int _width: The overall width of the slider bar
	* @parameter: int _height: The overall height of the slider bar
	* @return: bool: Successful or not
	********************/
	bool Initialise(DX10_Renderer* _pRenderer, DX10_Shader_Sprite* _pShader, v2float _centrePos, int _width, int _height)
	{
		if (_pRenderer == 0 || _width < 0 || _height < 0)
		{
			// Pointer is NULL or sizes are invalid therefore initialization is failed
			return false;
		}

		// Assign Member variables
		pRenderer = _pRenderer;
		minX = _centrePos.x - (_width / 2.0f);
		maxX = minX + _width;

		float barMinY = _centrePos.y - (_height / 8.0f);
		float sliderMinY = _centrePos.y - (_height / 2.0f);

		// Create the sprites
		sprite_bar = new DXSprite();
		sprite_slider = new DXSprite();

		// Create the Bar 
		VALIDATE(sprite_bar->Initialise(_pRenderer, _pShader, "SliderBar.png", _width + 20, (_height / 4)));
		sprite_bar->SetPosition(minX, barMinY);

		// Extend the min and max so that the centre of the slider reaches the min and max positions of the bar
		minX -= (_width / 20.0f);
		maxX += (_width / 20.0f);

		// Create the Slider
		VALIDATE(sprite_slider->Initialise(_pRenderer, _pShader, "Dragon.png", (_width / 10), _height));
		sprite_slider->SetPosition(minX, sliderMinY);
		buttonSlider = new GUI_Button();
		VALIDATE(buttonSlider->Initialize(pRenderer, sprite_slider, minX, sliderMinY, (float)(_width / 10.0f), (float)_height));

		return true;
	}

	
	/***********************
	* Process: Process the slider bar
	* @author: Callan Moore
	* @parameter: v2float _mousePos: The current mouse position
	* @return: void
	********************/
	void Process(v2float _mousePos)
	{
		float newX = _mousePos.x;

		if (selected == true)
		{
			if (newX < minX)
			{
				newX = minX;
			}
			else if (newX > maxX)
			{
				newX = maxX;
			}

			buttonSlider->SetPosition({ newX, buttonSlider->GetPosition().y });
		}
	}

	/***********************
	* Render: Render the slider bar to the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void Render()
	{
		sprite_bar->Render();
		buttonSlider->Render();
	}

	/***********************
	* SelectSlider: Set the selected state of the slider
	* @author: Callan Moore
	* @parameter: v2float _mousePos: The current mouse position
	* @parameter: bool _selected: The state to set if applicable
	* @return: void
	********************/
	void SelectSlider(v2float _mousePos, bool _selected)
	{
		selected = false;

		if (_selected == true)
		{
			if (buttonSlider->IsInBounds(_mousePos) == true)
			{
				selected = true;
			}
		}
	}

	/***********************
	* GetRatio: Retrieve the current ratio of the slider bar
	* @author: Callan Moore
	* @return: float: The ratio of how far along the slider is along the bar
	********************/
	float GetRatio()
	{
		 float numerator = buttonSlider->GetPosition().x - minX;
		 float denominator = maxX - minX;

		 return numerator / denominator;
	}

	/***********************
	* GetSelected: Retrieve the current selected state
	* @author: Callan Moore
	* @return: bool: Selected or not
	********************/
	bool GetSelected()
	{
		return selected;
	}
	
	/***********************
	* SetRatio: Set the ratio of the slider and update the slider position
	* @author: Callan Moore
	* @parameter: float _ratio: The new ratio to set
	* @return: void
	********************/
	void SetRatio(float _ratio)
	{
		_ratio = max(0, _ratio);
		_ratio = min(_ratio, 1);

		float newX = _ratio * (maxX - minX) + minX;
		float newY = buttonSlider->GetPosition().y;

		buttonSlider->SetPosition({ newX, newY });
	}
};

#endif	// __DX10_UI_ELEMENTS_H__