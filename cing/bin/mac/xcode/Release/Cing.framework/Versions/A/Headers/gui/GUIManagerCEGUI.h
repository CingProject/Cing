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
//#ifndef _GUIManagerCEGUI_H_
//#define _GUIManagerCEGUI_H_
//
//// Precompiled headers
//#include "Cing-Precompiled.h"
//
//#include "GUIPrereqs.h"
//#include "OutputDebugTextBoxCEGUI.h"
//
//// CEGUI
//#include "CEGUIInputEvent.h"
//
//// OIS
//#include "OISMouse.h"
//#include "OISKeyboard.h"
//
//// Common
//#include "common/Singleton.h"
//
//namespace Cing
//{
//
//	/**
//	* @internal
//	* GUI Manager based on CEGUI library
//	*/
//	class GUIManagerCEGUI: public SingletonStatic< GUIManagerCEGUI >, public OIS::MouseListener, public OIS::KeyListener
//	{
//	public:
//
//		// Required for singleton
//		friend class SingletonStatic< GUIManagerCEGUI >;
//
//		// Destructor
//		virtual ~GUIManagerCEGUI	();
//
//		// Init / Release / Update
//		void  init          ( Ogre::RenderWindow* ogreWindow, Ogre::SceneManager* ogreSceneManager );
//		void  end           ();
//
//		// GUI controll methods
//		void	addGUIELement	( CEGUI::Window* guiElement );
//
//		// Query methods
//		bool						isValid       () const	{ return m_bIsValid; }
//		OutputDebugTextBoxCEGUI&	getDebugOutput()		{ return m_debugOutput; }
//		CEGUI::Window*				getCEGUIWindow()		{ return m_mainSheet; };
//
//		// OIS Listeners interface
//		bool mouseMoved		( const OIS::MouseEvent &arg );
//		bool mousePressed	( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
//		bool mouseReleased(	 const OIS::MouseEvent &arg, OIS::MouseButtonID id );
//
//		// OIS Listeners interface
//		bool keyPressed	( const OIS::KeyEvent &arg );
//		bool keyReleased( const OIS::KeyEvent &arg );
//
//		void messageBox			( const char* text, bool fullScreen = false );
//		bool isMessageBoxVisible();
//		void hideMessageBox		();
//
//
//	private:
//
//		// private constructor to ensure singleton
//		GUIManagerCEGUI();
//
//		// private methods
//		CEGUI::MouseButton convertOISButtonToCEGUI( OIS::MouseButtonID buttonID );
//
//		// Attributes
//		CEGUI::OgreRenderer*		m_CEGUIRenderer;	///< CEGUI Renderer. Connects gui system with render system
//		CEGUI::System*				m_CEGUISystem;		///< Main CEGUI object
//		CEGUI::Window*				m_mainSheet;		///< Main gui sheet: place where all gui elements will be placed<s
//		CEGUI::FrameWindow*			m_messageBoxWindow;
//		OutputDebugTextBoxCEGUI		m_debugOutput;		///< Text Box to output debug messages
//		bool						m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
//
//	};
//
//} // namespace Cing
//
//#endif // _GUIManagerCEGUI_H_
