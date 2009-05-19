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

#ifndef _Window_H_
#define _Window_H_

#include "GraphicsPrereqs.h"
#include "graphics/Color.h"
// OGRE
#include "externLibs/Ogre3d/include/OgreRenderTarget.h"

namespace Graphics
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
	bool                                  init                  ( Ogre::RenderWindow* pOgreWindow );
	void                                  end                   ();
	void                                  update                ();

	// Query methods    
	bool                                  isValid               () const { return m_bIsValid; }
	bool                                  isClosed              () const;
	bool                                  isFullScreen          () const;
	int																		getWidth							() const { return m_width; }
	int																		getHeight							() const { return m_height; }
	void                                  getMetrics            ( TWindowMetrics& metrics ) const;
	size_t                                getWindowHandle       () const;
	const Ogre::RenderTarget::FrameStats& getFrameStats         () const { return *m_stats; }
	Ogre::RenderWindow*										getOgreWindow					() { return m_pOgreWindow; }
	const Ogre::ColourValue&							getBackgroundColor		();

	// Various
	void                                  attachCameraToWindow  ( Camera3D& camera );
	void                                  setBackgroundColor    ( const Color& color );

private:

	// Attributes
	Ogre::RenderWindow*	                  m_pOgreWindow;  		///< Ogre window
	Ogre::Viewport*                       m_mainViewport; 		///< Window viewport
	const Ogre::RenderTarget::FrameStats* m_stats;        		///< Window render statistics
	int																		m_width, m_height;	///< Window size
	bool                                  m_bIsValid;					///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _Window_H_
