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

// Precompiled headers
#include "Cing-Precompiled.h"

// Graphics includes
#include "GraphicsManager.h"
#include "Window.h"
#include "TexturedQuad.h"
#include "Image.h"
#include "ShapeManager.h"
#include "FontManager.h"
#include "GraphicsUserApi.h"
#include "RTTRectSaveManager.h"

// Framework
#include "framework/UserAppGlobals.h"
#include "framework/Application.h"

// Common includes
#include "common/Exception.h"
#include "common/Release.h"
#include "common/MathUtils.h"
#include "common/CommonConstants.h"
#include "common/ResourceManager.h"
#include "common/LogManager.h"
#include "common/SystemUtils.h"

// Ogre includes
#include "OgreRoot.h"
#include "OgreConfigFile.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"
#include "OgreStringConverter.h"
#include "OgreStringConverter.h"
#include "OgreHardwarePixelBuffer.h"
#include "Overlay/OgreOverlaySystem.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "RenderSystems/GL/OSX/OgreOSXCocoaView.h"
#endif

// Collada
#if defined( _MSC_VER ) // TODO, need OgreCollada working on os x!
#include "OgreCollada/include/OgreCollada.h"
#endif

namespace Cing
{
    
	/**
     * @internal
     * @brief Constructor. Initializes class attributes.
     */
	GraphicsManager::GraphicsManager():
    m_bIsValid		( false ),
    m_showFps		( false ),
    m_fpsAverage	( 50 ),
    m_pSceneManager( NULL ),
    m_fill( true ),
    m_stroke( true ),
    m_smooth( false ),
    m_imageMode( CORNER ),
    m_rectMode( CORNER ),
    m_ellipseMode( CENTER ),
    m_setupCalled( false ),
    m_defaultWindowWidth( 640 ),
    m_defaultWindowHeight( 480 ),
    m_defaultGraphicMode( OPENGL ),
    m_fullscreen( false ),
    m_vSync(true),
    m_fsaa(0),
    m_windowBorder(true),
    m_windowMonitorIndex(0),
    m_saveFrame(false),
	m_windowPosX(-INT_MAX),
	m_windowPosY(-INT_MAX),
    m_shadowsEnabled(false),
    m_showConfigDialog(false)
    {
        // Store the window in global var
        appWindow = &m_mainWindow;
        
        // Set default 2D renderer (user will overrride with a correct one)
        setRenderer2D( &m_baseRenderer2D );
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
     * @brief Inits the graphics driver (OpenGL or DirectX) and creates the application window
     * @param view   The view that should be used instead of creating a new window (an OgreView, currently only used in OS X)
     * @return true if the initialization was ok | false otherwise
     */
    bool GraphicsManager::createWindow( void* view )
    {
        // Check if the class is already initialized
        if ( isValid() )
            return true;
        
        // Get reference to Ogre Root
        Ogre::Root& ogreRoot = Ogre::Root::getSingleton();
        
        // Setup default values if user didn't call setup
        setup( m_defaultWindowWidth, m_defaultWindowHeight );
        
        // Init Ogre
        ogreRoot.initialise(false, appName);
        
        // Create the app window (First define parameters)
        Ogre::NameValuePairList windowParams;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        if ( view != NULL )
        {
            OgreView *ogreView = (OgreView *)view;
            windowParams["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)ogreView);
            windowParams["macAPI"] = "cocoa";
            windowParams["macAPICocoaUseNSView"] = "cocoa";
        }
#endif
        
        windowParams["title"] = appName;
        windowParams["border"] = m_windowBorder? "fixed": "none";
        windowParams["monitorIndex"] = toString(m_windowMonitorIndex);
        windowParams["colourDepth"] = toString(32); // only applied if on fullscreen
		windowParams["left"] = m_windowPosX != -INT_MAX? toString(m_windowPosX): "0";
		windowParams["top"] = m_windowPosY != -INT_MAX? toString(m_windowPosY): "0";       
		windowParams["depthBuffer"] = "true";
        windowParams["FSAA"] = toString(m_fsaa);
        windowParams["displayFrequency"] = toString(60);
        windowParams["vsync"] = toString(m_vSync);
        
        // NOTE: externalWindowHandle allows to pass an external window handle to embed the rendering window inside id.
        // BUT! in OSX if passed empty... the window is not created (at least in Ogre 1.9RC1)
        //windowParams["externalWindowHandle"] = toString(0);
        
        // Then create it!
        Ogre::RenderWindow *ogreWindow = ogreRoot.createRenderWindow(appName, width, height, m_fullscreen, &windowParams) ;
        if ( !ogreWindow )
        {
            LOG_ERROR( "Error creating application window" );
            return false;
        }
        
        // Create main window
        m_mainWindow.init( ogreWindow );
        
        // Set global window size variables (in case they changed with the window creation for some reason
        width	= m_mainWindow.getWidth();
        height	= m_mainWindow.getHeight();
        
        // Create the scene manager
        m_pSceneManager = ogreRoot.createSceneManager( Ogre::ST_GENERIC );
        
        // Set the global pointer to the scene manager
        ogreSceneManager	= m_pSceneManager;
        
        // Init the main camera
		m_activeCamera.init( m_mainWindow.getViewport(), m_pSceneManager );
        
        // Set the global pointer to the camera
        ogreCamera	= m_activeCamera.getOgreCamera();
        
        // Make the camera render in the main window
        m_mainWindow.attachCameraToWindow( m_activeCamera.getOgreCamera() );
        
        // Background color
        m_mainWindow.setBackgroundColor( Color( 200, 200, 200 ) );
        
        
        // By default full ambient light
        GraphicsManager::getSingleton().getSceneManager().setAmbientLight( Color(255).normalized() );
        
        return true;
    }
    
    /**
     * @brief Inits the application graphic related resources.
     * @note createWindow() should be called before initReSources. After initReSources call is done, the GraphicsManager is
     * considered to be valid and correctly initialize if there were no errors.
     * @return true if the initialization was ok | false otherwise
     */
    bool GraphicsManager::initReSources()
    {
        // First check that we have a valid ogreSceneManager (which means that createWindow() has been called
        if ( ogreSceneManager == NULL )
        {
            LOG_ERROR( "Error initializing GraphicsManager. The method createWindow() should be called before calling initReSources()" );
            return false;
        }
        
        // Init ShapeManager
        ShapeManager::getSingleton().init();
        
        // Init the debug overlay
        // TODO
        //m_debugOverlay.init();
        
        // Not Use default camera controller (the user would need to activate it)
        enableDefault3DCameraControl( false );
        
        // Init all registered resource groups in Ogre
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        
        // Init the new Ogre Overlay system (necessary to use fonts or overlays since Ogre 1.9)
        m_overlaySystem = new Ogre::OverlaySystem();
        ogreSceneManager->addRenderQueueListener(m_overlaySystem);
        
        // Init the Font Manager
        FontManager::getSingleton().init();
        
        // Init the cvFont
        //cvInitFont(&m_cvFont, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6, 0, 2);
        
        // Init 2dCanvas (and make sure it does not cast shadows and does not receive dynamic lighting)
        m_canvas.init(width, height, RGBA);
        m_canvas.getTexturedQuad().enableCastShadows( false );
        m_canvas.getTexturedQuad().enableLighting( false );
        
        // Init style queue
        m_styles.push_front( Style(Color(255,255,255), Color(0,0,0), 1) );
        
        // Init transform stack
        m_transforms.push( Transform() );
        m_shapesTransforms.push( Transform() );
        
        // Init pixels
        for (int i = 0; i < m_canvas.getWidth() * m_canvas.getHeight(); i++)
            pixels.push_back( Color( 200, 200, 200 ) );
        
        // Set image background color and render queue
        m_canvas.fill(Color(200));
        m_canvas.forceRenderQueue( Ogre::RENDER_QUEUE_BACKGROUND );
        
        
        // Init RTT texture and setup viewport
        m_RttTexture = Ogre::TextureManager::getSingleton().createManual("RttTexSaveImage", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, m_mainWindow.getWidth(), m_mainWindow.getHeight(), 0, Ogre::PF_BYTE_RGBA, Ogre::TU_RENDERTARGET);
        Ogre::RenderTarget* rttTex	= m_RttTexture->getBuffer()->getRenderTarget();
        rttTex->setAutoUpdated(false);
        Ogre::Viewport* vp	= rttTex->addViewport( m_activeCamera.getOgreCamera() );
        vp->setOverlaysEnabled(true);
        
        // Set default coordinate system:
        m_coordSystem = OPENGL3D;
        
        // This is to adjust 2d and 3d coordinates like in Processing (which is Cing's coordinate system by default)
        applyCoordinateSystemTransform(PROCESSING);
        
        // The class is now initialized
        m_bIsValid = true;
        
        // Reset fps timer
        m_fpsTimer.reset();
        
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
        
        // Release camera stuff
        //m_defaultCamController.end();
        m_activeCamera.end();
        
        // Release canvas
        m_canvas.end();
        
        // Release the Font Manager
        FontManager::getSingleton().end();
		FontManager::destroySingleton();

        // Overlay system
        delete m_overlaySystem;
        
        // Release scene manager
        Ogre::Root::getSingleton().destroySceneManager( m_pSceneManager );
        m_pSceneManager = NULL;
                
        // Release the Shape Manager
        ShapeManager::getSingleton().end();
        
        // The class is not valid anymore
        m_bIsValid			= false;
        m_shadowsEnabled	= false;
    }
    
    /**
     * @internal
     * @brief Renders everything in the scene
     */
    void GraphicsManager::draw()
    {
        // Reset the "global" matrix stack
        clearMatrixStack();
        
        // Set canvas to be drawn
        m_canvas.drawBackground( 0, 0, (float)width, (float)height);
        
        // Update 3d primitive drawing	( shape, lines,...)
        ShapeManager::getSingleton().update();
        
        // Update default camera controller
        //m_defaultCamController.update();
        
        // Get Frame stats (and display fps if enabled)
        const Ogre::RenderTarget::FrameStats& frameStats = m_mainWindow.getFrameStats();
        frameRate = frameStats.avgFPS;
        
        // Calculate fps (on mac frame stats work very inconsistent)
        double fps = 1000.0f / ((double)m_fpsTimer.getMicroseconds() / 1000.0);
        m_fpsAverage.addValue( fps );
        m_fpsTimer.reset();
        
        // Show fps
        if ( m_showFps )
        {
            std::ostringstream oss;
            //oss << "FPS: " << frameStats.lastFPS;
            oss << "FPS: " << m_fpsAverage.getValue();
            
            pushMatrix();
            resetMatrix();
            pushStyle();
            stroke(0);
            fill(255, 0, 0);
            text( oss.str(), 10, 0 );
            popStyle();
            popMatrix();
        }
        
        
        // Render scene
        Ogre::Root::getSingleton().renderOneFrame();
        
        // Update window
        m_mainWindow.update();
        
        // Render the viewport to texture and save to disk if required
        if ( m_saveFrame || !m_rectSaveList.empty() )
        {
            m_RttTexture->getBuffer()->getRenderTarget()->update();
            
            if ( m_saveFrame )
            {
            	if ( isPathAbsolute( m_frameName ) == false  )
					m_frameName = ResourceManager::userDataPath + m_frameName;
		    m_RttTexture->getBuffer()->getRenderTarget()->writeContentsToFile(m_frameName);
                m_saveFrame = false;
            }
            
            for ( std::vector<TNameRect>::const_iterator it = m_rectSaveList.begin(); it != m_rectSaveList.end(); ++it )
            {
            const Rect& rect = it->rect;
                Ogre::PixelFormat pf = m_RttTexture->getBuffer()->getRenderTarget()->suggestPixelFormat();
                size_t w = m_RttTexture->getWidth();
                size_t h = m_RttTexture->getHeight();
                void* buffer = malloc( Ogre::PixelUtil::getMemorySize( w, h, 1, pf ) );
                
            Ogre::PixelBox* pbox = new Ogre::PixelBox( Ogre::Box( 0, 0, w, h ), pf, buffer );
                m_RttTexture->getBuffer()->getRenderTarget()->copyContentsToMemory( *pbox, Ogre::RenderTarget::FB_AUTO );

			std::string path = it->name;
			if ( isPathAbsolute( it->name ) == false  )
				path = ResourceManager::userDataPath + it->name;

			RTTRectSaveManager::getSingleton().storePicture( path, rect, pbox, it->outputWidth, it->outputHeight );
            }
            m_rectSaveList.clear();
        }
        
        
        // Update the Font Manager post render (will make fonts not in use any more invisible)
        FontManager::getSingleton().postRender();
        
        // Mark all drawable images as not visible
        std::list< TexturedQuad* >::iterator it = m_drawableImagesQueue.begin();
        for (; it != m_drawableImagesQueue.end(); ++it )
            (*it)->setVisible( false );
    }
    
    /**
     * @brief Configures the render system and window for the application.
     * @note It should be called first to any other graphics related function
     * @param windowWidth   width of the application's window
     * @param windowHeight  height of the application's window
     * @param mode          specifies the render driver to use. Default OPENGL
     */
    void GraphicsManager::setup( int windowWidth, int windowHeight, GraphicMode mode )
    {
        // Check if setup has already been called
        if ( m_setupCalled )
            return;
        
        // Store width and height in globals
        width	= windowWidth;
        height	= windowHeight;
        
        // Get ogre root to configure it
        Ogre::Root& ogreRoot = Ogre::Root::getSingleton();
        
        // Show config dialog?
        if ( m_showConfigDialog && ogreRoot.showConfigDialog() )
        {
            m_setupCalled = true;
            return;
        }
        
        // Config dialog was not shown or user hit cancel... setup it up here then...
        
        // Name of the chosen render system
        std::string rendererName = "NO_NAME";
        if ( mode == OPENGL )
            rendererName = "OpenGL Rendering Subsystem";
        else if ( mode == DIRECTX )
            rendererName = "Direct3D9 Rendering Subsystem";
        else
            THROW_EXCEPTION( "Critital Error: Graphics Mode in size() call does not exist" );
        
        // Get list of render systems
        const Ogre::RenderSystemList&	availableRenderers = ogreRoot.getAvailableRenderers();
        Ogre::RenderSystem      *selectedRenderSystem = NULL;
        if ( availableRenderers.size() == 0 )
            THROW_EXCEPTION( "Critical Error: No available render systems. Please re-install the application" );
        
        // Select the render system
        for ( size_t i = 0; i < availableRenderers.size(); ++i )
        {
            if ( availableRenderers.at( i )->getName().compare( rendererName ) == 0 )
            {
                selectedRenderSystem = availableRenderers.at( i );
                break;
            }
        }
        
        // Set render system to ogre
        ogreRoot.setRenderSystem( selectedRenderSystem );
        
        // Configure rest of the settings depending on the rendering system selected
        if ( mode == OPENGL )
        {
            // Get list of options for this render system
            // TODO: This could be used to create a custom settings setup to make it easy to configur for users
            Ogre::ConfigOptionMap opts;
            opts = selectedRenderSystem->getConfigOptions();
            Ogre::ConfigOptionMap::iterator pOpt = opts.begin();
            String strLine;
            while( pOpt!= opts.end() )
            {
                strLine = pOpt->second.name + ": " + pOpt->second.currentValue;
                ++pOpt;
            }
            
            // Generate the video mode string
            std::ostringstream videoMode;
            videoMode << windowWidth << " x " << windowHeight;;
            
            // Set render system settings
            selectedRenderSystem->setConfigOption("Full Screen", m_fullscreen? "Yes": "No" );
            selectedRenderSystem->setConfigOption("Video Mode", videoMode.str().c_str() );
            
            // Set render system settings specified by user
            // TODO: Make all options available to user
            // RTT possible values: FBO, PBuffer, Copy
            selectedRenderSystem->setConfigOption( "RTT Preferred Mode", "FBO" );
            selectedRenderSystem->setConfigOption( "Colour Depth", "32" );
            selectedRenderSystem->setConfigOption( "VSync",m_vSync? "Yes": "No" );
            selectedRenderSystem->setConfigOption( "FSAA", intToString(m_fsaa) );
            
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            selectedRenderSystem->setConfigOption( "Display Frequency", "60" );
            selectedRenderSystem->setConfigOption( "vsyncInterval", "1" );
#endif
            
        }
        else if ( mode == DIRECTX )
        {
            // Generate the video mode string
            std::ostringstream videoMode;
            videoMode << windowWidth << " x " << windowHeight << " @ 32-bit colour";
            
            if ( selectedRenderSystem )
            {
                // Set render system settings specified by user
                selectedRenderSystem->setConfigOption("Full Screen", m_fullscreen? "Yes": "No" );
                selectedRenderSystem->setConfigOption("Video Mode", videoMode.str().c_str() );
                
                // Rest options with default values
                selectedRenderSystem->setConfigOption("VSync", m_vSync? "Yes": "No" );
                selectedRenderSystem->setConfigOption( "Floating-point mode","Consistent" );
                selectedRenderSystem->setConfigOption( "FSAA", intToString(m_fsaa) );
            }
        }
        
        // Validate render system options
        String optionsStatus = selectedRenderSystem == NULL? "": selectedRenderSystem->validateConfigOptions();
        if ( optionsStatus != "" )
        {
            LOG_ERROR( "Invalid Render System Options. Error: %s", optionsStatus.toChar() );
        }
        
        // Set up done
        m_setupCalled = true;
    }
    
    /**
     * @internal
     * @brief   Reset style stack
     */
    void GraphicsManager::clearStyleStack()
    {
        m_styles.clear();
        m_styles.push_front(Style(Color(255,255,255), Color(0,0,0), 1));
    };
    
    /**
     * @internal
     * @brief   Reset matrix stack (Clear and add one identity transform)
     */
    void GraphicsManager::clearMatrixStack()
    {
        while ( !m_transforms.empty() )
        {
            m_transforms.pop();
            m_shapesTransforms.pop();
        }
        m_transforms.push(Transform());
        m_shapesTransforms.push(Transform());
    };
    
    /**
     * @brief   Saves an image with the current frame on screen. The image is saved in the data folder
     * @param   name name of the image to be saved/exported
     */
    void GraphicsManager::saveFrame( const String& name )
    {
        m_saveFrame = true;
        m_frameName = name;
    };
    
	/**
 	 * @brief   Saves an image with a rectangle taken from the current frame on screen. The image is saved in the data folder
 	 * @param   name name of the image to be saved/exported
 	 * @param   rect rectangle to be saved
 	*/
	void GraphicsManager::saveFrame( const String& name, const Rect& rect )
	{
		TNameRect newScreenCapture;
		newScreenCapture.name = name;
		newScreenCapture.rect = rect;
		newScreenCapture.outputWidth = width;
		newScreenCapture.outputHeight = height;
    	m_rectSaveList.push_back( newScreenCapture );
	};

	/**
 	 * @brief   Saves an image with a rectangle taken from the current frame on screen, and allows to define the of the saved image. The image is saved in the data folder
 	 * @param   name name of the image to be saved/exported
 	 * @param   rect rectangle to be saved
 	 * @param   imageWidth width of the saved image file
 	 * @param   imageHeight height of the saved image file
 	 */
	void GraphicsManager::saveFrame( const String& name, const Rect& rect, int imageWidth, int imageHeight )
	{
		TNameRect newScreenCapture;
		newScreenCapture.name = name;
		newScreenCapture.rect = rect;
		newScreenCapture.outputWidth = imageWidth;
		newScreenCapture.outputHeight = imageHeight;
   	 	m_rectSaveList.push_back( newScreenCapture );
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
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        m_activeCamera.getOgreCamera()->setPolygonMode( (Ogre::PolygonMode) mode );
    }
    
    /**
     * @brief Sets the fill color used to draw all shapes rendered after this call.
     *
     * @param Color color to use to fill shapes
     */
    void GraphicsManager::setFillColor( const Color& color )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        m_styles.front().m_fillColor = color;
        
        // We are using the emissive color to fake the fill color with lighting activated
        // TODO fix this
        //m_pSceneManager->setAmbientLight( m_styles.front().m_fillColor );
        
        m_fill = true;
    }
    
    /**
     * @brief Sets the border color used to draw shapes after this call.
     *
     * @param Color color to use to draw the border of shapes
     */
    void GraphicsManager::setStrokeColor( const Color& color )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
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
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
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
    void GraphicsManager::enableDefault3DCameraControl( bool useDefault )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        // Enable controller
        //if ( useDefault )
        //	m_defaultCamController.init( m_activeCamera );
        //// Enable controller
        //else
        //	m_defaultCamController.end();
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
        // Security check: if the queue is empty, do nothing
        if ( m_drawableImagesQueue.empty() )
            return;
        
        std::list< TexturedQuad* >::iterator it = m_drawableImagesQueue.begin();
        for (; it != m_drawableImagesQueue.end(); ++it )
            if ( *it == img )
            {
                m_drawableImagesQueue.erase( it );
                return;
            }
    }
    
