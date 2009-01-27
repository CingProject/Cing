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

// Graphics includes
#include "GraphicsManager.h"
#include "Window.h"
#include "ImageResourceManager.h"
#include "TexturedQuad.h"
#include "Image.h"

// Framework
#include "framework/UserAppGlobals.h"

// Common includes
#include "common/Exception.h"
#include "common/Release.h"
#include "common/MathUtils.h"
#include "common/CommonConstants.h"

// Ogre includes
#include "externLibs/Ogre3d/include/OgreRoot.h"
#include "externLibs/Ogre3d/include/OgreConfigFile.h"
#include "externLibs/Ogre3d/include/OgreRenderWindow.h"
#include "externLibs/Ogre3d/include/OgreRenderSystem.h"
#include "externLibs/Ogre3d/include/OgreBillboard.h"
#include "externLibs/Ogre3d/include/OgreBillboardSet.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"

// TEMP
#include "externLibs/Ogre3d/include/OgreTextAreaOverlayElement.h"

// GUI
//#include "gui/GUIManager.h"


namespace Graphics
{
  
/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
GraphicsManager::GraphicsManager():
  m_bIsValid    ( false ),
	m_showFps			( false ),
  m_pSceneManager( NULL ),
	m_lines( NULL),
	m_linesNode( NULL),
	m_canvas( NULL ),
	m_fill( true ),
	m_stroke( true ),
	m_smooth( false ),
	m_rectMode( CORNER ),
	m_ellipseMode( CENTER )
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
  if ( /*!ogreRoot.restoreConfig() &&*/ !ogreRoot.showConfigDialog() )
    THROW_EXCEPTION( "User canceled the config dialog!" );

  // Init rendering engine and create main window
  Ogre::RenderWindow* ogreWindow = ogreRoot.initialise( true, "Vision Library Demo" );       
	  if ( !ogreWindow )
    THROW_EXCEPTION( "Error creating application window" );

  // Create main window
  m_mainWindow.init( ogreWindow );

	// Set global window size variables
	Globals::width = m_mainWindow.getWidth();
	Globals::height = m_mainWindow.getHeight();

  // Create the scene manager
  m_pSceneManager = ogreRoot.createSceneManager( Ogre::ST_GENERIC );
  
  // PreInit GUI Manager (QuickGUI requirements)
  //GUI::GUIManager::getSingleton().preInit();

	// Initialize graphics resources, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Init ImageResourceManager 
	ImageResourceManager::getSingleton().init();

  // Init the main camera
   m_activeCamera.init( m_pSceneManager );

  // Make the camera render in the main window
  m_mainWindow.attachCameraToWindow( m_activeCamera );

  // Background color
  m_mainWindow.setBackgroundColor( Color( 200, 200, 200 ) );

  // Init the default font / text
  m_defaultFont.init();  
  m_defaultText.init();
  m_defaultText.setPos( 0.01f, 0.01f );		        // Text position, using relative co-ordinates
  m_defaultText.setCol( 0.3f, 0.3f, 0.3f, 1.0f );	// Text color (Red, Green, Blue, Alpha)  

	// Init the debug overlay
  // TODO
  //m_debugOverlay.init();

	// Use default camera controller
	useDefault3DCameraControl( true );

	// Init the cvFont
	cvInitFont(&m_cvFont, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6, 0, 2);

  // Init GUI Manager
	//GUI::GUIManager::getSingleton().init();

	// Set the global pointer to the scene manager and camera
	Globals::ogreSceneManager	= m_pSceneManager;
	Globals::ogreCamera				= m_activeCamera.getOgreCamera();

	// Init objects to allow 3d simple primitives drawing (lines...)
	m_lines     = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
	m_linesNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode("Graphics::Lines");
	m_linesNode->attachObject(m_lines);

	m_lines->addPoint(0,0,0);
	m_lines->update();
	m_linesPoints.push_back( Ogre::Vector3(0,0,0));

	// Init 2dCanvas
	m_canvas = new Image(Globals::width, Globals::height, RGB);

	// Init style queue
	m_styles.push_front( Style() );

	// Init transform stack
	m_transforms.push( Transform() );

	// Init Globals::pixels
	for (int i = 0; i < m_canvas->getWidth() * m_canvas->getHeight(); i++)
		Globals::pixels.push_back( Color::Black );

	// The class is now initialized
	m_bIsValid = true;

	return true;
}


