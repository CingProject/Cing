/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

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

#include "DebugOverlay.h"
#include "Font.h"

// Framework
#include "framework/Application.h"

// Ogre
#include "OgreOverlayManager.h"
#include "OgreOverlayContainer.h"
#include "OgreTextAreaOverlayElement.h"

namespace Cing
{

// Static member definition
const std::string DebugOverlay::DEFAULT_DEBUG_PANEL_NAME    = "DebugPanel";
const std::string DebugOverlay::DEFAULT_DEBUG_TEXTAREA_NAME = "DebugTextArea";

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
DebugOverlay::DebugOverlay():
  m_bIsValid( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
DebugOverlay::~DebugOverlay()
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
bool DebugOverlay::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Check application correctly initialized (could not be if the user didn't calle size function)
  Application::getSingleton().checkSubsystemsInit();

  // Get the overlay manager
  Ogre::OverlayManager& overlayMgr = Ogre::OverlayManager::getSingleton();

  // Create the panel to show the debug info 
  Ogre::OverlayContainer* panel = static_cast< Ogre::OverlayContainer* >( overlayMgr.createOverlayElement( "Panel", DEFAULT_DEBUG_PANEL_NAME ) );
  panel->setMetricsMode( Ogre::GMM_PIXELS );

  // Set panel properties
  // TODO poner defaults
  panel->setPosition( 10, 10 );
  panel->setDimensions( 300, 120 );

  // Create a text area to show the debug text
  Ogre::TextAreaOverlayElement* textArea = static_cast< Ogre:: TextAreaOverlayElement* >( overlayMgr.createOverlayElement( "Font", DEFAULT_DEBUG_TEXTAREA_NAME ) );
  textArea->setMetricsMode(Ogre::GMM_PIXELS);
  textArea->setPosition(0, 0);
  textArea->setDimensions(300, 120);
  textArea->setCharHeight(26);

  // set the font name to the font resource that you just created.
  textArea->setFontName( "DefaultFont" );

  // say something
  textArea->setCaption( "Hello, World!" ); 

  // Create an overlay, and add the panel
  Ogre::Overlay* overlay = overlayMgr.create( "Overlay" );
  overlay->add2D(panel);

  // Add the text area to the panel
  panel->addChild(textArea);

  // Show the overlay
  overlay->show();

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void DebugOverlay::end()
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
void DebugOverlay::update()
{

}

} // namespace Cing