    /**
     * @brief Modifies the location from which images draw
     * Valid values are: CORNER, CORNERS, CENTER (CORNERS not implemented yet)
     */
    void GraphicsManager::setImageMode( int	mode )
    {
        // Check valid mode
        if ( (mode != CORNERS) && (mode != CORNER) && (mode != CENTER) )
        {
            LOG_ERROR( "setImageMode: mode %d not supported. Supported modes are CENTER, CORNER, or CORNERS", mode );
            return;
        }
        else if ( mode == CORNERS )
        {
            LOG_WARNING( "setImageMode. CORNERS Image Mode is not implemented yet. Defaulting to CORNER" );
            m_imageMode = CORNER;
        }
        else
            m_imageMode = mode;
    }
    
    
    /**
     * @brief Modifies the location from which rectangles draw
     * Valid values are: CORNER, CORNERS, CENTER
     */
    void GraphicsManager::setRectMode( int	mode )
    {
        // Check valid mode
        if ( (mode != CORNERS) && (mode != CORNER) && (mode != CENTER) )
        {
            LOG_ERROR( "setRectMode: mode %d not supported. Supported modes are CENTER, CORNER, or CORNERS", mode );
            return;
        }
        else
            m_rectMode = mode;
    }
    
    /**
     * @brief Modifies the location from which ellipses draw
     * Valid values are: CORNER, CORNERS, CENTER
     */
    void GraphicsManager::setEllipseMode( int	mode )
    {
        // Check valid mode
        if ( (mode != CORNERS) && (mode != CORNER) && (mode != CENTER) )
        {		
            LOG_ERROR( "setEllipseMode: mode %d not supported. Supported modes are CENTER, CORNER, or CORNERS", mode );
            return;
        }
        else
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
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        if ( !isValid() )
            return;
        
        m_mainWindow.getOgreWindow()->getViewport(0)->setBackgroundColour( color.normalized() );
        
        // Set the canvas color
        m_canvas.fill( color );
        
        // Make it visible for the next render
        m_canvas.drawBackground( 0, 0, (float)width, (float)height);
    }
    
