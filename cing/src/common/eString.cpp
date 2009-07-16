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

#include "eString.h"


namespace Cing
{

/**
 * @brief Returns the character located at a specific position within the string
 * @param index Index of the caracter to retrieve
 */
char String::charAt( int index )
{
	return this->at( index );
}

/**
 * @brief
 */
int String::indexOf( const std::string& str )
{
	return (int)this->find_first_of( str );
}

/**
 * @brief
 */
int String::indexOf( const std::string& str , int fromIndex )
{
	return (int)this->rfind( str, fromIndex );
}


	//// Compare

bool String::equals( const std::string& str )
{
	return *this == str;
}

	//// Modify string

/**
 * @brief
 */
void String::toUpperCases()
{
	std::string r = *this;
	for(unsigned int i = 0;i < size(); i++)
		r[i] = ::toupper( at( i ));
	*this = r.c_str();
}

/**
 * @brief
 */
void String::toLowerCases()
{
	std::string r = *this;
	for(unsigned int i = 0;i < size(); i++)
		r[i] = ::tolower( at( i ));
	*this = r.c_str();
}

/**
 * @brief
 */
String String::substring( int beginIndex )
{
	String result = this->substr( beginIndex).c_str();
	return result;
}

/**
 * @brief
 */
String String::substring( int beginIndex, int endIndex ) {
	String result = this->substr( beginIndex, endIndex).c_str();
	return result;
}

/*Ogre::UTFString String::toUTF() const
{
   Ogre::UTFString UTFString;
   int i;
   Ogre::UTFString::code_point cp;
   for (i=0; i<(int)this->size(); ++i)
   {
      cp = (*this)[i];
      cp &= 0xFF;
      UTFString.append(1, cp);
   }
   return UTFString;
}*/

}

