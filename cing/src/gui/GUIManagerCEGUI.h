/*
This source file is part of the Cing project
For the latest info, see http://www.cing.cc

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

#ifndef _GUIManagerCEGUI_H_
#define _GUIManagerCEGUI_H_

#include "GUIPrereqs.h"
#include "OutputDebugTextBoxCEGUI.h"

// CEGUI
#include "CEGUIInputEvent.h"

// OIS
#include "OISMouse.h"
#include "OISKeyboard.h"

// Common
#include "common/Singleton.h"

namespace Cing
{

	/**
	* @internal
	* GUI Manager based on CEGUI library
	*/
	class GUIManagerCEGUI: public SingletonStatic< GUIManagerCEGUI >, public OIS::MouseListener, public OIS::KeyListener
	{
	public:

		// Required for singleton
		friend class SingletonStatic< GUIManagerCEGUI >;

		// Destructor
		virtual ~GUIManagerCEGUI	();

		// Init / Release / Update
		void  init          ( Ogre::RenderWindow* ogreWindow, Ogre::SceneManager* ogreSceneManager );
		void  end           ();

		// GUI controll methods
		void	addGUIELement	( CEGUI::Window* guiElement );

		// Query methods
		bool						isValid       () const	{ return m_bIsValid; }
		OutputDebugTextBoxCEGUI&	getDebugOutput()		{ return m_debugOutput; }
		CEGUI::Window*				getCEGUIWindow()		{ return m_mainSheet; };

		// OIS Listeners interface
		bool mouseMoved		( const OIS::MouseEvent &arg );
		bool mousePressed	( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool mouseReleased(	 const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		// OIS Listeners interface
		bool keyPressed	( const OIS::KeyEvent &arg );
		bool keyReleased( const OIS::KeyEvent &arg );

		void messageBox			( const char* text, bool fullScreen = false );
		bool isMessageBoxVisible();
		void hideMessageBox		();


	private:

		// private constructor to ensure singleton
		GUIManagerCEGUI();

		// private methods
		CEGUI::MouseButton convertOISButtonToCEGUI( OIS::MouseButtonID buttonID );

		// Attributes
		CEGUI::OgreRenderer*		m_CEGUIRenderer;	///< CEGUI Renderer. Connects gui system with render system
		CEGUI::System*				m_CEGUISystem;		///< Main CEGUI object
		CEGUI::Window*				m_mainSheet;		///< Main gui sheet: place where all gui elements will be placed<s
		CEGUI::FrameWindow*			m_messageBoxWindow;
		OutputDebugTextBoxCEGUI		m_debugOutput;		///< Text Box to output debug messages
		bool						m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace Cing

#endif // _GUIManagerCEGUI_H_
