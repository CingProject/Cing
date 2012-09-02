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

#ifndef _Cing_Application_H_
#define _Cing_Application_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "CingPlugin.h"

// Common
#include "common/Singleton.h"

// OIS
#include "OISMouse.h"
#include "OISKeyboard.h"

// Ogre
#include "OgreTimer.h"

namespace Cing
{

class Plugin;

/**
 * @internal
 * Base class to create an application. Manages the render loop and the initialization and
 * release of the main application systems
 */
class Application: public SingletonStatic< Application >, public OIS::KeyListener, public OIS::MouseListener
{
public:

	// Required for singleton
	friend class SingletonStatic< Application >;

	// Constructor / Destructor
	virtual ~Application();

	// Init / Release / Loop
	bool  initApp   ();
	void  endApp    ();
	void  drawApp   ();

	// Initialize application required subsystems
	void  initSubSystems      ();
	void  checkSubsystemsInit ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }

	// Event handlers
	bool mouseMoved			( const OIS::MouseEvent& event );
	bool mousePressed		( const OIS::MouseEvent& event, OIS::MouseButtonID id  );
	bool mouseReleased		( const OIS::MouseEvent& event, OIS::MouseButtonID id  );
	bool keyPressed			( const OIS::KeyEvent& event );
	bool keyReleased		( const OIS::KeyEvent& event);

	// Control application flow
	void exit  		() { m_finish = true; }
	void delay 		( unsigned int milliseconds );
	void loop  		() { m_loop = true;  };
	void noLoop		() { m_loop = false; };
	void redraw		() { m_needUpdate = true; };
	void frameRate( int forcedFrameRate );

	// Get methods
	unsigned long long getFrameCount() { return m_frameCount; };

	// Plugins for the application	
	void			registerPlugin	( Plugin& plugin );
	Cing::Plugin*	getPlugin		( const std::string& pluginName );


private:
	// private constructor to ensure singleton
	Application();

	// Internal methods
	void initPlugins	( PluginInitTime time );
	void endPlugins		( PluginEndTime time );
	void updatePlugins	( PluginUpdateTime time );

	// Attributes
	Ogre::Timer				m_timer;				///< Application timer. Used to measure time between frames
	Ogre::Timer				m_absTimer;				///< Timer to control the total time elapsed since application started
	unsigned long long		m_frameCount;   		///< Number of frames since application started
	int						m_forcedFrameRate;		////< If != 0, the application will try to execute at this frame rate
	double					m_timePerFrameMillis;	///< Time that will take every frame to execute if the fps is forced (in microseconds)
	bool					m_finish;				///< If true app will be closed in the next iteration of the app loop
	bool					m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
	bool					m_loop;         		///< If true app will be call draw() user funtion continuosly
	bool					m_needUpdate;   		///< If true app will be call draw() user funtion one time

	typedef std::list< Plugin* >	PluginList;
	PluginList						m_plugins;				///< Plugins currently active in the application
};

} // namespace Cing

#endif // _Application_H_
