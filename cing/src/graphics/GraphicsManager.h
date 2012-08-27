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

#ifndef _GraphicsManager_H_
#define _GraphicsManager_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"
#include "Window.h"
#include "Camera3D.h"
#include "DebugOverlay.h"
#include "TextArea.h"
#include "CameraController.h"
#include "OgreManualObject.h"
#include "Transform.h"
#include "Style.h"
#include "Shape.h"
#include "Image.h"

// OpenCv includes
#include "OpenCV/cxtypes.h"
#include "OpenCV/cxcore.h"
#include "OpenCV/highgui.h"

//TEMP
#include "input/InputPrereqs.h"

// Common
#include "common/Singleton.h"
#include <vector>
#include <stack>
#include "common/eString.h"
#include "common/MathUtils.h"

namespace Cing
{
	/**
	* @internal
	* Manages the communication with the Graphics engine (OGRE)
	*/
	class GraphicsManager: public SingletonStatic< GraphicsManager >
	{

	public:

		// Required for singleton
		friend class SingletonStatic< GraphicsManager >;

		// Destructor
 		virtual ~GraphicsManager								();

		// Life cycle (in order of "appearance")
		bool						createWindow				();
		bool						initReSources				();
		void						draw						();
		void						end							();

		// App window setup
		void						setup						( int windowWidth, int windowHeight, GraphicMode mode = OPENGL );
		void						fullscreen					()		{ m_fullscreen = true; }
		void						vSync						(bool value )		{ m_vSync = value; }
		void						FSAA						( int fsaa )		{ m_fsaa = fsaa; }
		void						windowBorder				( bool border )		{ m_windowBorder = border; }
		void						windowMonitorIndex			( unsigned int index ) { m_windowMonitorIndex = index; }

		// Query methods
		bool						isValid                   () const	{ return m_bIsValid; }
		const Window&				getMainWindow             () const	{ return m_mainWindow; }
		Window&						getMainWindow             ()		{ return m_mainWindow; }
		Camera3D&					getActiveCamera           ()		{ return m_activeCamera; }
		const Ogre::SceneManager&	getSceneManager           () const	{ return *m_pSceneManager; }
		Ogre::SceneManager&			getSceneManager           ()		{ return *m_pSceneManager; }
		Ogre::SceneManager*			getSceneManagerPtr			()       { return m_pSceneManager; }
		CameraController&			getDefaultCameraController	()		 { return m_defaultCamController; }

		// Common capabilities checking
		bool                      	hasVertexProgramsSupport	() const;
		bool                      	hasFragmentProgramsSupport	() const;
		bool                      	hasBumpMappingSupport		() const;

		// Rendering related methods
		void						setRenderMode				( RenderMode mode );

		// Color related methods
		void						setFillColor				(  const Color& color );
		void						setStrokeColor				(  const Color& color );
		const Color&				getFillColor				() const { return 	m_styles.front().m_fillColor; }
		const Color&				getStrokeColor				() const { return 	m_styles.front().m_strokeColor; }

		void						setBackgroundColor ( const Color& color );

		// Appearance drawing methods
		void 						setStrokeWeight				(  int weight );
		int  						getStrokeWeight				() { return m_styles.front().m_strokeWeight; }

		void 						noFill						() { m_fill   = false; }
		void 						noStroke					() { m_stroke = false; }
		void 						noSmooth					() { m_smooth = false; }

		bool						getFill						() const { return m_fill; }
		bool						getStroke					() const { return m_stroke; }
		bool						getSmooth 					() const { return m_smooth; }

		void 						smooth						() { m_smooth = true; }

		int							getImageMode				() const { return m_imageMode; }
		void             			setImageMode				(  int	mode );

		int							getRectMode					() const { return m_rectMode; }
		void             			setRectMode					(  int	mode );

		int							getEllipseMode				() const { return m_ellipseMode; }
		void             			setEllipseMode				(  int	mode );

		const CvFont&				getCvFont					() const { return  m_cvFont; }

		// Debug methods
		void						showFps						( bool show );

		// Camera control
		void						enableDefault3DCameraControl( bool value );

		// Drawing images control (Temp)
		void						addDrawableImage			( TexturedQuad* img );
		void						removeDrawableImage			( TexturedQuad* img );

