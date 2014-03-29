/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef _Cing_MathUtils_H_
#define _Cing_MathUtils_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "CommonPrereqs.h"
#include "CommonTypes.h"

#include "PerlinNoise.h"
#include "framework/UserAppGlobals.h"
#include "common/LogManager.h"

#include "OgreBitwise.h"

#include <numeric>
# include <cmath>

// Undefine previous min/max definitions ( from other libs ) 
#ifdef max
#undef max
#endif // max
#ifdef min
#undef min
#endif // min

/**
 * @file
 * This file contains several mathematical util functions
 */

namespace Cing
{

// Constants
const float PI          = 3.14159265359f;
const float HALF_PI     = 1.57079632679f;
const float TWO_PI      = 6.28318530717f;

const float EPSILON     = 0.00001f;


/**
 * @brief Returns the next power of 2 of a given number
 * @param[in] number Number from which the next power of two will be returned
 * @return the next power of 2 of a given number
 */
inline int firstPO2From( int number )
{
	return Ogre::Bitwise::firstPO2From(number);
}

/*
 * @brief Returns the min of the two received values
 * @return the min of the two received values
 */
template < typename T >
T min( T value1, T value2 )
{
	return (value1 < value2)? value1: value2;
}

/*
 * @brief Returns the max of the two received values
 * @return the max of the two received values
 */
template < typename T >
T max( T value1, T value2 )
{
	return (value1 > value2)? value1: value2;
}

/*
 * @brief Returns the given value clamped to the range [minVal..maxVal]
 * @return the value clamped to a given range
 */
template < typename T >
T clamp( T value, T minVal, T maxVal )
{
	T result = (value >= minVal) ? value : minVal;
	result = (result <= maxVal) ? result : maxVal;

	return result;
}

/*
 * @brief Rounds a number into the closest integer
 * @return the rounded number
 */
template < typename T >
T round( T value )
{
	return floor( value + (T)0.5 );
}


/**
 * @brief Returns the absolute value of the received number
 * @param[in] value Number to get the absolute value
 * @return the absolute value of the received number
 */
template< typename T >
T abs ( T value ) {  return std::abs( value );  }

/**
 * @brief Converts degrees into radians
 * @param[in] deg Degrees to convert
 * @return Radians equivalent to the degrees received
 */
inline float degToRad ( float deg )       {  return static_cast< float >( (PI * deg) / 180.0f );  }

/**
 * @brief Converts degrees into radians
 * @param[in] deg Degrees to convert
 * @return Radians equivalent to the degrees received
 */
inline float radians ( float deg )       {  return static_cast< float >( (PI * deg) / 180.0f );  }

/**
 * @brief Converts radians into degrees
 * @param[in] rad Radians to convert
 * @return Degrees equivalent to the radians received
 */
inline float radToDeg ( float rad )       {  return static_cast< float >( (180.0f * rad) / PI );  }

/**
* @brief Converts radians into degrees
* @param[in] rad Radians to convert
* @return Degrees equivalent to the radians received
*/
inline float degrees ( float rad )       {  return static_cast< float >( (180.0f * rad) / PI );  }

/**
 * @internal
 * @brief Starts random number generator seed
 * @param[in] seed Seed to set
 */
inline void setRandomSeed( unsigned int seed )
{
    srand(seed);
}

/**
 * @internal
 * @brief Starts random number generator seed
 * @param[in] seed Seed to set
 */
inline void randomSeed( int seed )
{
    srand( (unsigned int) seed);
}


/**
 * @brief Returns a random number in a range min..max (int version)
 * @param[in] min Min number for the generated random value
 * @param[in] max Max number for the generated random value
 */
inline int random( int min, int max )
{
	// Check min is higher than max
	if ( min > max )
	{
		return random(max, min);
	}

    return min + ( rand() % ( max - min+1 ) );
}

/**
 * @brief Returns a random number in a range 0..max (int version)
 * @param[in] max Max number for the generated random value
 */
inline int random( int max )
{
    return random( 0, max );
}

/**
 * @brief Returns a random number in a range min..max (floating numbers version)
 * @param[in] min Min number for the generated random value
 * @param[in] max Max number for the generated random value
 */
template< typename T >
inline T random( T min, T max )
{
	// Check min is higher than max
	if ( min > max )
	{
		return random(max, min);
	}

    // Number 0..1
    T normalizedRandom = static_cast<T>( rand()) / RAND_MAX;

    // Return float in range min..max
    return  min  + ( normalizedRandom * ( max - min ) );
}


/**
 * @brief Returns a random number in a range 0..max (floating numbers version)
 * @param[in] max Max number for the generated random value
 */
template< typename T >
inline T random( T max )
{
    return random( (T)0, max );
}


/**
 * @brief Compares two float numbers with an error margin
 * @param[in] f1 first float to compare
 * @param[in] f2 second float to compare
 * @param[in] epsilon difference between the values to consider them equal
 * @return true if both floats are equal (approx) | false otherwise
 */
template< typename T >
inline bool equal( T f1, T f2, T epsilon = (T)EPSILON )
{
    // Compare the abs difference between the float
    if( abs( f1 - f2 ) < epsilon )
        return true;
    else
        return false;
}

/**
 * @brief Calculates the distance between two points in space
 * @param[in] pos1 first position
 * @param[in] pos2 second position
 * @return the distance between two points in space
 */
inline float dist( const Vector& pos1, const Vector& pos2 )
{
  return pos1.distance( pos2 );
}

/**
 * @brief Calculates the distance between two points in space
 * @param[in] pos1 first position
 * @param[in] pos2 second position
 * @return the distance between two points in space
 */
inline float dist( float x1, float y1, float x2, float y2 )
{
  return sqrt((x1-x2)*(x1-x2) + ((y1-y2)*(y1-y2)));
}

/**
 * @brief Calculates the distance between two points in space
 * @param[in] pos1 first position
 * @param[in] pos2 second position
 * @return the distance between two points in space
 */
inline float dist( float x1, float y1, float z1,  float x2, float y2, float z2 )
{
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

/**
* @brief Calculates the magnitude (or length) of a vector.
*/
inline float mag( float x1, float y1)
{
	return sqrt((x1*x1) + (y1*y1) );
}
/**
* @brief Calculates the magnitude (or length) of a vector.
*/
inline float mag( float x1, float y1, float z1)
{
	return sqrt((x1*x1) + (y1*y1) + (z1*z1));
}


/**
 * @brief Returns true if 2 rectangles intersect
 * @param r1 Rectangle 1 (will be tested agains r2)
 * @param r2 Rectangle 2 (will be tested agains r1)
 * @return true if there is an intersection 
 */
inline bool contains( const Rect& r1, const Rect& r2) 
{
	return !( r1.left > r2.right || r1.right < r2.left || r1.top > r2.bottom || r1.bottom < r2.top);
}

/**
 * @brief Calculates intersection between 2 lines (defined by begining and end points in 2D coordinates) and checks for parallel lines.
 * also checks that the intersection point is actually on the line segment p1-p2
 * Original code from: http://workshop.evolutionzone.com/2007/09/10/code-2d-line-intersection/
 * @param p1 Start coordinates of the first line 
 * @param p2 End coordinates of the first line 
 * @param p3 Start coordinates of the second line 
 * @param p4 Endcoordinates of the second line 
 * @param[out] intersection Coordinates of the intersection if there is one
 * @return true if there is an intersection
 */
inline bool findIntersection( const Point2& p1, const Point2& p2, const Point2& p3, const Point2& p4, Point& intersection ) 
{
	float xD1,yD1,xD2,yD2,xD3,yD3;
	float dot,deg,len1,len2;
	float segmentLen1,segmentLen2;
	float ua,div;

	// calculate differences
	xD1 = p2.x-p1.x;
	xD2 = p4.x-p3.x;
	yD1 = p2.y-p1.y;
	yD2 = p4.y-p3.y;
	xD3 = p1.x-p3.x;
	yD3 = p1.y-p3.y;  

	// calculate the lengths of the two lines
	len1 = sqrt(xD1*xD1+yD1*yD1);
	len2 = sqrt(xD2*xD2+yD2*yD2);

	// calculate angle between the two lines.
	dot = (xD1*xD2+yD1*yD2); // dot product
	deg = dot/(len1*len2);

	// if abs(angle)==1 then the lines are parallell,
	// so no intersection is possible
	if(abs(deg)==1) 
		return false;

	// find intersection Pt between two lines
	div=yD2*xD1-xD2*yD1;
	ua=(xD2*yD3-yD2*xD3)/div;
	//ub=(xD1*yD3-yD1*xD3)/div; // commented as never used.
	intersection.x = p1.x+ua*xD1;
	intersection.y = p1.y+ua*yD1;

	// calculate the combined length of the two segments
	// between Pt-p1 and Pt-p2
	xD1 = intersection.x-p1.x;
	xD2 = intersection.x-p2.x;
	yD1 = intersection.y-p1.y;
	yD2 = intersection.y-p2.y;
	segmentLen1 = sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

	// calculate the combined length of the two segments
	// between Pt-p3 and Pt-p4
	xD1 = intersection.x-p3.x;
	xD2 = intersection.x-p4.x;
	yD1 = intersection.y-p3.y;
	yD2 = intersection.y-p4.y;
	segmentLen2 = sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

	// if the lengths of both sets of segments are the same as
	// the lenghts of the two lines the point is actually
	// on the line segment.

	// if the point isn't on the line, return null
	if(abs(len1-segmentLen1)>0.01 || abs(len2-segmentLen2)>0.01)
		return false;

	// return the valid intersection
	return true;
}

/*
 * Returns true if a line and an ellipse intersect (and returns in the out parameters the intersection coordinates).
 * TODO: Test and review return parameters
 * Original source: http://www.codeguru.com/forum/showthread.php?p=435345
 * @param _radiusX radius in x axis of the ellipse
 * @param _radiusY radius in y axis of the ellipse
 * @param centerX X coordinate of the center of the ellipse
 * @param centerY Y coordinate of the center of the ellipse
 * @param x1 x coordinate of the start of the line
 * @param y1 y coordinate of the start of the line
 * @param x2 x coordinate of the end of the line
 * @param y2 y coordinate of the end of the line
 * @param[out] xi1 x coordinate of the first intersection point
 * @param[out] xi2 x coordinate of the second intersection point
 * @param[out] yi1 y coordinate of the first intersection point
 * @param[out] yi2 y coordinate of the second intersection point
 * @param[out] firstPairOnSegment true of the first point is an intersection
 * @param[out] secondPairOnSegment true of the second point is an intersection
 */
inline bool ellipseIntersectsLine(	float _radiusX, float _radiusY, float centerX, float centerY, float x1, float y1, float x2, float y2,
									float &xi1, float &xi2, float &yi1, float &yi2, bool &firstPairOnSegment, bool &secondPairOnSegment) 
{																		
	// NOTE: original author wrote ellipse equation wrong, swapping for correction
	// radiusX should be semimajor access, radiusY semiminor access
	float radiusX = _radiusY;
	float radiusY = _radiusX;

	float aa,bb,cc,m;
	//
	if ( x1 != x2)
	{
		m = (y2-y1)/(x2-x1);
		float c = y1 - m*x1;
		//
		aa = radiusY*radiusY + radiusX*radiusX*m*m;
		bb = 2*radiusX*radiusX*c*m - 2*radiusX*radiusX*centerY*m - 2*centerX*radiusY*radiusY;
		cc = radiusY*radiusY*centerX*centerX + radiusX*radiusX*c*c - 2*radiusX*radiusX*centerY*c + radiusX*radiusX*centerY*centerY - radiusX*radiusX*radiusY*radiusY;
	}
	else
	{
		//
		// vertical line case
		//
		aa = radiusX*radiusX;
		bb = -2.0f*centerY*radiusX*radiusX;
		cc = -radiusX*radiusX*radiusY*radiusY + radiusY*radiusY*(x1-centerX)*(x1-centerX);
	}

	float d = bb*bb-4*aa*cc;
	//
	// intersection points : (xi1,yi1) and (xi2,yi2)
	//
	if (d > 0.0)
	{
		if (x1 != x2)
		{
			xi1 = (-bb + sqrt(d)) / (2*aa);
			xi2 = (-bb - sqrt(d)) / (2*aa);
			yi1 = y1 + m * (xi1 - x1);
			yi2 = y1 + m * (xi2 - x1);
		}
		else
		{
			yi1 = (-bb + sqrt(d)) / (2*aa);
			yi2 = (-bb - sqrt(d)) / (2*aa);
			xi1 = x1;
			xi2 = x1;
		}
		// the above logic is to check if the infinite length line crosses the ellipse,
		// now check and make sure that x,y pairs are actually on our line segment
		float low_x, high_x, low_y, high_y = 0;
		if( x1 < x2 ) {
			low_x = x1;
			high_x = x2;
		}
		else {
			low_x = x2;
			high_x = x1;
		}
		if( y1 < y2 ) {
			low_y = y1;
			high_y = y2;
		}
		else {
			low_y = y2;
			high_y = y1;
		}

		if( xi1 < low_x || xi1 > high_x || yi1 < low_y || yi1 > high_y)
			firstPairOnSegment = false;
		else
			firstPairOnSegment = true;
		if( xi2 < low_x || xi2 > high_x || yi2 < low_y || yi2 > high_y)
			secondPairOnSegment = false;
		else
			secondPairOnSegment = true;

		// No intersection?
		if(!secondPairOnSegment && !firstPairOnSegment)
		{
			return false;
		}

	}
	else
	{
		return false; // no intersections
	}

	// There is an intersection
	return true;
}

/**
 * @brief Maps a value from one range to another.
 *
 * @param[in] value value to map
 * @param[in] low1    minimum value of input range
 * @param[in] hight1  max value of input range
 * @param[in] low2    minimum value of output range
 * @param[in] hight2  max value of output range
 * @code
 *  map(v,0,1,100,200);
 * @endcode
 */
inline float map( float value, float low1, float hight1, float low2, float hight2 )
{
    // Clamp value with limits of input range
    value = (value < low1) ?  low1 : value;
    value = (value > hight1) ?  hight1 : value;

    // Map to range 0..1
	float v = (value-low1) / std::abs(hight1-low1);

    // Map to output range
	return v * std::abs(hight2-low2) + low2;
}

/*
 * @brief Constrains a value so it does not exceed a range (boundary limits for the range can be in any order)
 *
 * @param value value to constrain
 * @param limit1   first boundary to constrain value (it could be higher or lower than limit2)
 * @param limit2   second boundary to constrain value (it could be higher or lower than limit1)
 *
 * @return the constrained value
 */
	template<typename T1, typename T2, typename T3>
	inline T1 constrain( T1 value, T2 limit1, T3 limit2 )
	{
		// Case where limit1 is lower than limit2
		if ( limit1 < limit2 )
		{
			if ( value > limit2 )	return (T1)limit2;
			if ( value < limit1 )	return (T1)limit1;
			return value;
		}
		// other case: limit2 is lower than limit1 (so they are in inverse order)
		else 
		{
			if ( value > limit1 )	return (T1)limit1;
			if ( value < limit2 )	return (T1)limit2;
			return value;
		}
	}


/*
 * @brief Returns true if a number is within a defined range, false otherwise
 *
 * @param value value to check
 * @param min   min value of the valid range
 * @param max   max value of the valid range
 *
 * @return the constrained value
 */
inline bool inRange( float value, float min, float max )
{
	if (( value <= max ) && ( value >= min ))
		return true;
	return false;
}


/**
 * @brief Returns the angle between two vectors. They must be normalized
 *
 * @param the angle between two vectors
 */
inline float angleBetweenVectors( const Vector& v1, const Vector& v2 )
{
	return acos( v1.dotProduct( v2 ) );
}

/**
 * @brief Returns the Perlin noise value at specified coordinates
 *				The resulting value will always be between 0.0 and 1.0
 *
 * @param  	float: x coordinate in noise space
 */
inline float noise( float x )
{
	return 1.0;
}

/**
 * @brief Returns the Perlin noise value at specified coordinates
 *				The resulting value will always be between 0.0 and 1.0
 *
 * @param  	float: x coordinate in noise space
 */
inline float noise( float x, float y )
{
	return abs(_noise.get(x,y));
}

/**
 * @brief Returns the Perlin noise value at specified coordinates
 *				The resulting value will always be between 0.0 and 1.0
 *
 * @param  	float: x coordinate in noise space
 */
inline float noise( float x, float y, float z )
{
	return 1.0;
}

/**
 * @brief Calculates a number between two numbers at a specific increment
 *				The resulting value will always be between 0.0 and 1.0
 *
 * @param  	float: value1
 * @param  	float: value2
 * @param  	float: amt   from 0 to 1
 */
inline float lerp( float value1, float value2, float amt )
{
	return  value1 + amt * (value2 - value1);
}

/*
 * @brief Stores values and returns the average of all of them
 */
template <typename T>
struct Average
{
	Average()
	{
		cacheValid = false;
		nValues = 3;
		values.reserve( nValues );
		index = 0;
	}
	/// @param nValues number of values to store
	Average( int _nValues )
	{
		cacheValid = false;
		nValues = _nValues;
		values.reserve( nValues );
		index = 0;
	}
	~Average()
	{
		cacheValid = false;
		values.clear();
	}
	/// @param Sets the number of values that this Average will store.
	void setNumberValues( int _nValues )
	{
		cacheValid = false;
		nValues = _nValues;
		values.clear();
		values.reserve( nValues );
		index = 0;
	}

	// Clears current values
	void flush()
	{
		cacheValid = false;
		values.clear();
		values.reserve( nValues );
		index = 0;
	}


	/// @brief ads a value to average
	void addValue( const T& value )
	{
		if ( values.size() < nValues )
			values.push_back( value );
		else
			values[index] = value;

		// calculate new index
		index = (++index) % nValues;

		// Cache not valid anymore (until getValue is called again)
		cacheValid = false;
	}

	/// @brief returns the average value
	T getValue()
	{
		// No values still
		if ( values.size() == 0 ) 
			return T();

		// Check if we have a valid cache and return it if so
		if ( cacheValid )
			return cache;

		// No chache:  Calculate current average (and store in cache)
		cache		= std::accumulate( values.begin(), values.end(), T(0.0) ) / (float)values.size();
		cacheValid	= true;
		return cache;
	}

	std::vector< T >		values;
	size_t					nValues;
	size_t					index;
	T						cache;
	bool					cacheValid;
};

// Helpers for most common Average types
typedef Average<double> AverageDouble;
typedef Average<float>	AverageFloat;
typedef Average<int>	AverageInt;

} // namespace Cing

#endif // _MathUtils_H_
