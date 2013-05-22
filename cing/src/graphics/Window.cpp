/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#include "Window.h"
#include "Camera3D.h"

// Ogre includes
#include "OgreRenderWindow.h"
#include "OgreWindowEventUtilities.h"
#include "OgreCamera.h"

// Common includes
#include "common/Exception.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Window::Window():
	m_bIsValid      ( false ),
	m_mainViewport  ( NULL  ),
	m_stats         ( NULL  ),
	m_width			( -1    ),
	m_height		( -1    ),
	m_pOgreWindow   ( NULL  )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Window::~Window()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] pOgreWindow Pointer to the previously created Ogre window handle
 * @return true if the initialization was ok | false otherwise
 */
bool Window::init( Ogre::RenderWindow* pOgreWindow )
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Check window pointer
	if ( !pOgreWindow )
		THROW_EXCEPTION( "Internal Error: Invalid window" );


	// Store Ogre window handle
	m_pOgreWindow = pOgreWindow;

	// The class is now initialized
	m_bIsValid = true;

	// Store window size
	TWindowMetrics metrics;
	getMetrics( metrics );
	m_width		= metrics.width;
	m_height	= metrics.height;

	// Init the stats
	m_stats = &m_pOgreWindow->getStatistics();
	
	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Window::end()
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
void Window::update()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// Pump windows messages
	// This is block only on mac: more info: http://www.ogre3d.org/forums/viewtopic.php?f=5&t=48491&start=100
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	Ogre::WindowEventUtilities::messagePump();
//#endif

	// Get window statistics	
	m_stats = &m_pOgreWindow->getStatistics();
}

/**
 * @internal
 * @brief Returns true if the window is closed. False otherwise.
 * @return true if the window is closed. False otherwise.
 */
bool Window::isClosed() const 
{ 
  if ( !isValid() )
    return false;

  return m_pOgreWindow->isClosed();
}

/**
 * @brief Returns true if the window is in full screen mode. False otherwise.
 * @return true if the window is in full screen mode. False otherwise.
 */
bool Window::isFullScreen() const
{
  if ( !isValid() )
    return false;

  return m_pOgreWindow->isFullScreen();
}

/**
 * @brief Returns true if the window is active (has the focus), false otherwise
 * @return true if the window is active (has the focus), false otherwise
 */
bool Window::isActive() const
{
  if ( !isValid() )
    return false;

  return m_pOgreWindow->isActive();
}

/**
 * @internal 
 * @brief Used to retrieve the metrics of the window
 * @param[out] metrics stores the metrics of the window
 */
void Window::getMetrics( TWindowMetrics& metrics ) const
{
  if ( !isValid() )
    return;

  // Get window metrics
  int           left, top;
  unsigned int  width, height, colourDepth;
  m_pOgreWindow->getMetrics( width, height, colourDepth, left, top );

  //Fill data to return
  metrics.width         = width;
  metrics.height        = height;
  metrics.colourDepth   = colourDepth;
  metrics.left          = left;
  metrics.top           = top;
}

/**
 * @internal
 * @brief   Returns the window handle
 * @return  Returns the window handle
 */
size_t Window::getWindowHandle() const
{
  if ( !isValid() )
    return 0;

  // Get window handle
  size_t windowHandle = 0;
  m_pOgreWindow->getCustomAttribute( "WINDOW", &windowHandle );

  return windowHandle;
}


/**
 * @internal
 * @brief Specify the camera that will be rendered in this window.
 * As off this moment, this window will be the render target for this camera.
 * @note This method modifies the aspect ration of the camera to match the windows' aspect ratio
 * @param[in] camera Camera to attach to the window
 */
void Window::attachCameraToWindow( Camera3D& camera, int viewportIndex /*= 0*/ )
{
  if ( !isValid() )
    return;

  // Viewport pointer
  Ogre::Viewport* viewport = NULL;

  // Get the viewport pointer
  if ( m_pOgreWindow->getNumViewports()-1 >=  viewportIndex )
  {
	  viewport = m_pOgreWindow->getViewport(viewportIndex);
	  viewport->setCamera( camera.getOgreCamera() );
  }
  // Create the viewport if it does not exist
  else
	 viewport = m_pOgreWindow->addViewport( camera.getOgreCamera(), viewportIndex );

  // Set the camera aspect ratio to match the window's aspect ratio
  camera.getOgreCamera()->setAspectRatio( float(viewport->getActualWidth()) / viewport->getActualHeight() ); 

  // If there is no main viewport -> assign the new created 
  if ( !m_mainViewport || (viewportIndex == 0))
	  m_mainViewport = viewport;
}

/**
 * @internal
 * @brief Specify the camera that will be rendered in this window.
 * As off this moment, this window will be the render target for this camera.
 * @note This method modifies the aspect ration of the camera to match the windows' aspect ratio
 * @param[in] camera Camera to attach to the window
 */
void Window::attachCameraToWindow( Ogre::Camera* ogreCamera, int viewportIndex /*= 0*/ )
{
  if ( !isValid() )
    return;

  // Viewport pointer
  Ogre::Viewport* viewport = NULL;

  // Get the viewport pointer
  if ( m_pOgreWindow->getNumViewports()-1 >=  viewportIndex )
  {
	  viewport = m_pOgreWindow->getViewport(viewportIndex);
	  viewport->setCamera( ogreCamera );
  }

  // Create the viewport if it does not exist
  else
	 viewport = m_pOgreWindow->addViewport( ogreCamera, viewportIndex );

  // Set the camera aspect ratio to match the window's aspect ratio
  ogreCamera->setAspectRatio( float(viewport->getActualWidth()) / viewport->getActualHeight() ); 

  // If there is no main viewport -> assign the new created 
  if ( !m_mainViewport || (viewportIndex == 0))
	  m_mainViewport = viewport;
}

const Ogre::ColourValue& Window::getBackgroundColor()
{
  if ( !isValid() )
	  return Color::Black;

  return m_mainViewport->getBackgroundColour();
}

/**
 * @internal
 * @brief Modifies the background of the window 
 * (really the main viewport in the window)
 * @param[in] color Color to set to the background
 */
void Window::setBackgroundColor( const Color& color )
{
  if ( !isValid() )
    return;

  m_mainViewport->setBackgroundColour( color.normalized() ); 
}


Ogre::Viewport* Window::getViewport( int index )
{
  if ( !isValid() )
    return NULL;

  // Get the viewport pointer
  if ( m_pOgreWindow->getNumViewports() >  index )
	  return m_pOgreWindow->getViewport(index);
  else 
	  return NULL;
}


} // namespace Cing