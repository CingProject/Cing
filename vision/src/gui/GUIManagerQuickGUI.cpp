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

#include "GUIManagerQuickGUI.h"

// Common 
#include "common/Release.h"

// Graphics
#include "graphics/GraphicsManager.h"
#include "graphics/Camera3D.h"

//QuickGUI
#include "externLibs/QuickGUI/include/QuickGUI.h"

namespace GUI
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
GUIManagerQuickGUI::GUIManagerQuickGUI():
  m_quickGuiRoot      ( NULL  ),
  m_quickGUIManagerQuickGUI   ( NULL  ),
  m_defaultSheet      ( NULL  ),
  m_debugLabelText  ( NULL  ),
  m_bIsValid          ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
GUIManagerQuickGUI::~GUIManagerQuickGUI()
{
  // Release resources
  end();
}

/**
 * @internal
 * Registers the QuickGUI parser.
 * @note It should be called before Ogre::ResourceGroupManager::initialiseResourceGroups() call
 */
void GUIManagerQuickGUI::preInit()
{
  // Register gui parser to load resources when Ogre does it...
  QuickGUI::registerScriptParser();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @return true if the initialization was ok | false otherwise
 */
bool GUIManagerQuickGUI::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Create quickGUI root object
  m_quickGuiRoot = new QuickGUI::Root();

  // Load skin
  QuickGUI::SkinSetManager::getSingleton().loadSkin( "qgui", QuickGUI::SkinSet::IMAGE_TYPE_PNG );

  // Create and configure the GUI manager
  const Graphics::Camera3D&   activeCamera = Graphics::GraphicsManager::getSingleton().getActiveCamera();
  m_quickGUIManagerQuickGUI = m_quickGuiRoot->createGUIManager( activeCamera.getOgreCamera()->getViewport() , "qgui" );
  m_quickGUIManagerQuickGUI->setSceneManager( &(Graphics::GraphicsManager::getSingleton().getSceneManager()) );

  // Store the default sheet pointer (this sheet will used to place default widgets on it)
  m_defaultSheet = m_quickGUIManagerQuickGUI->getDefaultSheet();

  // Create debug text label (hidden)
  m_debugLabelText = m_defaultSheet->createLabel();
  m_debugLabelText->setPosition(QuickGUI::Point( 0, 0 ));  
  m_debugLabelText->hide();

  // Hide mouse
  m_quickGUIManagerQuickGUI->getMouseCursor()->hide();

  // Image test
  //QuickGUI::Image* logoImage = m_defaultSheet->createImage();
  //logoImage->setDimensions( QuickGUI::Rect(600,10,160,90) );
  //logoImage->setTexture( "qgui.checkbox.unchecked.down.png" );


	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void GUIManagerQuickGUI::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Release resources
  // TODO: delete QuickGUI::Root::getSingletonPtr();?
  Common::Release( m_quickGuiRoot );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Updates the class state
 */
void GUIManagerQuickGUI::update()
{

}


/**
 * @internal
 * @brief Makes the debug text visible
 */
void GUIManagerQuickGUI::showDebugText()
{
  m_debugLabelText->show();
}

/**
 * @internal
 * @brief Makes the debug text invisible
 */
void GUIManagerQuickGUI::hideDebugText()
{
  m_debugLabelText->hide();
}

/**
 * @internal
 * @brief Sets the current debug text
 */
void GUIManagerQuickGUI::setDebugText( const char* text )
{
  m_debugLabelText->setText( text );
}

} // namespace GUI