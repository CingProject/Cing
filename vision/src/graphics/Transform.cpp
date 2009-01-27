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

#include "Transform.h"
#include "common/MathUtils.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Transform::Transform() :	m_bIsValid( false )
{
	init();
};


/**
* @internal
* @brief Initializes the class so it becomes valid.
*
* @return true if the initialization was ok | false otherwise
*/
bool Transform::init()
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init values
	m_position	= Vector( 0, 0, 0 );
	m_rotation	= Vector( 0, 0, 0 );
	m_scale			= Vector( 1.0, 1.0, 1.0 );

	m_4x4 = Ogre::Matrix4::IDENTITY;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Transform::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// The class is not valid anymore
	m_bIsValid = false;
}

// Translate
void Transform::translate(  float x, float y, float z )
{
 	// Apply transform
	m_4x4 = m_4x4.concatenate( Ogre::Matrix4::getTrans(x,y,z) );
}

//Rotate
void Transform::rotate(  float x, float y, float z )
{
	// Make temporal transform
	Ogre::Matrix3 m3Temp;
	m3Temp.FromEulerAnglesXYZ( Ogre::Radian(x), Ogre::Radian(y), Ogre::Radian(z) );
	Ogre::Matrix4 tMat = Ogre::Matrix4( m3Temp );

	// Apply transform
	m_4x4 = m_4x4.concatenate(tMat);
}

//Scale
void Transform::scale(  float x, float y, float z )
{
	// Apply transform
	m_4x4 = m_4x4.concatenate(Ogre::Matrix4::getScale( x, y, z ));
}

} // namespace Graphics
