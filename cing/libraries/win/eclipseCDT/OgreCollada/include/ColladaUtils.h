/*
-----------------------------------------------------------------------------
This source file is part of OgreCollada (http://ogrecollada.sf.net)

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

-----------------------------------------------------------------------------
*/

/**
 * \file ColladaUtils.h
 * \author Van Aarde Krynauw
 * \date 2007-09-23
 * \brief Miscellaneous utility functions.
 */

#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "ColladaTypes.h"

//Debug Logging
#ifdef DEBUG
	#define LOG_DEBUG(msg) { Ogre::LogManager::getSingleton().logMessage( Ogre::String((msg)) ); }
#else
	#define LOG_DEBUG(msg)
#endif

//Normal logging
#define LOG(msg) { Ogre::LogManager::getSingleton().logMessage( (msg) ); }

namespace OgreCollada
{

	class Utils
	{
		public:
			/**
			* Converts an integer value to a string
			* \param number The number to convert to a string
			* \return The string representation of the integer value
			*/
			static std::string int2str(const int& number);
			
			/**
			* Converts a floating point value to a string
			* \param number The number to convert to a string
			* \return The string representation of the floating point value
			*/
			static std::string float2str(float number);
			
			/** 
			* Convert a string to integer
			* \param str The string containing an integer
			* \return The integer extracted from the string
			*/
			static int str2int(const std::string str);
			
			/**
			* Extract the n-th integer from a given string
			* \param str String containing the integer
			* \param n The n-th integer to extract
			* \return The extracted integer
			*/
			static int getInt(const std::string str, uint n);
	};
	
}


#endif