    /**
     * @brief Apply coordinate system transforms to subsequent rendering and camera
     *
     * @param[in] coordSystem
     */
    void GraphicsManager::applyCoordinateSystemTransform( const GraphicsType& coordSystem )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        // Check if it's actually the same coordinate system we already had
        if ( coordSystem == m_coordSystem)
            return;
        
        // Set camera and scene manager setting accordingly
        m_coordSystem = coordSystem;
        switch(m_coordSystem)
        {
            case OPENGL3D:
            {
                // Reset camera position (and orientation?)
                m_activeCamera.getSceneNode()->setPosition( Ogre::Vector3( 0, 0, 2000.0 ) );
                // Reset applied simmetry to y-world axis
                m_pSceneManager->getRootSceneNode()->setScale(1,1,1);
            }
                break;
            case PROCESSING:
            {
                // Calculate new camera position 
                float cameraDistance    =  (height / 2.0f ) / tanf( (m_activeCamera.getOgreCamera()->getFOVy().valueRadians()) / 2.0f );
                
                Ogre::Vector3 camPos = Ogre::Vector3( width/2.0f, height/2.0f, cameraDistance );
                // Set the camera position
                
                m_activeCamera.getSceneNode()->setPosition( camPos );
                m_activeCamera.getSceneNode()->lookAt( Ogre::Vector3(width/2.0f, height/2.0f, 0.0f), Ogre::Node::TS_WORLD );
                
                // Apply simmetry to y-world axis
                m_pSceneManager->getRootSceneNode()->setScale(1,-1,1);
            }	
                break;
            default:
                break;
        }
    }
    
    /**
     * @brief Enables use of shadows. Should be called in the setup before initializing any 3d object.
     *
     * @param[in] technique Shadow technique to use
     */
    void GraphicsManager::enableShadows( ShadowTechnique technique )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        if ( !isValid() )
        {
            LOG_ERROR( "enableShadows error. setup() has not been called yet" );
            return;
        }
        
        m_pSceneManager->setShadowTechnique( (Ogre::ShadowTechnique)technique );
        m_shadowTechnique	= technique;
        m_shadowsEnabled	= true;
    }
    
    
    /**
     * @brief Sets the shadow color used to modulate areas in shaodw (when shadows that modulate are enabled)
     * @param color color of the shadow. Range 0..255
     */
    void GraphicsManager::setShadowColor( const Color& color )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        if ( !isValid() )
        {
            LOG_ERROR( "enableShadows error. setup() has not been called yet" );
            return;
        }
        
        m_pSceneManager->setShadowColour( color.normalized() );
    }
    
    /**
     * @brief Sets the far distance at which the shadows will still be rendered. Used to optimize performance.
     *
     * @param[in] distance Distance it world units. At this distance form the camera, shadows won't be rendered any more.
     */
    void GraphicsManager::setShadowFarDistance( float distance )
    {
        // Check application correctly initialized (could not be if the user didn't call size function)
        Application::getSingleton().checkSubsystemsInit();
        
        if ( !isValid() )
        {
            LOG_ERROR( "enableShadows error. setup() has not been called yet" );
            return;
        }
        
        m_pSceneManager->setShadowFarDistance( distance );
    }
    
    
} // namespace Cing
