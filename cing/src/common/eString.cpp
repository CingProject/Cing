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

// Precompiled headers
#include "Cing-Precompiled.h"


#include "eString.h"


namespace Cing
{

/// String utils

/// Converts an int to string
String intToString(int inputNumber)
{
	std::stringstream s;
	s << inputNumber;
	return s.str();
}

/// Converts a string to int
int stringToInt(const std::string& str)
{
	int number;
	std::stringstream s(str);
	s >> number;
	return number;
}

float stringToFloat(const std::string& str)
{
	float number;
	std::stringstream s(str);
	s >> number;
	return number;
}

double stringToDouble(const std::string& str)
{
	double number;
	std::stringstream s(str);
	s >> number;
	return number;
}

unsigned long stringToUint32(const std::string& str) {
	unsigned long number;
	std::stringstream s(str);
	s >> number;
	return number;
}

/// Converts a wstring to int
int stringToInt(const std::wstring& str)
{
	int number;
	std::wstringstream s(str);
	s >> number;
	return number;
}

float stringToFloat(const std::wstring& str)
{
	float number;
	std::wstringstream s(str);
	s >> number;
	return number;
}

double stringToDouble(const std::wstring& str)
{
	double number;
	std::wstringstream s(str);
	s >> number;
	return number;
}

unsigned long stringToUint32(const std::wstring& str) {
	unsigned long number;
	std::wstringstream s(str);
	s >> number;
	return number;
}

/**
 * @brief Splits a string into a series of tokens (delimited by a delimiter character).
 * @param str			The string to split or tokenize
 * @param delim			The charactar delimiter
 * @param tokens[out]	This vector of strings stores the found individual tokens
 */
void split(const std::string& str, char delim, std::vector<std::string>& tokens )
{
    std::stringstream ss(str);
    std::string item;
    while ( std::getline( ss, item, delim ) )
    {
        tokens.push_back( item );
    }
}

/**
 * @brief Splits a string into a series of tokens (delimited by a delimiter character). This is a less optimal version than the previous
 * as it returns a copy vector with the found tokens
 * @param str		The string to split or tokenize
 * @param delim		The charactar delimiter
 * @return 			A vector containing the found individual tokens
 */
std::vector<std::string> split(const std::string& str, char delim )
{
	std::vector<std::string> tokens;
	split(str, delim, tokens);
	return tokens;
}

/* 
 * @brief Convert a string into a wstring for cases in whic you need to display accent or apostrophes and you need wider character range.
 * @paran string to convert
 * @return converted string to wide chars
 */ 
std::wstring toWString( const std::string& str )
{
  std::wstringstream wstrm;
  wstrm << str.c_str();
  return wstrm.str();

}

/*
 * @brief Converts a string into a Ogre UTF string. This is necessary if you need to handle non ascii characters (like accents: ó á and such).
 * @note Source: http://www.ogre3d.org/forums/viewtopic.php?t=32814&highlight=utfstring
 * @param str String to convert
 * @return the converted to UTF string
 */
Ogre::UTFString toUTF( const std::string& str)
{
   Ogre::UTFString UTFString;
   int i;
   Ogre::UTFString::code_point cp;
   for (i=0; i<(int)str.size(); ++i)
   {
      cp = str[i];
      cp &= 0xFF;
      UTFString.append(1, cp);
   }
	return UTFString;
}


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


void String::replaceSubStr( const String& subStrToFind, const String& subStrToReplace )
{
  std::string::size_type pos = 0;

  while((pos = find(subStrToFind, pos)) != std::string::npos)
  {
    replace(pos, subStrToFind.length(), subStrToReplace);
    pos += subStrToReplace.length();
  }
}


/*
 * @brief Converts a string into a Ogre UTF string. This is necessary if you need to handle non ascii characters (like accents: ó á and such).
 * @note Source: http://www.ogre3d.org/forums/viewtopic.php?t=32814&highlight=utfstring
 * @param str String to convert
 * @return the converted to UTF string
 */
Ogre::UTFString String::toUTF() const
{
	return Cing::toUTF(*this);
}

}

