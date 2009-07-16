/**
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

#include "Font(Deprecated).h"

// Ogre
#include "Ogre3d/include/OgreFontManager.h"

// Framework
#include "framework/Application.h"

namespace Graphics
{

// Static member definition
const std::string Font_Deprecated::DEFAULT_FONT_NAME       = "DefaultFont";
const int         Font_Deprecated::DEFAULT_FONT_SIZE       = 16;
const int         Font_Deprecated::DEFAULT_FONT_RESOLUTION = 96;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Font_Deprecated::Font_Deprecated():
  m_bIsValid( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Font_Deprecated::~Font_Deprecated()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @return true if the initialization was ok | false otherwise
 */
bool Font_Deprecated::init( const std::string& name /*= DEFAULT_FONT_NAME*/, int size /*= DEFAULT_FONT_SIZE*/, int resolution /*= DEFAULT_FONT_RESOLUTION*/ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Check application correctly initialized (could not be if the user didn't calle size function)
  Framework::Application::getSingleton().checkSubsystemsInit();

  // Get the font manager
  Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();

  // Load the font
  Ogre::ResourcePtr font = fontMgr.getByName( DEFAULT_FONT_NAME );
  font->load();


	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Font_Deprecated::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Updates the class state
 */
void Font_Deprecated::update()
{

}

} // namespace Graphics