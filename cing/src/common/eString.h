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

#ifndef _Cing_String_H_
#define _Cing_String_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include <string>
#include <iostream>
#include <sstream>

// Ogre Unicode support
//#undef OGRE_UNICODE_SUPPORT
//#define OGRE_UNICODE_SUPPORT 1

//#define INT32 int
//#define UINT32 unsigned short
//#include <iterator>
//#include <string>
#include "OgreUTFString.h"


namespace Cing
{
	// Forward declarations
	class String;

	/// String utils
	template< typename T>
	String		toString( T input );
	String		intToString(int inputNumber);
	int			stringToInt(const String& str);


	/**
	* @internal
	* Class to make easy work with strings. It is based on std::string
	*/
	class String: public std::string
	{
	public:
		String() {}
		String(const std::string& input) : std::string( input ) {} // for automatic cast when using operator =
		String(const char * input) : std::string( input ) {}

		// Get string data
		char				charAt		( int index );
		int					indexOf		( const std::string& str );
		int					indexOf		( const std::string& str, int fromIndex );
		int					length		() const { return (int)size(); }
		String				substring	( int beginIndex );
		String				substring	( int beginIndex, int endIndex );
		const char*			toChar		() const { return c_str(); }
		//Ogre::UTFString		toUTF		() const;


		// Compare
		bool equals ( const std::string& str );

		// Modify string
		void toLowerCases	();
		void toUpperCases	();
		void replaceSubStr	( const String& subStrToFind, const String& subStrToReplace );
	};


	// Template method definition
	template<typename T>
	String	toString( T input )
	{
		std::stringstream s;
		s << input;
		return s.str();
	}

} // namespace Cing

#endif // _String_H_
