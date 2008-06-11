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

// Graphics includes
#include "GraphicsManager.h"
#include "Window.h"

// Framework
#include "framework/UserAppGlobals.h"

// Common includes
#include "common/Exception.h"
#include "common/Release.h"

// Ogre includes
#include "OgreRoot.h"
#include "OgreConfigFile.h"
#include "OgreRenderWindow.h"
#include "externLibs/Ogre3d/include/OgreRenderSystem.h"
#include "externLibs/Ogre3d/include/OgreBillboard.h"
#include "externLibs/Ogre3d/include/OgreBillboardSet.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"

// GUI
#include "gui/GUIManager.h"


namespace Graphics
{
  
/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
GraphicsManager::GraphicsManager():
  m_bIsValid    ( false ),
  m_pSceneManager( NULL )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
GraphicsManager::~GraphicsManager()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @return true if the initialization was ok | false otherwise
 */
bool GraphicsManager::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

	// Get reference to Ogre Root
	Ogre::Root& ogreRoot = Ogre::Root::getSingleton();

  // Show config dialog
  if ( !ogreRoot.restoreConfig() && !ogreRoot.showConfigDialog() )
    THROW_EXCEPTION( "User canceled the config dialog!" );

  // Init rendering engine and create main window
  Ogre::RenderWindow* ogreWindow = ogreRoot.initialise( true, "Vision Library Demo" );       
  if ( !ogreWindow )
    THROW_EXCEPTION( "Error creating application window" );

  // Create main window
  m_mainWindow.init( ogreWindow );

  // Create the scene manager
  m_pSceneManager = ogreRoot.createSceneManager( Ogre::ST_GENERIC );
  
  // PreInit GUI Manager (QuickGUI requirements)
  GUI::GUIManager::getSingleton().preInit();

	// Initialize graphics resources, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  // Init the main camera
  m_activeCamera.init( m_pSceneManager );

  // Make the camera render in the main window
  m_mainWindow.attachCameraToWindow( m_activeCamera );

  // Background color
  //m_mainWindow.setBackgroundColor( Color( 0.8, 0.8, 0.8 ) );

  // Init the default font / text
  m_defaultFont.init();  
  m_defaultText.init();
  m_defaultText.setPos( 0.01f, 0.9f );		        // Text position, using relative co-ordinates
  m_defaultText.setCol( 1.0f,1.0f,1.0f,0.5f );	// Text color (Red, Green, Blue, Alpha)  

	// Init the debug overlay
  // TODO
  //m_debugOverlay.init();

	// Use default camera controller
	useDefault3DCameraControl( true );

	// Init the cvFont
	cvInitFont(&m_cvFont, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6, 0, 2);

  // Init GUI Manager
  GUI::GUIManager::getSingleton().init();

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void GraphicsManager::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;
  
  // Release GUI Manager
  GUI::GUIManager::getSingleton().end();

	// Release camera stuff
	m_defaultCamController.end();
	m_activeCamera.end();

  // Release scene manager
	Ogre::Root::getSingleton().destroySceneManager( m_pSceneManager );
	m_pSceneManager = NULL;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Renders everything in the scene
 */
void GraphicsManager::draw()
{
	// Update default camera controller
	m_defaultCamController.update();

  // Render scene
	Ogre::Root::getSingleton().renderOneFrame();

  // Update window
  m_mainWindow.update();

  // Get Frame stats
  const Ogre::RenderTarget::FrameStats& frameStats = m_mainWindow.getFrameStats();

  // Show fps
  std::ostringstream oss;
  oss << "FPS: " << frameStats.lastFPS;
  m_defaultText.setText( oss.str() );	// Text to be displayed
}


/**
 * @internal
 * @brief   Returns true if the system supports vertex programs (vertex shaders)
 * @return  true if the system supports vertex programs (vertex shaders)
 */
bool GraphicsManager::hasVertexProgramsSupport() const
{
  // Get system capabilities
  const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();

  // Check vertex programs
  if ( caps->hasCapability( Ogre::RSC_VERTEX_PROGRAM ) )
    return true;

  // Not supported
  return false;
}

/**
 * @internal
 * @brief   Returns true if the system supports fragment programs (pixel shaders)
 * @return  true if the system supports pixel programs (pixel shaders)
 */
bool GraphicsManager::hasFragmentProgramsSupport() const
{
  // Get system capabilities
  const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();

  // Check fragment programs
  if ( caps->hasCapability( Ogre::RSC_FRAGMENT_PROGRAM ) )
    return true;

  // Not supported
  return false;
}


/**
 * @internal
 * @brief   Returns true if the system supports bump mapping.
 * To support bump mapping the system need support of: vertex programs, and (fragment programs or dot3 texture
 * in the fixed-function pipeline)
 * @return  true if the system supports bump mapping
 */
bool GraphicsManager::hasBumpMappingSupport() const
{
  // Get system capabilities
  const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();

  // Check capabilities programs
  if (  caps->hasCapability( Ogre::RSC_VERTEX_PROGRAM ) && 
        ( caps->hasCapability( Ogre::RSC_FRAGMENT_PROGRAM ) || caps->hasCapability( Ogre::RSC_DOT3 ) ) )
    return true;

  // Not supported
  return false;
}

/**
 * @brief Sets the render mode used to draw the objects in the scene
 *
 * @param mode Active render mode. Possible values: POINTS, WIREFRAME, SOLID
 */
void GraphicsManager::setRenderMode( RenderMode mode )
{
	m_activeCamera.getOgreCamera()->setPolygonMode( (Ogre::PolygonMode) mode );
}

/**
 * @brief Sets the fill color used to draw all shapes rendered after this call.
 *
 * @param Color color to use to fill shapes
 */
void GraphicsManager::setFillColor( const Color& color )
{
	m_fillColor = color;

	// We are using the emissive color to fake the fill color with lighting activated
	// TODO dejar esto bien
	m_pSceneManager->setAmbientLight( m_fillColor );
}

/**
 * @brief Sets the border color used to draw shapes after this call.
 *
 * @param Color color to use to draw the border of shapes
 */
void GraphicsManager::setStrokeColor( const Color& color )
{
	m_strokeColor = color;
}

/**
 * @brief Sets the border color used to draw shapes after this call.
 *
 * @param Color color to use to draw the border of shapes
 */
void GraphicsManager::setStrokeWeight( int weight )
{
	m_strokeWeight = weight;
}


/**
 * @brief Allows to enable or disable the default 3d camera control
 *
 * @param useDefault If true, the default camera control will be enabled. 
 * If false, it will be disable, so the user will be reposible to control the 3d camera
 * if it is required.
 *
 * @note The default camera control is composed by:
 * - mouse: controls rotation
 * - keyboard:
 *		- arrow keys: moves forward/backward/left/right
 *		- pg up/down: moves camera up and down
 *		- r: restore camera rotation to initial settings
 */
void GraphicsManager::useDefault3DCameraControl( bool useDefault )
{
	// Enable controller
	if ( useDefault )
		m_defaultCamController.init( m_activeCamera );
	// Enable controller
	else
		m_defaultCamController.end();	
}

} // namespace Graphics