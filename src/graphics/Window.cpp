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

#include "Window.h"
#include "Camera3D.h"

// Ogre includes
#include "OgreRenderWindow.h"
#include "OgreWindowEventUtilities.h"
#include "OgreCamera.h"

// Common includes
#include "common/Exception.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Window::Window():
  m_bIsValid      ( false ),
  m_mainViewport  ( NULL  ),
  m_stats         ( NULL  ),
	m_width					( -1    ),
	m_height				( -1    ),
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
  // Pump windows messages
  Ogre::WindowEventUtilities::messagePump();

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
 * @internal
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
void Window::attachCameraToWindow( Camera3D& camera )
{
  if ( !isValid() )
    return;

  // Create the viewport
  m_mainViewport = m_pOgreWindow->addViewport( camera.getOgreCamera() );

  // Set the camera aspect ratio to match the window's aspect ratio
  camera.getOgreCamera()->setAspectRatio( float(m_mainViewport->getActualWidth()) / m_mainViewport->getActualHeight() ); 
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

} // namespace Graphics