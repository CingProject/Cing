/*
This source file is part of the Cing project
For the latest info, see http://www.cing.cc

Copyright (c) 2008-2010 Julio Obelleiro and Jorge Cano

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

#ifndef _MediaPlayerOCV_H_
#define _MediaPlayerOCV_H_

// Precompiled headers
#include "Cing-Precompiled.h"

// Graphics
#include "graphics/Image.h"

// Ogre
#include "OgreTimer.h"

// OpenCV
#include "opencv2/highgui/highgui.hpp"

// Common
#include "PTypes/include/pasync.h"

namespace Cing
{
	/**
	* Media player that uses GStreamer to decode and play video/audio
	*/
	class MediaPlayerOCV
	{
	public:

		// Constructor / Destructor
		MediaPlayerOCV();
		MediaPlayerOCV( const char* filename, float fps = -1 );
		~MediaPlayerOCV();

		// Init / Release / Update
		bool    init    ();
		bool    load    ( const char* fileName, GraphicsType requestedVideoFormat = RGB, float fps = -1 );
		void    end     ();
		void    update  ();
		Image&  getImage();

		// Query methods
		bool    isValid   () const { return m_bIsValid; }
		bool    isPlaying ();
		float   duration  () const { return (float)m_videoDuration; }
		float   time      ();
		int     getWidth  () const { return m_videoWidth; }
		int     getHeight () const { return m_videoHeight; }

		// Media control
		void    play    ();
		void    loop    ();
		void    stop    ();
		void    pause   ();
		void    setLoop ( bool loop ) { m_loop = loop; }
		void    noLoop  () { setLoop( false  ); }
		void    jump    ( float whereInSecs );
		void    speed   ( float rate );

		// Audio Specific Control
		void	toggleMute();
		void	setVolume ( float volume );
		float	getVolume () const;

	private:
		// Internal methods
		bool			buildPathToFile			( const String& path );
		void			copyBufferIntoImage		();

		// Open CV stuff
		cv::VideoCapture		m_capture;			///< OpenCV capture to read movie file
		
		// Media info
		String					m_fileName;			///< Name of the loaded video
		String					m_filePath;			///< Full path of the loaded video
		double         			m_videoDuration;	///< Duration in seconds
		float                 	m_videoFps;			///< Frames per second of the loaded video
		int						m_videoNFrames;		///< Total Number of frames
		int                   	m_videoWidth;		///< Width in pixels of the loaded video
		int                   	m_videoHeight;		///< Height in pixels of the loaded video

		// Playback Settings
		bool                  	m_loop;				///< If true, the video will loop
		bool					m_loopPending;		///< Loop pending 

		// Audio settings	
		bool					m_mute;				///< True if volume is mutted (0)
		float					m_volume;			///< Current audio volume (0..1)

		// Playback control
		Ogre::Timer				m_timer;			///< Used to control de playback
		int						m_currentFrame;		///< Current frame (last frame retrieved from capture)
		bool					m_playing;			///< True when the player is active (playing)
		
		// Buffer Stuff
		int						m_bufferSizeInBytes;///< Size of the buffer in bytes (w*h*nChannels)
		GraphicsType			m_pixelFormat;		///< Pixel format in which new image frames will be stored
		Image					m_frameImg;			///< Image containing the buffer of the current video frame
		bool					m_newBufferReady;	///< True when there is a new buffer ready

		// Internal stuff
		bool                  	m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace

#endif // _MediaPlayerOCV_H_
