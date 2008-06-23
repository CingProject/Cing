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

#include "GUIManagerCEGUI.h"

// Framework
#include "framework/UserAppGlobals.h"

// Input
#include "input/InputManager.h"

// Common
#include "common/CommonUtils.h"

// CEGUI
#include "externLibs/Ogre3d/include/OgreCEGUIRenderer.h"
#include "externLibs/Ogre3d/include/cegui/CEGUI.h"
#include "externLibs/Ogre3d/include/cegui/elements/CEGUIScrollablePane.h"
#include "externLibs/Ogre3d/include/cegui/elements/CEGUIListbox.h"


namespace GUI
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
GUIManagerCEGUI::GUIManagerCEGUI():
	m_CEGUIRenderer( NULL ),
	m_CEGUISystem( NULL ),
	m_mainSheet( NULL ),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
GUIManagerCEGUI::~GUIManagerCEGUI()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief  Initializes the gui system
 * 
 * @param ogreWindow Ogre Window where the application will be rendered
 * @param ogreSceneManager Ogre Scene manager where the gui syste will insert its stuff
 */
void GUIManagerCEGUI::init( Ogre::RenderWindow* ogreWindow, Ogre::SceneManager* ogreSceneManager )
{
	// Init CEGUI
	m_CEGUIRenderer = new CEGUI::OgreCEGUIRenderer( ogreWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, ogreSceneManager );
	m_CEGUISystem = new CEGUI::System( m_CEGUIRenderer );

	// Register this class as OIS input listener to receive mouse notifications
	// TODO: register also as keyBoardListener
	Input::InputManager::getSingleton().getMouse().addListener( this );

	// Select available skin sets
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"WindowsLook.scheme");
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

	// Set mouse cursor and font
	//m_CEGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	//m_CEGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
	CEGUI::FontManager::getSingleton().createFont("Iconified-12.font");

	// Create default sheet to place GUI elements
	CEGUI::WindowManager&	win = CEGUI::WindowManager::getSingleton();
	m_mainSheet = win.createWindow( "DefaultGUISheet", "Vision/DefaultGUISheet");
	m_mainSheet->setSize( CEGUI::UVector2(CEGUI::UDim(0, Globals::width), CEGUI::UDim(0, Globals::height) ) ); 
	m_CEGUISystem->setGUISheet( m_mainSheet );

	// Now the gui managet is valid
	m_bIsValid = true;

	// Init debug output
	m_debugOutput.init();
	m_debugOutput.setVisible( false );
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void GUIManagerCEGUI::end()
{
	m_debugOutput.end();
	Common::Release( m_CEGUISystem );
	Common::Release( m_CEGUIRenderer );
	m_bIsValid = false;
}

/**
 * @internal 
 * @brief 
 *
 * @param
 */
void GUIManagerCEGUI::addGUIELement( CEGUI::Window* guiElement )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to add a GUI element to a GUIManager that has not been initialized" );
		return;
	}

	m_mainSheet->addChildWindow( guiElement );
}

/**
 * @internal
 * Mouse pressed handler
 *
 * @param event Contains the information about the mouse
 */
bool GUIManagerCEGUI::mouseMoved( const OIS::MouseEvent& event )
{
	if ( !isValid() )
		return false;

	// Inject mouse wheel and position
	// NOTE: we are injecting the absolute position (instead of relative), because is the only way
	// to math OIS and CEGUI mouse coordinates when in windowed mode
	m_CEGUISystem->injectMouseWheelChange(event.state.Z.rel);
	m_CEGUISystem->injectMousePosition( event.state.X.abs, event.state.Y.abs);

	return true;
}

/**
 * @internal
 * Mouse pressed handler
 *
 * @param event Contains the information about the mouse
 * @param id button that has been pressed
 */
bool GUIManagerCEGUI::mousePressed( const OIS::MouseEvent& event, OIS::MouseButtonID id  )
{
	if ( !isValid() )
		return false;

	// Notify CEGUI about the mouse event
	m_CEGUISystem->injectMouseButtonDown( convertOISButtonToCEGUI(id) );

	return true;
}


/**
 * @internal
 * Mouse released handler
 *
 * @param event Contains the information about the mouse
 * @param id button that has been released
 */
bool GUIManagerCEGUI::mouseReleased( const OIS::MouseEvent& event, OIS::MouseButtonID id  )
{
	if ( !isValid() )
		return false;

	// Notify CEGUI about the mouse event
	m_CEGUISystem->injectMouseButtonUp( convertOISButtonToCEGUI(id) );

	return true;
}

/*
 * @internal
 * @brief Converts a OIS mouse button id to the CEGUI equivalent
 *
 * @param buttonID OIS button id
 * @return CEGUI Mouse button id
 */
CEGUI::MouseButton GUIManagerCEGUI::convertOISButtonToCEGUI( OIS::MouseButtonID buttonID )
{
	switch ( buttonID )
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}

} // namespace gui