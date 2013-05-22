/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

// Precompiled headers
#include "Cing-Precompiled.h"

#include "LightingUserAPI.h"
#include "GraphicsManager.h"

// Ogre
#include "OgreSceneManager.h"

// Framework
#include "framework/Application.h"

namespace Cing
{

/**
 * @brief Sets the ambient light of the scene. The ambient light affects all objects in the scene from all directions
 *
 * @param gray Gray value for the ambient light, this is, red, green and blue are equal
 */
void ambientLight( float gray )
{
  // Check application correctly initialized (could not be if the user didn't calle size function)
  Application::getSingleton().checkSubsystemsInit();

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
  // Check application correctly initialized (could not be if the user didn't calle size function)
  Application::getSingleton().checkSubsystemsInit();

	GraphicsManager::getSingleton().getSceneManager().setAmbientLight( Color(red , green , blue).normalized() );
}

} // namespace Cing