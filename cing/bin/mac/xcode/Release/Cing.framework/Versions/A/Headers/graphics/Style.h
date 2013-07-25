/*
  This source file is part of the Cing project
  For the latest info, see http://www.XXX.org

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

#ifndef _Style_H_
#define _Style_H_

#include "GraphicsPrereqs.h"
#include "Graphics/Color.h"

namespace Cing
{

/**
 * @internal
 * 
 */
class Style
{
public:

	// Constructor / Destructor
	 Style();
	 Style( Color fillColor, Color strokeColor, int strokeWeight );
	 Style( bool isStroke, bool isFill, Color fillColor, Color strokeColor, int strokeWeight );
	~Style();

	void init(){};
	void end(){};

	// Query methods
	bool                isValid         () const  { return m_bIsValid;     } 

	// Styling properties
	Color									m_fillColor;			  ///< Color used to fill shapes
	Color									m_strokeColor;			///< Color used to draw shapes
	int										m_strokeWeight;			///< Width of the stroke used for draw lines, points, and the border around shapes
	bool									m_fill;							///< Indicates if the shape is filled or not
	bool									m_stroke;						///< Indicates if the border of the shape is draw or not
private:
	bool                  m_bIsValid;					///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
};

} // namespace Cing

#endif // _Style_H_
