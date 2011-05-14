/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

/**
 * @brief Returns the absolute value of the received number
 * @param[in] number Number to get the absolute value
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
 * @return true if both floats are equal (approx) | false otherwise
 */
inline bool equal( float f1, float f2 )
{
    // Compare the abs difference between the float
    if( fabs( f1 - f2 ) < EPSILON )
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
 * @brief Constrains a value so it does not exceed a range
 *
 * @param value value to constrain
 * @param min   min value of the valid range
 * @param min   min value of the valid range
 *
 * @return the constrained value
 */
inline float constrain( float value, float min, float max )
{
	if ( value > max )	return max;
	if ( value < min )	return min;
	return value;
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
struct Average
{
	Average()
	{
		nValues = 3;
		values.reserve( nValues );
		index = 0;
	}
	/// @param nValues number of values to store
	Average( int _nValues )
	{
		nValues = _nValues;
		values.reserve( nValues );
		index = 0;
	}

	/// @brief ads a value to average
	void addValue( double value )
	{
		if ( values.size() < nValues )
			values.push_back( value );
		else
			values[index] = value;

		// calculate new index
		index = (++index) % values.capacity();
	}

	/// @brief returns the ave
	double getValue()
	{
		double sum = std::accumulate( values.begin(), values.end(), 0.0 );
		return sum / (double)values.size();
	}

	std::vector< double >	values;
	size_t								nValues;
	size_t								index;
};

} // namespace Cing

#endif // _MathUtils_H_
