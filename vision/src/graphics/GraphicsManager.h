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

#ifndef _GraphicsManager_H_
#define _GraphicsManager_H_

#include "GraphicsPrereqs.h"
#include "Window.h"
#include "Camera3D.h"
#include "DebugOverlay.h"
#include "Font.h"
#include "Text.h"
#include "CameraController.h"

#include "DynamicLines.h"

// OpenCv includes
#include "externLibs/OpenCV/cxcore/include/cxtypes.h"
#include "externLibs/OpenCV/highgui/include/highgui.h"

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
	Window&										getMainWindow             ()			 { return m_mainWindow; }
	Camera3D&									getActiveCamera           ()			 { return m_activeCamera; }
	const Ogre::SceneManager& getSceneManager           () const { return *m_pSceneManager; }
	Ogre::SceneManager&       getSceneManager           ()       { return *m_pSceneManager; }
	Ogre::SceneManager*       getSceneManagerPtr        ()       { return m_pSceneManager; }
	CameraController&					getDefaultCameraController()			 { return m_defaultCamController; }	

	// Common capabilities checking
	bool                      hasVertexProgramsSupport  () const;
	bool                      hasFragmentProgramsSupport() const;
	bool                      hasBumpMappingSupport     () const;

	// Rendering related methods
	void											setRenderMode							( RenderMode mode );

	// Color related methods
	void											setFillColor							(  const Color& color );

	void											setStrokeColor						(  const Color& color );
	const Color&							getFillColor					  	() const { return m_fillColor; }
	const Color&							getStrokeColor						() const { return m_strokeColor; }
	void											setBackgroundColor ( const Color& color );

	// Appearance drawing methods
	void 											setStrokeWeight						(  int weight );
	int  											getStrokeWeight						() { return m_strokeWeight; }

	void 											noFill										() { m_fill   = false; }
	bool											getFill										() const { return m_fill; }
	void 											noStroke									() { m_stroke = false; }
	bool											getStroke									() const { return m_stroke; }
	void 											smooth									  () { m_smooth = true; }
	void 											noSmooth									() { m_smooth = false; }
	bool											getSmooth 								() const { return m_smooth; }

	const RectMode&						getRectMode								() const { return m_rectMode; }
	void             					setRectMode								(  const  RectMode&	mode );

	const CvFont&							getCvFont									() const { return  m_cvFont; }

	// Debug methods
	void											showFps										( bool show );

	// Camera control
	void											useDefault3DCameraControl	( bool useDefault );

	// Drawing images control (Temp)
	void											addDrawableImage					( TexturedQuad* img );
	void											removeDrawableImage				( TexturedQuad* img );

  // Drawing 3d lines (Temp)
	void addVertex( Common::Vector newPos );
  
	// Drawing 2d primitives
	void line			( float x1, float y1, float x2, float y2);
	void point		( float x1, float y1 );
  void triangle	( float x1, float y1, float x2, float y2, float x3, float y3 );
  void rect			( float x1, float y1, float x2, float y2 );
	void quad			( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
	void ellipse	( float x, float y, float width, float height);
	void text			( float x1, float y1, const char* text );

	// 2D Canvas
	Graphics::Image*			m_canvas;

private:

	// private constructor to ensure singleton
	GraphicsManager      ();

	//TODO: poner decente
	Ogre::Light*					pLight;
	Ogre::SceneNode*			pLightSceneNode;

	// Attributes

	// Camera controller
	CameraController			m_defaultCamController;

	// Core
	Ogre::SceneManager*   m_pSceneManager;  ///< Main scene manager
	Window                m_mainWindow;     ///< Main application window
	Camera3D              m_activeCamera;   ///< Active camera
	
	// Misc
	DebugOverlay          m_debugOverlay;   ///< Debug overlay used to show debug information
	Font                  m_defaultFont;    ///< Default system font
	Text                  m_defaultText;    ///< To print text to screen


	// Simple primitives 3d related drawing ( lines, circles , ...)
	DynamicLines*								m_lines;
	Ogre::SceneNode*						m_linesNode;
	std::vector <Ogre::Vector3> m_linesPoints;

	Color									m_fillColor;			///< Color used to fill shapes
	Color									m_strokeColor;			///< Color used to draw shapes
	int										m_strokeWeight;			///< Width of the stroke used for draw lines, points, and the border around shapes
	Graphics::RectMode		m_rectMode;
	bool									m_fill;
	bool									m_stroke;
	bool									m_smooth;

	CvFont								m_cvFont;					///< Font used to draw text on images

	// To manage visibility of loaded images
	// TODO optimize this
	std::list< TexturedQuad* >	m_drawableImagesQueue; ///< Images that are being drawn by the user ar maked as not visible every frame
																							// to if the user does not call the draw one frame the image is not drawn

	bool									m_showFps;				///< Indicates whether the frames per second should be shown or not
	bool                  m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	
};

} // namespace Graphics

#endif // _GraphicsManager_H_
