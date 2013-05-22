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

#ifndef _Window_H_
#define _Window_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"
#include "graphics/Color.h"
// OGRE
#include "OgreRenderTarget.h"

namespace Cing
{

/**
 * @internal
 * Represents a rendering window with an associated camera.
 * It is also a wrapper around the Ogre window class.
 */
class Window
{
public:

  // Public structures
  /**
   * @internal
   * Contains the information of the window metrics
   */
  struct TWindowMetrics
  {
    unsigned int  width;
    unsigned int  height;
    unsigned int  colourDepth;
    int           left;
    int           top;

  };

	// Constructor / Destructor
	 Window                   ();
	~Window                   ();

	// Init / Release / Update
	bool                                  	init                  	( Ogre::RenderWindow* pOgreWindow );
	void                                  	end                   	();
	void                                  	update                	();

	// Query methods    
	bool                                  	isValid               	() const { return m_bIsValid; }
	bool                                  	isClosed              	() const;
	bool                                  	isFullScreen          	() const;
	bool									isActive				() const;
	int										getWidth				() const { return m_width; }
	int										getHeight				() const { return m_height; }
	void                                  	getMetrics            	( TWindowMetrics& metrics ) const;
	size_t                                	getWindowHandle       	() const;
	const Ogre::RenderTarget::FrameStats& 	getFrameStats         	() const { return *m_stats; }
	Ogre::RenderWindow*					  	getOgreWindow			() { return m_pOgreWindow; }
	const Ogre::ColourValue&			  	getBackgroundColor		();
	Ogre::Viewport*							getMainViewport			() { return m_mainViewport; }						
	Ogre::Viewport*							getViewport				( int index = 0 );
	
	// Various
	void                                  	attachCameraToWindow  ( Camera3D& camera, int viewportIndex = 0 );
	void                                  	attachCameraToWindow  ( Ogre::Camera* ogreCamera, int viewportIndex = 0);
	void                                  	setBackgroundColor    ( const Color& color );

private:

	// Attributes
	Ogre::RenderWindow*	                  	m_pOgreWindow;  		///< Ogre window
	Ogre::Viewport*                       	m_mainViewport; 		///< Window viewport
	const Ogre::RenderTarget::FrameStats* 	m_stats;        		///< Window render statistics
	int										m_width, m_height;		///< Window size
	bool                                  	m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _Window_H_