		// Stacks
		void						clearStyleStack();
		void						clearMatrixStack();

		//Save frames
		void						saveFrame( const String& name );
        void                        saveFrame( const String& name, const Rect& box );

		// Import Collada
		//bool loadCollada( const String& fileName );
		// Coordinate systems
		bool						isProcessingMode				() { return ( m_coordSystem == PROCESSING );	}	
		void						applyCoordinateSystemTransform	( const GraphicsType& coordSystem );

		// Shawods
		void						enableShadows				( ShadowTechnique technique );
		void						setShadowColor				( const Color& color );
		void						setShadowFarDistance		( float distance ); 
		bool						shadowsEnabled				() const { return m_shadowsEnabled; }
		ShadowTechnique				getCurrentShadowTechnique	() const { return m_shadowTechnique; }

        Ogre::TexturePtr			getRttTexture               () { return m_RttTexture; }


		// Public Attributes (to be removed)

		// 2D Canvas
		Image						m_canvas;			///< Background image
		
		// 2D / 3D Transforms
		std::stack < Transform >	m_transforms;		///< Stack to store transform objects
		std::stack < Transform >	m_shapesTransforms;	///< Stack to store transform shapes. They have a different stack as their tranformations differ.
														///< This is because 3d objects and Images are centered in relation to its parent node, but shapes are not
														///< to be able to have 1 shared parent node for all shapes (performance reasons)
		std::deque < Style >		m_styles;			///< Deque to store style properties ( fill color, stroke weight, etc.)

	private:

		// private constructor to ensure singleton
		GraphicsManager      ();

		//TODO: Clean up
		Ogre::Light*				pLight;
		Ogre::SceneNode*			pLightSceneNode;

		// Attributes

		// Core
		Ogre::SceneManager*			m_pSceneManager;  ///< Main scene manager

		// Camera controller
		CameraController			m_defaultCamController;


		Window						m_mainWindow;     ///< Main application window
		Camera3D					m_activeCamera;   ///< Active camera

		// Graphics system setup
		int                        	m_defaultWindowWidth;
		int                        	m_defaultWindowHeight;
		GraphicMode                	m_defaultGraphicMode;
		bool                       	m_setupCalled;			///< True when the setup method has been already called
		bool                       	m_fullscreen;			///< If true, the applcation will run in full screen mode
		bool                       	m_vSync;				///< If true, the applcation sync with dispay render (usually 60fps)
		int							m_fsaa;					/// < Antialiasing value (0..16). Depends on HW support
		bool						m_windowBorder;			/// If true, the main window will have regular frame/border, if false, it won't have any
		unsigned int				m_windowMonitorIndex;	/// Monitor index for the main window (allows to set windows in secondary monitors)

		// Misc
		DebugOverlay				m_debugOverlay;			///< Debug overlay used to show debug information
		//MovableText				m_systemFont;			///< To print system info to screen

		// Styling properties
		// TODO: Eliminates this. Change styles queue to stack
		int							m_imageMode;			///< Parameters input mode to draw images
		int							m_rectMode;				///< Parameters input mode to draw rectangles
		int							m_ellipseMode;			///< Parameters input mode to draw ellipses
		bool						m_fill;
		bool						m_stroke;
		bool						m_smooth;

		CvFont						m_cvFont;					///< Font used to draw text on images

		// To manage visibility of loaded images
		// TODO optimize this
		std::list< TexturedQuad* >	m_drawableImagesQueue;	///< Images that are being drawn by the user ar maked as not visible every frame
															/// to if the user does not call the draw one frame the image is not drawn

		//to allow screen capture and effects
		Ogre::TexturePtr			m_RttTexture;
		bool						m_saveFrame;
        typedef std::pair< std::string, Rect> TNameRect;
        std::vector<TNameRect>      m_rectSaveList;
		String						m_frameName;

		// Shadows related
		ShadowTechnique				m_shadowTechnique;	///< Current Shadow Technique
		bool						m_shadowsEnabled;	///< True if any shadow technique has been enabled

	    GraphicsType				m_coordSystem;
		bool						m_showFps;			///< Indicates whether the frames per second should be shown or not
		Ogre::Timer					m_fpsTimer;
		AverageDouble				m_fpsAverage;
		bool						m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	};

} // namespace Cing

#endif // _GraphicsManager_H_
