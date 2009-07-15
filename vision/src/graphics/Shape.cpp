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

#include "Shape.h"
#include "common/CommonTypes.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Shape::Shape(): m_bIsValid ( false )
{
}

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Shape::Shape(Style style, Graphics::GraphicsType renderOp ): m_bIsValid( false ), m_style( style ), m_renderOp( renderOp )
{
}
/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Shape::Shape( Graphics::GraphicsType renderOp ): m_bIsValid( false ), m_renderOp( renderOp )
{
}
/**
 * @internal
 * @brief Destructor. Class release.
 */
Shape::~Shape()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief 
 */
void Shape::addVertex(float x, float y, Ogre::ColourValue theColor)
{
	m_vertexData.push_back(Ogre::Vector3(x,y,0));
	m_vertexColors.push_back( theColor );
};

/**
 * @internal
 * @brief 
 */
void Shape::addVertex(float x, float y, float z, Ogre::ColourValue theColor)
{
	m_vertexData.push_back(Ogre::Vector3(x,y,z));
	m_vertexColors.push_back( theColor );
};

} // namespace Graphics