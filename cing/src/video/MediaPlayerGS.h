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

#ifndef _MediaPlayerGS_H_
#define _MediaPlayerGS_H_

// Precompiled headers
#include "Cing-Precompiled.h"

// Graphics
#include "graphics/Image.h"

// GStreamer
#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>

// Common
#include "PTypes/include/pasync.h"

namespace Cing
{
	/**
	* Media player that uses GStreamer to decode and play video/audio
	*/
	class MediaPlayerGS
	{
	public:

		// Constructor / Destructor
		MediaPlayerGS();
		MediaPlayerGS( const char* filename, float fps = -1 );
		~MediaPlayerGS();

		// Init / Release / Update
		bool    init    ();
		bool    load    ( const char* fileName, GraphicsType requestedVideoFormat = RGB, float fps = -1 );
		void    end     ();
		void    update  ();
		Image&  getImage();

		// Query methods
		bool    isValid   () const { return m_bIsValid; }
		bool    isPlaying ();
		bool	isPaused  		();
		float   duration  () const { return (float)m_videoDuration; }
		float   time      ();
		int     getWidth  () const { return m_videoWidth; }
		int     getHeight () const { return m_videoHeight; }
		float   fps		  		() const { return m_videoFps; }
		float   frameRate 		() const { return m_videoFps; }
		int		numberOfFrames	() const { return m_nFrames; } 

		// Media control
		void    play    ();
		void    loop    ();
		void    stop    ();
		void    pause   ();
		void    setLoop ( bool loop ) { m_loop = loop; }
		void    noLoop  () { setLoop( false  ); }
		void    jump		( float whereInSecs );		///< Note: For accurate frame level positioning, videos should have keyframes in all frames.
		void    jumpToFrame	( unsigned int frameNumber );
		void    speed   ( float rate );

		// Audio Specific Control
		void	toggleMute();
		void	setVolume ( float volume );
		float	getVolume () const;

		// Setters / Handlers
		void	onNewBuffer		( GstBuffer* newBuffer );
		void	onEndOfStream	();

		// GStreamer specific query methods. Can be used to access advanced GStreamer features
		GstElement*	getPlayBin() { return m_pipeline; }

	private:
		// Internal methods
		bool			buildPathToFile			( const String& path );
		bool			createPipeline			();
		void			setPipelineState		( GstState state );
		bool			createVideoSink			();
		bool			configureVideoFormat	( GraphicsType requestedVideoFormat );
		void			copyBufferIntoImage		();
		void			flushBusMsg				();
		const char*		cingToGstPixelFormat	( GraphicsType cingPixelFormat );
		GraphicsType	gstToCingPixelFormat	( const String& gstVideoFormat );
		bool			checkVideoFormatMatch	( GstVideoFormat gstVideoFormat, GraphicsType cingVideoFormat );

		// GStreamer stuff
		GstElement*				m_pipeline;	
		GstElement*				m_appSink;
		GstElement*				m_appBin;

		// Media info
		String					m_fileName;			///< Name of the loaded video
		String					m_filePath;			///< Full path of the loaded video
		double         			m_videoDuration;	///< Duration in seconds
		float                 	m_videoFps;			///< Frames per second of the loaded video
		int						m_nFrames;			///< Total number of frames of the video
		int                   	m_videoWidth;		///< Width in pixels of the loaded video
		int                   	m_videoHeight;		///< Height in pixels of the loaded video

		// Playback Settings/Info
		bool                  	m_loop;				///< If true, the video will loop
		bool					m_loopPending;		///< Loop pending 
		bool					m_endOfFileReached; ///< True when the playback head reached the end of the file
		bool					m_playing;			///< True if the stream is playing (the user called play() or loop() not necessarily that the pipeline is already in playing state)
		bool					m_paused;			///< True if the stream is paused (tue user called pause(), not that the pipeline state is paused)

		// Audio settings	
		bool					m_mute;				///< True if volume is mutted (0)
		float					m_volume;			///< Current audio volume (0..1)

		// Buffer Stuff
		pt::mutex				m_bufferMutex;		///< Mutex to ensure threading safe buffer copy (from gstreamer to our internal buffer)
		unsigned char*			m_internalBuffer;	///< Internal buffer to store the buffer comming from gstreamer
		int						m_bufferSizeInBytes;///< Size of the buffer in bytes (w*h*nChannels)
		String					m_outputGstVideoFormat;///< GStreamer output buffer video format
		GraphicsType			m_pixelFormat;		///< Pixel format in which new image frames will be stored
		Image					m_frameImg;			///< Image containing the buffer of the current video frame
		bool					m_newBufferReady;	///< True when there is a new buffer ready

		// Internal stuff
		bool                  	m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace

#endif // _MediaPlayerGS_H_
