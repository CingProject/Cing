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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "Style.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Style::Style(): m_bIsValid    ( false )
{
}

// Todo: Arreglar el tema de la incialización de estilos, uniformizar
Style::Style( Color fillColor, Color strokeColor, int strokeWeight) :		m_fillColor   ( fillColor ),
																																				m_strokeColor ( strokeColor ),
																																				m_strokeWeight( strokeWeight ),
																																				m_bIsValid    ( true )
{
};

Style::Style( bool isStroke, bool isFill, Color fillColor, Color strokeColor, int strokeWeight ) :	m_stroke			( isStroke ),
																																																		m_fill				( isFill ),
																																																		m_fillColor   ( fillColor ),
																																																		m_strokeColor ( strokeColor ),
																																																		m_strokeWeight( strokeWeight ),
																																																		m_bIsValid    ( true )

{

};

/**
 * @internal
 * @brief Destructor. Class release.
 */
Style::~Style()
{
  // Release resources
  end();
}

} // namespace Cing