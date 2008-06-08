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

#ifndef _GraphicsManager_H_
#define _GraphicsManager_H_

#include "GraphicsPrereqs.h"
#include "Window.h"
#include "Camera3D.h"
#include "DebugOverlay.h"
#include "Font.h"
#include "Text.h"

//TEMP
#include "input/InputPrereqs.h"

// Common
#include "common/Singleton.h"

namespace Graphics
{

/**
 * @internal
 * Manages the communication with the Graphics engine (OGRE)
 */
class GraphicsManager: public Common::SingletonStatic< GraphicsManager >
{

public:

	// Required for singleton
	friend class Common::SingletonStatic< GraphicsManager >;

	// Destructor
	virtual ~GraphicsManager														();

	// Init / Release / Draw
	bool                      init                      ();
	void                      end                       ();
	void                      draw                      ();

	// Query methods
	bool                      isValid                   () const { return m_bIsValid; }
	const Window&             getMainWindow             () const { return m_mainWindow; }
	const Camera3D&           getActiveCamera           () const { return m_activeCamera; }
	const Ogre::SceneManager& getSceneManager           () const { return *m_pSceneManager; }
	Ogre::SceneManager&       getSceneManager           ()       { return *m_pSceneManager; }

	// Common capabilities checking
	bool                      hasVertexProgramsSupport  () const;
	bool                      hasFragmentProgramsSupport() const;
	bool                      hasBumpMappingSupport     () const;

	// Color related methods
	void											setFillColor							(  const Color& color );
	void											setStrokeColor						(  const Color& color );
	const Color&							getFillColor					  	() const { return m_fillColor; }
	const Color&							getStrokeColor						() const { return m_strokeColor; }

	// Appearance drawing methods
	void setStrokeWeight			(  int weight );
	int  getStrokeWeight			() { return m_strokeWeight; }

	// TEMP
	void keyPressed( const OIS::KeyEvent &event );

private:

	// private constructor to ensure singleton
	GraphicsManager      ();

	//TODO: poner decente
	Ogre::Light*      pLight;
	Ogre::SceneNode*  pLightSceneNode;


	// Attributes
	Ogre::SceneManager*   m_pSceneManager;  ///< Main scene manager
	Window                m_mainWindow;     ///< Main application window
	Camera3D              m_activeCamera;   ///< Active camera
	DebugOverlay          m_debugOverlay;   ///< Debug overlay used to show debug information
	Font                  m_defaultFont;    ///< Default system font
	Text                  m_defaultText;    ///< To print text to screen
	Color									m_fillColor;			///< Color used to fill shapes
	Color									m_strokeColor;		///< Color used to draw shapes
	float									m_strokeWeight;		///< Width of the stroke used for draw lines, points, and the border around shapes
	bool                  m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	};

} // namespace Graphics

#endif // _GraphicsManager_H_
