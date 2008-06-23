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

#ifndef _GUIManagerCEGUI_H_
#define _GUIManagerCEGUI_H_

#include "GUIPrereqs.h"
#include "OutputDebugTextBoxCEGUI.h"

// CEGUI
#include "externLibs/Ogre3d/include/cegui/CEGUIInputEvent.h"

// OIS
#include "externLibs/Ogre3d/include/ois/OISMouse.h"

// Common
#include "common/Singleton.h"

namespace GUI
{

/**
 * @internal
 * GUI Manager based on CEGUI library
 */
class GUIManagerCEGUI: public Common::SingletonStatic< GUIManagerCEGUI >, public OIS::MouseListener
{
public:

	// Required for singleton
	friend class Common::SingletonStatic< GUIManagerCEGUI >;

	// Destructor
	virtual ~GUIManagerCEGUI	();

	// Init / Release / Update
	void  init          ( Ogre::RenderWindow* ogreWindow, Ogre::SceneManager* ogreSceneManager );
	void  end           ();

	// GUI controll methods
	void	addGUIELement	( CEGUI::Window* guiElement );

	// Query methods
	bool										isValid       () const { return m_bIsValid; }
	OutputDebugTextBoxCEGUI&	getDebugOutput()			 { return m_debugOutput; }

	// OIS Listeners interface
	bool mouseMoved		( const OIS::MouseEvent &arg );
	bool mousePressed	( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


private:

	// private constructor to ensure singleton
	GUIManagerCEGUI();

	// private methods
	CEGUI::MouseButton convertOISButtonToCEGUI( OIS::MouseButtonID buttonID );

	// Attributes
	CEGUI::OgreCEGUIRenderer*		m_CEGUIRenderer;	///< CEGUI Renderer. Connects gui system with render system
	CEGUI::System*							m_CEGUISystem;		///< Main CEGUI object
	CEGUI::Window*							m_mainSheet;			///< Main gui sheet: place where all gui elements will be placed<s
	OutputDebugTextBoxCEGUI			m_debugOutput;		///< Text Box to output debug messages
	bool												m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace GUI

#endif // _GUIManagerCEGUI_H_
