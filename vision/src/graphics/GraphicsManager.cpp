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

// TEMP
#include "externLibs/Ogre3d/include/ois/OISKeyboard.h"
#include "input/InputManager.h"

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
  m_pOgreRoot    ( NULL ),
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

  // Create Ogre root object
  // TODO mover plugins cfg a zona de la librería
  m_pOgreRoot = new Ogre::Root( "Plugins\\plugins.cfg" );
  //m_pOgreRoot = new Ogre::Root( "" );

  // Show config dialog
  if ( !m_pOgreRoot->restoreConfig() && !m_pOgreRoot->showConfigDialog() )
    THROW_EXCEPTION( "User canceled the config dialog!" );

  // Init rendering engine and create main window
  Ogre::RenderWindow* ogreWindow = m_pOgreRoot->initialise( true, "Vision Library Demo" );       
  if ( !ogreWindow )
    THROW_EXCEPTION( "Error creating application window" );

  // Create main window
  m_mainWindow.init( ogreWindow );

  // Create the scene manager
  m_pSceneManager = m_pOgreRoot->createSceneManager( Ogre::ST_GENERIC );
  
  // PreInit GUI Manager (QuickGUI requirements)
  GUI::GUIManager::getSingleton().preInit();

  // Load assets
  loadAssets();

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


  // Ambient lighting
  float gray = 0.1f;
  m_pSceneManager->setAmbientLight( Color(gray , gray , gray) );

  //for (unsigned int i = 0; i < NUM_LIGHTS; ++i)
  //{
  //  mLightPivots[i] = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
  //  mLightPivots[i]->rotate(mLightRotationAxes[i], mLightRotationAngles[i]);
  //  mLightPivots[i]->setPosition( Vector3( Globals::width / 2, Globals::height / 2, 100 ) );

  //  // Create a light, use default parameters
  //  mLights[i] = m_pSceneManager->createLight("Light" + Ogre::StringConverter::toString(i));
  //  Vector3 lightPos( (Globals::width / 2) + 50, (Globals::height / 2) + 50, 100 );

  //  mLights[i]->setPosition( lightPos );
  //  mLights[i]->setDiffuseColour(mDiffuseLightColours[i]);
  //  mLights[i]->setSpecularColour(mSpecularLightColours[i]);
  //  mLights[i]->setVisible(mLightState[i]);
  //  // Attach light
  //  mLightPivots[i]->attachObject(mLights[i]);

  //  // Create billboard for light
  //  mLightFlareSets[i] = m_pSceneManager->createBillboardSet("Flare" + Ogre::StringConverter::toString(i));
  //  mLightFlareSets[i]->setMaterialName("Examples/Flare");
  //  mLightPivots[i]->attachObject(mLightFlareSets[i]);
  //  mLightFlares[i] = mLightFlareSets[i]->createBillboard( lightPos );
  //  mLightFlares[i]->setColour(mDiffuseLightColours[i]);
  //  mLightFlareSets[i]->setVisible(mLightState[i]);
  //}

  // Shadow test
  //m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  //m_pSceneManager->setShadowTextureSettings(1024, 2);
  //m_pSceneManager->setShadowColour(Color(1, 0, 0));
  // Shadow test

  //pLight = m_pSceneManager->createLight( "Light" );
  //pLight->setType(Ogre::Light::LT_POINT);
  //pLight->setPosition(200, 300, 400);
  //pLight->setDiffuseColour(1, 1, 0.7);
  //pLight->setSpecularColour(1, 1, 0.7);

  //// Attach light
  //pLightSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
  //pLightSceneNode->attachObject( pLight );
  //pLightSceneNode->lookAt( Vector3( Globals::width/2, Globals::height/2, 0), Ogre::Node::TS_WORLD );
  // TODO poner decente



  // Init the debug overlay
  // TODO
  //m_debugOverlay.init();

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

  // Release resources
  // TODO
  //Common::Release( m_pSceneManager );
  //Common::Release( m_pOgreRoot );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Renders everything in the scene
 */
void GraphicsManager::draw()
{


  // TODO: poner decente
  float MoveFactor = 6;

  // Get the mouse
  const Input::Mouse& mouse = Input::InputManager::getSingleton().getMouse();

  // Move the camera around with the left button
  if ( mouse.isButtonPressed(0) ) {
    Ogre::SceneNode *camNode = m_activeCamera.getOgreCamera()->getParentSceneNode();

    if (camNode == 0) {
      std::cerr << "mCamera isn't attached to any SceneNode !" << std::endl;
    }

    camNode->yaw(Ogre::Degree(mouse.getXAxisRelative() * MoveFactor * -0.1));
    camNode->pitch(Ogre::Degree(mouse.getYAxisRelative() * MoveFactor * -0.1));
  }

  // Zoom with the middle button...
  if (mouse.isButtonPressed(2)) 
  {
    m_activeCamera.getOgreCamera()->moveRelative(Vector3(0.0, 0.0, -0.5) * mouse.getYAxisRelative() * MoveFactor);
  }
  // ... and the wheel ;-)
  m_activeCamera.getOgreCamera()->moveRelative(Vector3(0.0, 0.0, -0.1) * mouse.getZAxisRelative() * MoveFactor);

  // Horizontal with the right button...
  if (mouse.isButtonPressed(1)) 
  {
    m_activeCamera.getOgreCamera()->moveRelative(Vector3(-0.5, 0.0, 0.0) * mouse.getXAxisRelative() * MoveFactor);
  }



  // Light rotation
  // animate the lights
  //for (size_t i = 0; i < NUM_LIGHTS; ++i)
  //  mLightPivots[i]->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(mLightSpeeds[i] / 100 ));

  //pLightSceneNode->rotate( Ogre::Vector3::UNIT_Y, Ogre::Degree(10) );
  // TODO: poner decente


  // Render scene
  m_pOgreRoot->renderOneFrame();

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
  const Ogre::RenderSystemCapabilities* caps = m_pOgreRoot->getRenderSystem()->getCapabilities();

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
  const Ogre::RenderSystemCapabilities* caps = m_pOgreRoot->getRenderSystem()->getCapabilities();

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
  const Ogre::RenderSystemCapabilities* caps = m_pOgreRoot->getRenderSystem()->getCapabilities();

  // Check capabilities programs
  if (  caps->hasCapability( Ogre::RSC_VERTEX_PROGRAM ) && 
        ( caps->hasCapability( Ogre::RSC_FRAGMENT_PROGRAM ) || caps->hasCapability( Ogre::RSC_DOT3 ) ) )
    return true;

  // Not supported
  return false;
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
 * @internal
 * 
 */
void GraphicsManager::loadAssets()
{
  // TODO correct paths
  // Load resource paths from config file
  Ogre::ConfigFile  cf;
  std::string       resourcePath;

  resourcePath = "..\\..\\..\\vision\\data\\";
  cf.load(resourcePath + "resources.cfg");

  // Go through all sections & settings in the file
  Ogre::String secName, typeName, archName;
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
  while (seci.hasMoreElements())
  {
    // Get section name and data
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

    // Iterate through section elements
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;

      // Add the resource location to the manager
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath + archName, typeName, secName);
    }
  }

  // Initialize, parse scripts etc
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GraphicsManager::keyPressed( const OIS::KeyEvent &event )
{
  // FW
  if ( event.key == OIS::KC_W )
  {
    m_activeCamera.moveRelative( Vector3( 0, 0, -10 ) );
  }

}


} // namespace Graphics