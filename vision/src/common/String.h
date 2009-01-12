/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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

#ifndef _String_H_
#define _String_H_

#include <string>


namespace Common
{

/**
 * @internal
 * Class to make easy work with strings. It is based on std::string
 */
class String: public std::string
{
public:

	// Get string data
	char		charAt		( int index );
	int			indexOf		( const std::string& str );
	int			indexOf		( const std::string& str, int fromIndex );
	int			length		();
	String	substring ( int beginIndex );
	String	substring ( int beginIndex, int endIndex );

	// Compare
	bool		equals	( const std::string& str );

	// Modify string
	void toLowerCase	();
	void toUpperCase	();
};

} // namespace Common

#endif // _String_H_