/**
* @internal 
* @brief Add a new vertex (for line drawing)
*
* @param Vector
*/
void GraphicsManager::addVertex( Common::Vector newPos )
{
	m_linesPoints.push_back( (Ogre::Vector3) newPos);
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
  //GUI::GUIManager::getSingleton().end();

	// Release camera stuff
	m_defaultCamController.end();
	m_activeCamera.end();

  // Release scene manager
	Ogre::Root::getSingleton().destroySceneManager( m_pSceneManager );
	m_pSceneManager = NULL;

	// Release image resource manager
	ImageResourceManager::getSingleton().end();

	// Release 3d lines...
	m_lines     = NULL;
	m_linesNode = NULL;

	// Release 2dCanvas...
  m_canvas->end();
	m_canvas = NULL;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Renders everything in the scene
 */
void GraphicsManager::draw()
{

	// Draw 2d Canvas in the background
	m_canvas->drawBackground(	0,
														0,
														m_mainWindow.getOgreWindow()->getViewport(0)->getActualWidth(),
														m_mainWindow.getOgreWindow()->getViewport(0)->getActualHeight());

	// Update lines
	if (m_lines->getNumPoints()!= m_linesPoints.size())
	{
		m_lines->clear();
		for (int i=0; i<(int)m_linesPoints.size(); i++) {
			m_lines->addPoint(m_linesPoints[i]);
		}
	}else{
		for (int i=0; i<(int)m_linesPoints.size(); i++) {
			m_lines->setPoint(i,m_linesPoints[i]);
		}
	}
	m_lines->update();

	// Clear the line points buffer
	m_linesPoints.clear();

	// Update default camera controller
	m_defaultCamController.update();

  // Render scene
	Ogre::Root::getSingleton().renderOneFrame();

  // Update window
  m_mainWindow.update();

  // Get Frame stats
  const Ogre::RenderTarget::FrameStats& frameStats = m_mainWindow.getFrameStats();

	Globals::frameRate = frameStats.avgFPS;

	// Show fps
	if ( m_showFps )
	{
		std::ostringstream oss;
		oss << "FPS: " << frameStats.lastFPS;
		m_defaultText.setText( oss.str() );	// Text to be displayed
		m_defaultText.show( true );
	}
	else
		m_defaultText.show( false );

	// Mark all drawable images as not visible
	std::list< TexturedQuad* >::iterator it = m_drawableImagesQueue.begin();
	for (; it != m_drawableImagesQueue.end(); ++it )
		(*it)->setVisible( false );

	// Reset matrix stacks
	clearMatrixStack();
}
/**
 * @internal
 * @brief   Reset style stack
 */
void GraphicsManager::clearStyleStack()
{
	m_styles.clear();
	m_styles.push_front(Style());
};
/**
 * @internal
 * @brief   Reset matrix stack
 */
void GraphicsManager::clearMatrixStack()
{
	while ( !m_transforms.empty() )
		m_transforms.pop();
	m_transforms.push(Transform());
};

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
	m_styles.front().m_fillColor = color;

	// We are using the emissive color to fake the fill color with lighting activated
	// TODO dejar esto bien
	m_pSceneManager->setAmbientLight( m_styles.front().m_fillColor );

	m_fill = true;
}

/**
 * @brief Sets the border color used to draw shapes after this call.
 *
 * @param Color color to use to draw the border of shapes
 */
void GraphicsManager::setStrokeColor( const Color& color )
{
	m_styles.front().m_strokeColor = color;

	m_stroke = true;
}

/**
 * @brief Sets the border color used to draw shapes after this call.
 *
 * @param Color color to use to draw the border of shapes
 */
void GraphicsManager::setStrokeWeight( int weight )
{
	m_styles.front().m_strokeWeight = weight;
}

/**
 * @internal 
 * @brief Makes the frames per second to be printed on the screen or not
 *
 * @param show if true the current fps wil be printed on screen, if false, it won't be printed
 */
void GraphicsManager::showFps( bool show )
{
	m_showFps = show;
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

/**
 * @internal 
 * @brief Informs that an image is created (so it can be be drawn), it will be made invisible after each frame is rendered.
 * This way, if the user does not call the draw method for the same image in any frame, it won't be rendered
 * @note This is a bit triky, but allows to emulate software rendering (this is, the image is renderd just when the draw method
 * is called), when it is really working with ogre's 3d scene manager
 *
 * @param img Image that is going to be rendered
 */
void GraphicsManager::addDrawableImage( TexturedQuad* img)
{
	m_drawableImagesQueue.push_back( img );
}

/**
 * @internal 
 * @brief Informs that an image that was added as drawable image (@sa addDrawableImage) is being released
 *
 * @param img Image that is going to be rendered
 */
void GraphicsManager::removeDrawableImage( TexturedQuad* img)
{
	std::list< TexturedQuad* >::iterator it = m_drawableImagesQueue.begin();
	for (; it != m_drawableImagesQueue.end(); ++it )
		if ( *it == img )
		{
			m_drawableImagesQueue.erase( it );
			return;
		}
}

/**
 * @brief Modifies the location from which rectangles draw
 */
void GraphicsManager::setRectMode( int	mode )
{
	m_rectMode = mode;
}

/**
 * @brief Modifies the location from which ellipses draw
 */
void GraphicsManager::setEllipseMode( int	mode )
{
	m_ellipseMode = mode;
}


/**
 * @internal 
 * @brief Modifies the background of the window 
 * (really the main viewport in the window)
 * @param[in] color Color to set to the background
 */
void GraphicsManager::setBackgroundColor( const Color& color )
{
  if ( !isValid() )
    return;
	m_mainWindow.getOgreWindow()->getViewport(0)->setBackgroundColour( color.normalized() );

	cvSet( &m_canvas->getCVImage(), cvScalar(color.b,color.g,color.r) );
	m_canvas->setUpdateTexture(true);
}
} // namespace Graphics
