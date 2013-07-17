///*
//  This source file is part of the Cing project
//  For the latest info, see http://www.cing.cc
//
//  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
// 
//  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//*/
//
//// Precompiled headers
#include "Cing-Precompiled.h"
//
//#include "GUIManagerCEGUI.h"
//
//// Framework
//#include "framework/UserAppGlobals.h"
//
//// Input
//#include "input/InputManager.h"
//
//// Common
//#include "common/CommonUtilsIncludes.h"
//
//// CEGUI
//#include "CEGUIOgreRenderer.h"
//#include "CEGUI.h"
//#include "elements/CEGUIScrollablePane.h"
//#include "elements/CEGUIListbox.h"
//
//// Graphics
//#include "graphics/GraphicsManager.h"
//
//
//namespace Cing
//{
//
///**
// * @internal
// * @brief Constructor. Initializes class attributes.
// */
//GUIManagerCEGUI::GUIManagerCEGUI():
//	m_CEGUIRenderer( NULL ),
//	m_CEGUISystem( NULL ),
//	m_mainSheet( NULL ),
//	m_messageBoxWindow( NULL ),
//	m_bIsValid  ( false )
//{
//}
//
///**
// * @internal
// * @brief Destructor. Class release.
// */
//GUIManagerCEGUI::~GUIManagerCEGUI()
//{
//	// Release resources
//	end();
//}
//
///**
// * @internal
// * @brief  Initializes the gui system
// *
// * @param ogreWindow Ogre Window where the application will be rendered
// * @param ogreSceneManager Ogre Scene manager where the gui syste will insert its stuff
// */
//void GUIManagerCEGUI::init( Ogre::RenderWindow* ogreWindow, Ogre::SceneManager* ogreSceneManager )
//{
//	// Init CEGUI for version 0.7
//	m_CEGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem( *GraphicsManager::getSingleton().getMainWindow().getOgreWindow() );
//	m_CEGUISystem	= CEGUI::System::getSingletonPtr();
//
//	// Set the actual window size to CEGUI
//	m_CEGUIRenderer->setDisplaySize( CEGUI::Size(Ogre::Real(width), Ogre::Real(height)));
//	m_CEGUISystem->notifyDisplaySizeChanged(CEGUI::Size(Ogre::Real(width), Ogre::Real(height)));
//
//	// Working for CEGUI 0.6
//	//m_CEGUIRenderer = new CEGUI::OgreCEGUIRenderer( ogreWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, ogreSceneManager );
//	//m_CEGUISystem = new CEGUI::System( m_CEGUIRenderer );
//
//	// Register this class as OIS input listener to receive mouse notifications
//	// TODO: register also as keyBoardListener
//	InputManager::getSingleton().getMouse().addListener( this );
//	InputManager::getSingleton().getKeyboard().addListener( this );
//
//	// Select available skin sets
//	CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");
//	CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");
//	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
//
//	// Set mouse cursor and font
//	//m_CEGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"WindowsLook", (CEGUI::utf8*)"MouseArrow");
//	//m_CEGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
//	CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
//
//	// Create default sheet to place GUI elements
//	CEGUI::WindowManager&	win = CEGUI::WindowManager::getSingleton();
//	m_mainSheet = win.createWindow( "DefaultGUISheet", "Vision/DefaultGUISheet");
//	m_mainSheet->setSize( CEGUI::UVector2(CEGUI::UDim(0, (float)width), CEGUI::UDim(0, (float)height) ) );
//	m_CEGUISystem->setGUISheet( m_mainSheet );
//
//	// Now the gui managet is valid
//	m_bIsValid = true;
//
//	// Create the default message box
//	m_messageBoxWindow = (CEGUI::FrameWindow*)win.createWindow( "WindowsLook/StaticText", "MessageBox" );
//	m_messageBoxWindow->setPosition( CEGUI::UVector2( cegui_absdim(0), cegui_absdim(0) ) );
//	m_messageBoxWindow->setSize( CEGUI::UVector2( cegui_absdim((float)width), cegui_absdim((float)height) ) );
//	m_messageBoxWindow->setProperty("VertFormatting", "TopAligned");
//	m_messageBoxWindow->setProperty("HorzFormatting", "LeftAligned");
//	m_messageBoxWindow->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
//	m_messageBoxWindow->setProperty("BackgroundColours", "tl:77777777 tr:77777777 bl:77777777 br:77777777");
//	m_messageBoxWindow->setVisible( false );
//
//	// Add it to the sheet
//	addGUIELement( m_messageBoxWindow );
//
//
//	// Init debug output
//	m_debugOutput.init();
//	m_debugOutput.setVisible( false );
//}
//
///**
// * @internal
// * @brief Releases the class resources.
// * After this call no method of this object can be called without calling init method again.
// */
//void GUIManagerCEGUI::end()
//{
//	m_debugOutput.end();
//	CEGUI::System::destroy();
//	//CEGUI::OgreRenderer::destroySystem();
//	m_bIsValid = false;
//}
//
///**
// * @internal
// * @brief
// *
// * @param
// */
//void GUIManagerCEGUI::addGUIELement( CEGUI::Window* guiElement )
//{
//	if ( !isValid() )
//	{
//		LOG_ERROR( "Trying to add a GUI element to a GUIManager that has not been initialized" );
//		return;
//	}
//
//	if ( m_mainSheet )
//		m_mainSheet->addChildWindow( guiElement );
//}
//
///**
// * @internal
// * Mouse pressed handler
// *
// * @param event Contains the information about the mouse
// */
//bool GUIManagerCEGUI::mouseMoved( const OIS::MouseEvent& event )
//{
//	if ( !isValid() )
//		return false;
//
//	// Inject mouse wheel and position
//	// NOTE: we are injecting the absolute position (instead of relative), because is the only way
//	// to math OIS and CEGUI mouse coordinates when in windowed mode
//	m_CEGUISystem->injectMouseWheelChange((float)event.state.Z.rel);
//	m_CEGUISystem->injectMousePosition( (float)event.state.X.abs, (float)event.state.Y.abs);
//
//	return true;
//}
//
///**
// * @internal
// * Mouse pressed handler
// *
// * @param event Contains the information about the mouse
// * @param id button that has been pressed
// */
//bool GUIManagerCEGUI::mousePressed( const OIS::MouseEvent& event, OIS::MouseButtonID id  )
//{
//	if ( !isValid() )
//		return false;
//
//	// Notify CEGUI about the mouse event
//	m_CEGUISystem->injectMouseButtonDown( convertOISButtonToCEGUI(id) );
//
//	return true;
//}
//
//
///**
// * @internal
// * Mouse released handler
// *
// * @param event Contains the information about the mouse
// * @param id button that has been released
// */
//bool GUIManagerCEGUI::mouseReleased( const OIS::MouseEvent& event, OIS::MouseButtonID id  )
//{
//	if ( !isValid() )
//		return false;
//
//	// Notify CEGUI about the mouse event
//	m_CEGUISystem->injectMouseButtonUp( convertOISButtonToCEGUI(id) );
//
//	return true;
//}
//
//bool GUIManagerCEGUI::keyPressed( const OIS::KeyEvent &arg )
//{
//	if ( !isValid() )
//		return false;
//
//	// Notify CEGUI about the mouse event
//	m_CEGUISystem->injectKeyDown( arg.key );
//	m_CEGUISystem->injectChar( arg.text );
//	return true;
//}
//
//bool GUIManagerCEGUI::keyReleased( const OIS::KeyEvent &arg )
//{
//	if ( !isValid() )
//		return false;
//
//	// Notify CEGUI about the mouse event
//	m_CEGUISystem->injectKeyUp( arg.key );
//	return true;
//}
//
//
//
///**
// * @internal
// * @brief
// *
// * @param
// */
//void GUIManagerCEGUI::messageBox( const char* text, bool fullScreen /*= false*/ )
//{
//	if ( m_messageBoxWindow )
//	{
//		if ( fullScreen )
//		{
//			m_messageBoxWindow->setProperty("VertFormatting", "VertCentred");
//			m_messageBoxWindow->setProperty("HorzFormatting", "LeftAligned");
//			int margin = 20;
//			m_messageBoxWindow->setPosition( CEGUI::UVector2( cegui_absdim((float)margin), cegui_absdim((float)margin) ) );
//			m_messageBoxWindow->setSize( CEGUI::UVector2( cegui_absdim((float)(width-margin*2)), cegui_absdim((float)(height-margin*2)) ) );
//		}
//		else
//		{
//			m_messageBoxWindow->setProperty("VertFormatting", "VertCentred");
//			m_messageBoxWindow->setProperty("HorzFormatting", "HorzCentred");
//			m_messageBoxWindow->setPosition( CEGUI::UVector2( cegui_absdim(0), cegui_absdim((float)height/2.0f) ) );
//			m_messageBoxWindow->setSize( CEGUI::UVector2( cegui_absdim((float)width), cegui_absdim(50) ) );
//		}
//		m_messageBoxWindow->setText( text );
//		m_messageBoxWindow->setVisible( true );
//	}
//}
//
//bool GUIManagerCEGUI::isMessageBoxVisible()
//{
//	if ( m_messageBoxWindow )
//		return m_messageBoxWindow->isVisible();
//	return false;
//}
//
//void GUIManagerCEGUI::hideMessageBox()
//{
//	if ( m_messageBoxWindow )
//	{
//		m_messageBoxWindow->setVisible( false );
//	}
//}
//
//
//
//
///*
// * @internal
// * @brief Converts a OIS mouse button id to the CEGUI equivalent
// *
// * @param buttonID OIS button id
// * @return CEGUI Mouse button id
// */
//CEGUI::MouseButton GUIManagerCEGUI::convertOISButtonToCEGUI( OIS::MouseButtonID buttonID )
//{
//	switch ( buttonID )
//	{
//	case OIS::MB_Left:
//		return CEGUI::LeftButton;
//
//	case OIS::MB_Right:
//		return CEGUI::RightButton;
//
//	case OIS::MB_Middle:
//		return CEGUI::MiddleButton;
//
//	default:
//		return CEGUI::LeftButton;
//	}
//}
//
//} // namespace Cing
