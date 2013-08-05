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

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

// http://stackoverflow.com/questions/274753/how-to-make-weak-linking-work-with-gcc
// if we are on Apple (clang llvm) we need to define stubs for the global app methods as weak.
// they will be overridden in the Cing application. Also requires the following added to OTHER_LINKER_FLAGS:
// -Wl,-flat_namespace,-undefined,dynamic_lookup

extern void setup()             __attribute__((weak_import));
extern void keyPressed()        __attribute__((weak_import));
extern void mouseMoved()        __attribute__((weak_import));
extern void mouseReleased()     __attribute__((weak_import));
extern void mousePressed()      __attribute__((weak_import));
extern void end()               __attribute__((weak_import));
extern void draw()              __attribute__((weak_import));

#endif

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
	bool  initApp       ();
	void  endApp        ();
	void  drawApp       ();
    void  drawOneFrame  ();

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
    bool shouldExit ();
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
    
    void            setOgreView     ( void* view );

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
    void*                           m_ogreView;
};

} // namespace Cing

#endif // _Application_H_
