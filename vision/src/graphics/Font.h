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

#ifndef _Font_H_
#define _Font_H_

#include "GraphicsPrereqs.h"

// STL
#include <string>

namespace Graphics
{

/**
 * @internal
 * Class to create and control the properties of a font to print text on screen
 */
class Font
{
public:

	// Constructor / Destructor
	 Font();
	~Font();

	// Init / Release / Update
	bool init     ( const std::string& name = DEFAULT_FONT_NAME, int size = DEFAULT_FONT_SIZE, int resolution = DEFAULT_FONT_RESOLUTION );
	void  end     ();
	void  update  ();

	// Query methods
	bool  isValid () const { return m_bIsValid; }

	// Const static attributes
	static const std::string DEFAULT_FONT_NAME;       ///< Default font name
	static const int         DEFAULT_FONT_SIZE;       ///< Default font size
	static const int         DEFAULT_FONT_RESOLUTION; ///< Default font resolution

private:

	// Attributes
	bool  m_bIsValid;	///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _Font_H_
