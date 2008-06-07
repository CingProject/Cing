/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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

#ifndef _MathUtils_H_
#define _MathUtils_H_

#include "CommonPrereqs.h"
#include "CommonTypes.h"

/**
 * @file
 * This file contains several mathematical util functions
 */

namespace Common
{

// Constants
const float PI          = 3.14159265359f;
const float EPSILON     = 0.00001f;


/**
 * @brief Returns the next power of 2 of a given number
 * @param[in] number Number from which the next power of two will be returned
 * @return the next power of 2 of a given number
 */
inline int nextPowerOf2( int number )
{
	int rval = 1;
	while( rval < number )
        rval <<= 1;
	return rval;
}

/**
 * @brief Returns the absolute value of the received number
 * @param[in] number Number to get the absolute value
 * @return the absolute value of the received number
 */
template< typename T > T abs ( T value ) {  return static_cast< T >( fabs( value ) );  }


/**
 * @brief Converts degrees into radians
 * @param[in] deg Degrees to convert
 * @return Radians equivalent to the degrees received
 */
inline float degToRad ( float deg )       {  return static_cast< float >( (PI * deg) / 180.0f );  }

/**
 * @brief Converts radians into degrees
 * @param[in] rad Radians to convert
 * @return Degrees equivalent to the radians received
 */
inline float radToDeg ( float rad )       {  return static_cast< float >( (180.0f * rad) / PI );  }

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
 * @brief Returns a random number in a range min..max (int version)
 * @param[in] min Min number for the generated random value
 * @param[in] max Max number for the generated random value
 */
inline int random( int min, int max )
{
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
 * @brief Returns a random number in a range min..max (float version)
 * @param[in] min Min number for the generated random value
 * @param[in] max Max number for the generated random value
 */
inline float random( float min, float max )
{
    // Number 0..1
    float normalizedRandom = (float)rand() / RAND_MAX;

    // Return float in range min..max
    return  min  + ( normalizedRandom * ( max - min ) );
}


/**
 * @brief Returns a random number in a range 0..max (float version)
 * @param[in] max Max number for the generated random value
 */
inline float random( float max )
{
    return random( 0.0f, max );
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
    float v = (value-low1) / fabs(hight1-low1); 

    // Map to output range
    return v * fabs(hight2-low2) + low2;
}


} // namespace Common

#endif // _MathUtils_H_
