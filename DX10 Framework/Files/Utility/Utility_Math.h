/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Utility_Math.h
* Description : Extra utility functions and structures for mathematics
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

//Inclusion Guards
#pragma once
#ifndef __UTILITY_MATH_H__
#define __UTILITY_MATH_H__

// Defines
#define _USE_MATH_DEFINES

// Library Includes
#include <math.h>

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

	/***********************
	* v2float: Constructor for the v2float struct
	* @author: Callan Moore
	* @parameter: x: X coordinate
	* @parameter: y: Y coordinate
	********************/
	v2float(float _x = 0, float _y = 0)
	{
		x = _x;
		y = _y;
	}

	/***********************
	* operator=: Assignment Operator for the v2float struct
	* @author: Callan Moore
	* @parameter: _v2: v2float to be copied
	* @return: v2float: the result of the assignment
	********************/
	v2float& operator=(const v2float _v2)
	{
		x = _v2.x;
		y = _v2.y;
		return *this;
	}

	/***********************
	* operator==: Is equal to Operator for the v2float struct
	* @author: Callan Moore
	* @parameter: _v2: v2float to be checked against
	* @return: bool: True if the v2floats are equal
	********************/
	bool operator==(const v2float _v2)
	{
		if (x == _v2.x
			&&	y == _v2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/***********************
	* operator+: Addition Operator for the v2float struct plus a v2float
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the addition
	* @return: v2float: The result of the addition
	********************/
	v2float operator+ (const v2float _v2) const
	{
		return v2float{ _v2.x + x, _v2.y + y };
	}

	/***********************
	* operator+: Addition Operator for the v2float struct plus a float
	* @author: Callan Moore
	* @parameter: _f: Right hand side of the addition
	* @return: v2float: The result of the addition
	********************/
	v2float operator+ (const float _f) const
	{
		return v2float{ _f + x, _f + y };
	}

	/***********************
	* operator+=: Addition Assignment Operator for the v2float struct
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the addition
	* @return: v2float: The result of the addition assignment
	********************/
	v2float operator+= (const v2float _v2)
	{
		return v2float{ x += _v2.x, y += _v2.y };
	}

	/***********************
	* operator-: Subtraction Operator for the v2float struct minus a v2float
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the subtraction
	* @return: v2float: The result of the subtraction
	********************/
	v2float operator- (const v2float _v2) const
	{
		return v2float{ x - _v2.x, y - _v2.y };
	}

	/***********************
	* operator-: Subtraction Operator for the v2float struct minus a float
	* @author: Callan Moore
	* @parameter: _f: Right hand side of the subtraction
	* @return: v2float: The result of the subtraction
	********************/
	v2float operator- (const float _f) const
	{
		return v2float{ x - _f, y - _f };
	}

	/***********************
	* operator-=: Subtraction Assignment Operator for the v2float struct minus a v2float
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the subtraction
	* @return: v2float: The result of the subtraction
	********************/
	v2float operator-= (const v2float _v2)
	{
		return v2float{ x -= _v2.x, y -= _v2.y };
	}

	/***********************
	* operator*: Multiplication Operator for a v2float times a v3float
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the multiplication
	* @return: v2float: The result of the multiplication
	********************/
	v2float operator* (const v2float _v2) const
	{
		return v2float{ _v2.x * x, _v2.y * y };
	}

	/***********************
	* operator*: Multiplication Operator for a v2float times a float
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the multiplication
	* @return: v2float: The result of the multiplication
	********************/
	v2float operator* (const float _f) const
	{
		return v2float{ _f * x, _f * y };
	}

	/***********************
	* operator/: Division Operator for a v2float times a float
	* @author: Callan Moore
	* @parameter: _v2: Right hand side of the Division
	* @return: v2float: The result of the Division
	********************/
	v2float operator/ (const float _f) const
	{
		return v2float{ x / _f, y / _f };
	}

	/***********************
	* Magnitude: Calculate the magnitude of this v2float
	* @author: Callan Moore
	* @return: float: The magnitude of the v2float
	********************/
	float Magnitude()
	{
		return (sqrt(pow(x, 2) + pow(y, 2)));
	}

	/***********************
	* NormaliseV2Float: Normalise a vector of 2 floats
	* @author: Callan Moore
	* @parameter: _v2: vector to normalize
	* @return: v2float: the normalized vector
	********************/
	v2float Normalise()
	{
		float fMagnitude = this->Magnitude();

		if (fMagnitude != 0)
		{
			this->x = (this->x / fMagnitude);
			this->y = (this->y / fMagnitude);
		}
		return (*this);
	}
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
	* operator-: Negate the following v3float
	* @author: Callan Moore
	* @return: v3float: The result of the negation
	********************/
	v3float operator- () const
	{
		return v3float{ -x, -y, -z };
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
	* operator*=: Multiplication Assignment Operator for a v3float times a v3float
	* @author: Jc Fowles
	* @parameter: _v3: Right hand side of the multiplication
	* @return: v3float: The result of the multiplication
	********************/
	v3float operator*= (const v3float _v3) 
	{
		return v3float{ x *= _v3.x, y *= _v3.y, z *= _v3.z };
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

	/***********************
	* ApproxEqual: Check if two vectors difference is within the approximation value therefore approximately equal
	* @author: Callan Moore
	* @parameter: _v3: second vector for comparison
	* @parameter: _apx: approximation value
	* @return: bool: True if the vectors are approximately equal
	********************/
	bool ApproxEqual(const v3float _v3, const float _apx)
	{
		if ((abs(_v3.x - x) <= _apx)
			&& (abs(_v3.y - y) <= _apx)
			&& (abs(_v3.z - z) <= _apx))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/***********************
	* Cross: Calculate the cross product of two vectors
	* @author: Callan Moore
	* @parameter: const v3float & _v3: The second vector
	* @return: v3float: The result of the cross product
	********************/
	v3float Cross(const v3float& _v3)
	{
		return v3float(y * _v3.z - z * _v3.y, z * _v3.x - x * _v3.z, x * _v3.y - y * _v3.x);
	}
	
	/***********************
	* Dot: Calculate the Dot product of two vectors
	* @author: Callan Moore
	* @parameter: const v3float & _v3: The second vector
	* @return: float: The result of the Dot product
	********************/
	float Dot(const v3float& _v3)
	{
		return ((x * _v3.x) + (y * _v3.y) + (z * _v3.z));
	}
};

/***********************
* OnLineSegment: Checks if a point is on the given line segment
* @author: Callan Moore
* @parameter: _linePtA: First point of the line
* @parameter: _linePtB: Second point of the line
* @parameter: _point: Point to check if it lies on the line segment
* @return: bool: True if the point is on the line, false otherwise
********************/
inline bool OnLineSegment(v2float _linePtA, v2float _linePtB, v2float _point)
{
	if (_linePtB.x <= max(_linePtA.x, _point.x) && _linePtB.x >= min(_linePtA.x, _point.x) &&
		_linePtB.y <= max(_linePtA.y, _point.y) && _linePtB.y >= min(_linePtA.y, _point.y))
	{
		return true;
	}

	return false;
}

/***********************
* Orientation: Determines the orientation of a line and a point
* @author: Callan Moore
* @parameter: _linePtA: First point on the line
* @parameter: _linePtB: second point on the line
* @parameter: _point: Point of the second line to cross reference
* @return: int: Value to represent the orientation of the 3 points
********************/
inline int Orientation(v2float _linePtA, v2float _linePtB, v2float _point)
{
	// 0 -> All points are co linear
	// 1 -> Clockwise orientation
	// 2 -> Counterclockwise orientation

	float val = (_linePtB.y - _linePtA.y) * (_point.x - _linePtB.x) - (_linePtB.x - _linePtA.x) * (_point.y - _linePtB.y);

	if (val == 0)
	{
		return 0;  // co linear
	}

	return (val > 0) ? 1 : 2;
}

/***********************
* CheckLinesIntersect: Checks whether two line segments intersect
* @author: Callan Moore
* @parameter: _line1PtA: First point on line 1
* @parameter: _line1PtB: Second point on line 1
* @parameter: _line2PtA: First point on line 2
* @parameter: _line2PtB: Second point on line 2
* @return: bool: True if the line segments intersect
********************/
inline bool CheckLinesIntersect(v2float _line1PtA, v2float _line1PtB, v2float _line2PtA, v2float _line2PtB)
{
	// Find the four orientations needed for general and special cases
	int orientation1 = Orientation(_line1PtA, _line1PtB, _line2PtA);
	int orientation2 = Orientation(_line1PtA, _line1PtB, _line2PtB);
	int orientation3 = Orientation(_line2PtA, _line2PtB, _line1PtA);
	int orientation4 = Orientation(_line2PtA, _line2PtB, _line1PtB);

	// General case
	if (orientation1 != orientation2 && orientation3 != orientation4)
	{
		return true;
	}

	// Special Cases
	// _line1PtA, _line1PtB and _line2PtA are co linear and _line2PtA lies on segment Line1
	if (orientation1 == 0 && OnLineSegment(_line1PtA, _line2PtA, _line1PtB))
	{
		return true;
	}

	// _line1PtA, _line1PtB and _line2PtA are co linear and _line2PtB lies on segment Line1
	if (orientation2 == 0 && OnLineSegment(_line1PtA, _line2PtB, _line1PtB))
	{
		return true;
	}

	// _line2PtA, _line2PtB and _line1PtA are co linear and _line1PtA lies on segment Line2
	if (orientation3 == 0 && OnLineSegment(_line2PtA, _line1PtA, _line2PtB))
	{
		return true;
	}

	// _line2PtA, _line2PtB and _line1PtB are co linear and _line1PtB lies on segment Line2
	if (orientation4 == 0 && OnLineSegment(_line2PtA, _line1PtB, _line2PtB))
	{
		return true;
	}

	return false; // No situation does the line segments intersect
}

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

/***********************
* RaySphereIntersect: Checks whether a Ray (In Local Space to the Sphere) intersect the Sphere
* @author: Jc Fowles
* @parameter: _rayOrigin: The Origin point of the Ray in Local Space to the Sphere
* @parameter: _rayDirection: The Direction of the Ray in Local Space to the Sphere
* @parameter: _sphereRad: Radius of the Sphere
* @return: bool: True if the Ray Intersect the Sphere
********************/
inline bool RaySphereIntersect(v3float _rayOrigin, v3float _rayDirection, float _sphereRad)
{
	// Calculate the a, b, and c Coefficients of the Discriminant
	float a = _rayDirection.Dot(_rayDirection);
	float b = _rayDirection.Dot(_rayOrigin) * 2.0f;
	float c = _rayOrigin.Dot(_rayOrigin) - pow(_sphereRad, 2);

	// Calculate the Discriminant (b^2 - 4ac)
	float discriminant = pow(b, 2) - (4 * a * c);

	if (discriminant < 0.0f)
	{
		// The Discriminant is Negative the Ray missed the Sphere
		return false;
	}
	else
	{
		// The Discriminant is Positive the Ray intersected the Sphere
		return true;
	}
}


/***********************
* PointOutsideOfPlane: Checks if a point is outside of the plane (on the opposite side as the normal)
* @author: Callan Moore
* @parameter: v3float _point: The point to check
* @parameter: v3float _triPointA: The first point of the triangle
* @parameter: v3float _triPointB: The second point of the triangle
* @parameter: v3float _triPointC: The third point of the triangle
* @return: bool: true if the point is outside the plane
********************/
inline bool PointOutsideOfPlane(v3float _point, v3float _triPointA, v3float _triPointB, v3float _triPointC)
{
	v3float pointToTriA = _point - _triPointA;
	v3float triAToTriB = _triPointB - _triPointA;
	v3float triAToTriC = _triPointC - _triPointA;
	
	return (pointToTriA.Dot(triAToTriB.Cross(triAToTriC)) >= 0 ? true : false);
}

/***********************
* ClosestPointOnTriangle: Calculate the closest point on a triangle to another point
* @author: Callan Moore
* @parameter: v3float _point: The point to check
* @parameter: v3float _triPointA: The first point of the triangle
* @parameter: v3float _triPointB: The second point of the triangle
* @parameter: v3float _triPointC: The third point of the triangle
* @return: v3float: The closest point on the triangle
********************/
inline v3float ClosestPointOnTriangle(v3float _point, v3float _triPointA, v3float _triPointB, v3float _triPointC)
{
	
	v3float triLineAB = _triPointB - _triPointA;
	v3float triLineAC = _triPointC - _triPointA;
	v3float triAToPoint = _point - _triPointA;
	v3float triBToPoint = _point - _triPointB;
	v3float triCToPoint = _point - _triPointC;

	// Check if The point is in the vertex region outside A
	float ABdotAP = triLineAB.Dot(triAToPoint);
	float ACdotAP = triLineAC.Dot(triAToPoint);
	if (ABdotAP <= 0.0f && ACdotAP <= 0.0f)
	{
		// Point is outside triangle closest to triPoint A
		return _triPointA; 
	}

	// Check if The point is in the vertex region outside B
	float ABdotBP = triLineAB.Dot(triBToPoint);
	float ACdotBP = triLineAC.Dot(triBToPoint);
	if (ABdotBP >= 0.0f && ACdotBP <= ABdotBP)
	{
		// Point is outside triangle closest to triPoint B
		return _triPointB; 
	}

	// Check if The point is in the vertex region outside C
	float ABdotCP = triLineAB.Dot(triCToPoint);
	float ACdotCP = triLineAC.Dot(triCToPoint);
	if (ACdotCP >= 0.0f && ABdotCP <= ACdotCP)
	{
		// Point is outside triangle closest to triPoint C
		return _triPointC; 
	}

	// Check if the Point is in edge region of triangle points A and B, if so return projection of the Point onto triangle line AB
	float regionCheckAB = ABdotAP * ACdotBP - ABdotBP * ACdotAP;
	if (regionCheckAB <= 0.0f && ABdotAP >= 0.0f && ABdotBP <= 0.0f) 
	{
		float modifier = ABdotAP / (ABdotAP - ABdotBP);
		return _triPointA + triLineAB * modifier; 
	}

	// Check if the Point is in edge region of triangle points A and C, if so return projection of the Point onto triangle line AC
	float regionCheckAC = ABdotCP*ACdotAP - ABdotAP*ACdotCP;
	if (regionCheckAC <= 0.0f && ACdotAP >= 0.0f && ACdotCP <= 0.0f) 
	{
		float modifier = ACdotAP / (ACdotAP - ACdotCP);
		return _triPointA + triLineAC * modifier; 
	}

	// Check if the Point is in edge region of triangle points B and C, if so return projection of the Point onto triangle line BC
	float regionCheckBC = ABdotBP*ACdotCP - ABdotCP*ACdotBP;
	if (regionCheckBC <= 0.0f && (ACdotBP - ABdotBP) >= 0.0f && (ABdotCP - ACdotCP) >= 0.0f) 
	{
		float modifier = (ACdotBP - ABdotBP) / ((ACdotBP - ABdotBP) + (ABdotCP - ACdotCP));
		return _triPointB + (_triPointC - _triPointB) * modifier; 
	}

	// The Point is inside face region
	float denominator = regionCheckBC + regionCheckAC + regionCheckAB;
	float modifierAC = regionCheckAC / denominator;
	float modiferAB = regionCheckAB / denominator;

	// Calculate the closest point inside the triangle
	return _triPointA + triLineAB * modifierAC + triLineAC * modiferAB; 
}

#endif	// __UTILITY_MATH_H__