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

#include "LightingUserAPI.h"
#include "GraphicsManager.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreSceneManager.h"

namespace Graphics
{

/**
 * @brief Sets the ambient light of the scene. The ambient light affects all objects in the scene from all directions
 *
 * @param gray Gray value for the ambient light, this is, red, green and blue are equal
 */
void ambientLight( float gray )
{
	GraphicsManager::getSingleton().getSceneManager().setAmbientLight( Color(gray , gray , gray).normalized() );
}

/**
 * @brief Sets the ambient light of the scene. The ambient light affects all objects in the scene from all directions
 *
 * @param red		Red value for the ambient light
 * @param green Green value for the ambient light
 * @param blue	Blue value for the ambient light
 */
void ambientLight( float red, float green, float blue )
{
	GraphicsManager::getSingleton().getSceneManager().setAmbientLight( Color(red , green , blue).normalized() );
}

} // namespace Graphics