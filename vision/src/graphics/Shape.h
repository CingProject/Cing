/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

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

#ifndef _Shape_H_
#define _Shape_H_

#include "GraphicsPrereqs.h"
#include "Color.h"
#include "Style.h"

namespace Graphics
{

/**
 * @internal
 * 
 */
class Shape
{
public:

	// Constructor / Destructor
	 Shape();
	 Shape( Graphics::GraphicsType renderOp );
	 Shape( Style style, Graphics::GraphicsType renderOp );

	~Shape();

	void init		(){};
	void end		(){};
	void clear	(){	m_vertexData.clear(); m_vertexColors.clear(); };

	// Query methods
	bool isValid         () const  { return m_bIsValid;					} 
	bool isStroke        () const  { return m_isStroke;					} 
	bool isFill					 () const  { return m_isFill;					  } 

	int	 getVertexCount  () const  { return (int)m_vertexData.size(); }

	void addVertex(float x, float y, Ogre::ColourValue theColor);
	void addVertex(float x, float y, float z, Ogre::ColourValue theColor);

	//
	Style														m_style;				///< Style to draw the shape
	Graphics::GraphicsType					m_renderOp;			///< POINTS, LINES, ...
	std::vector <Ogre::Vector3>			m_vertexData;
	std::vector <Ogre::ColourValue> m_vertexColors;

	Color														m_FillColor;
	Color														m_StrokeColor;

	bool														m_isFill;
	bool														m_isStroke;
	bool														m_isClose;

private:

	bool													  m_bIsValid;	    ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
};

} // namespace Graphics

#endif // _Shape_H_
