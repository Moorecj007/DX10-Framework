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
#pragma message("Included Utilities.h")

// Defines
#define _USE_MATH_DEFINES
#define ReleasePtr(x) { if(x){ delete x; x = 0;} }

#if defined(DEBUG) | defined(_DEBUG)
#define VALIDATE(x) {								\
	if (!x) {										\
		return (false);								\
	}												\
}
#else
	#define VALIDATE(x) if(!x) return false
#endif

// Library Includes
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>

/***********************
Constants 
********************/
const float PI = 3.14159265358979323f;

/***********************
* v2float: Struct to hold 2 float values
* @author: Callan Moore
********************/
struct v2float
{
	float x;
	float y;
};

/***********************
* v3float: Struct to hold 3 float values
* @author: Callan Moore
********************/
struct v3float
{
	float x;
	float y;
	float z;

	/***********************
	* v3float: Constructor for the v3float struct
	* @author: Callan Moore
	* @parameter: x: X coordinate
	* @parameter: y: Y coordinate
	* @parameter: z: Z coordinate
	********************/
	v3float(float _x = 0, float _y = 0, float _z = 0)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	/***********************
	* operator=: Assignment Operator for the v3float struct
	* @author: Callan Moore
	* @parameter: _v3: v3float to be copied
	* @return: v3float: the result of the assignment
	********************/
	v3float& operator=(const v3float _v3)
	{
		x = _v3.x;
		y = _v3.y;
		z = _v3.z;
		return *this;
	}

	/***********************
	* operator==: Is equal to Operator for the v3float struct
	* @author: Callan Moore
	* @parameter: _v3: v3float to be checked against
	* @return: bool: True if the v3floats are equal
	********************/
	bool operator==(const v3float _v3)
	{
		if (x == _v3.x
			&&	y == _v3.y
			&&	z == _v3.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/***********************
	* operator+: Addition Operator for the v3float struct plus a v3float
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the addition
	* @return: v3float: The result of the addition
	********************/
	v3float operator+ (const v3float _v3) const
	{
		return v3float{ _v3.x + x, _v3.y + y, _v3.z + z };
	}

	/***********************
	* operator+: Addition Operator for the v3float struct plus a float
	* @author: Callan Moore
	* @parameter: _f: Right hand side of the addition
	* @return: v3float: The result of the addition
	********************/
	v3float operator+ (const float _f) const
	{
		return v3float{ _f + x, _f + y, _f + z };
	}

	/***********************
	* operator+=: Addition Assignment Operator for the v3float struct
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the addition
	* @return: v3float: The result of the addition assignment
	********************/
	v3float operator+= (const v3float _v3)
	{
		return v3float{ x += _v3.x, y += _v3.y, z += _v3.z };
	}

	/***********************
	* operator-: Subtraction Operator for the v3float struct minus a v3float
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the subtraction
	* @return: v3float: The result of the subtraction
	********************/
	v3float operator- (const v3float _v3) const
	{
		return v3float{ x - _v3.x, y - _v3.y, z - _v3.z };
	}

	/***********************
	* operator-: Subtraction Operator for the v3float struct minus a float
	* @author: Callan Moore
	* @parameter: _f: Right hand side of the subtraction
	* @return: v3float: The result of the subtraction
	********************/
	v3float operator- (const float _f) const
	{
		return v3float{ x - _f, y - _f, z - _f };
	}

	/***********************
	* operator-=: Subtraction Assignment Operator for the v3float struct minus a v3float
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the subtraction
	* @return: v3float: The result of the subtraction
	********************/
	v3float operator-= (const v3float _v3)
	{
		return v3float{ x -= _v3.x, y -= _v3.y, z -= _v3.z };
	}

	/***********************
	* operator*: Multiplication Operator for a v3float times a v3float
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the multiplication
	* @return: v3float: The result of the multiplication
	********************/
	v3float operator* (const v3float _v3) const
	{
		return v3float{ _v3.x * x, _v3.y * y, _v3.z * z };
	}

	/***********************
	* operator*: Multiplication Operator for a v3float times a float
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the multiplication
	* @return: v3float: The result of the multiplication
	********************/
	v3float operator* (const float _f) const
	{
		return v3float{ _f * x, _f * y, _f * z };
	}

	/***********************
	* operator/: Division Operator for a v3float times a float
	* @author: Callan Moore
	* @parameter: _v3: Right hand side of the Division
	* @return: v3float: The result of the Division
	********************/
	v3float operator/ (const float _f) const
	{
		return v3float{ x / _f, y / _f, z / _f };
	}

	/***********************
	* Magnitude: Calculate the magnitude of this v3float
	* @author: Callan Moore
	* @return: float: The magnitude of the v3float
	********************/
	float Magnitude()
	{
		return (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
	}

	/***********************
	* NormaliseV3Float: Normalise a vector of 3 floats
	* @author: Callan Moore
	* @parameter: _v3: vector to normalise
	* @return: v3float: the normalised vector
	********************/
	v3float Normalise()
	{
		float fMagnitude = this->Magnitude();

		if (fMagnitude != 0)
		{
			this->x = (this->x / fMagnitude);
			this->y = (this->y / fMagnitude);
			this->z = (this->z / fMagnitude);
		}
		return (*this);
	}

	/***********************
	* Limit: Limit the magnitude of the vector if it is greater than the input float
	* @author: Callan Moore
	* @parameter: _f: vector to normalise
	* @return: v3float: the normalised vector
	********************/
	v3float Limit(const float _f)
	{
		float fMagnitude = this->Magnitude();

		if (fMagnitude > _f)
		{
			(*this) = (this->Normalise()) * _f;
		}
		return (*this);
	}
};

/***********************
* DegreesToRadians: Converts degrees to radians
* @author: Callan Moore
* @parameter: _degrees: Angle in Degrees to convert
* @return: float: Angle in Radians
********************/
inline float DegreesToRadians(float _degrees)
{
	return ((_degrees * (float)M_PI) / 180.0f);
}

/***********************
* RadiansToDegrees: Converts radians to degrees
* @author: Callan Moore
* @parameter: _radians: Angle in radians to convert
* @return: float: Angle in degrees
********************/
inline float RadiansToDegrees(float _radians)
{
	return (180.0f * _radians / float(M_PI));
}

#endif // __UTILITIES_H__

