/*
 This source file is part of the Cing project
 For the latest info, see http://www.cing.cc
 
 Copyright (c) 2008-2010 Julio Obelleiro and Jorge Cano
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the im_mediaPlayerlied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Tem_mediaPlayerle Place, Suite 330, Boston, MA  02111-1307  USA
 */

// Precompiled headers
#include "Cing-Precompiled.h"

#include "MediaPlayerOCV.h"

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/XMLElement.h"

// Framework
#include "framework/UserAppGlobals.h"


namespace Cing
{
	
	
	
	/**
	 * Default constructor.
	 */
	MediaPlayerOCV::MediaPlayerOCV():
	m_bufferSizeInBytes(0),
	m_videoWidth	( 0 ),
	m_videoHeight	( 0 ),
	m_videoFps		( 0 ),
	m_videoDuration	( 0 ),
	m_loop			( false ),
	m_loopPending	( false	),
	m_newBufferReady( false ),
	m_bIsValid		( false ),
	m_pixelFormat	( RGB	),
	m_mute			( false ),
	m_volume		( 1.0f  ),
	m_currentFrame	( 0 )
	{
	}
	
	/**
	 * Constructor to load a movie file.
	 * @param filename	Name of the movie to load (it can be a local path relative to the data folder, or a network path)
	 * @param fps		Desired Frames per Second for the playback. -1 means to use the fps of the movie file.
	 */
	MediaPlayerOCV::MediaPlayerOCV( const char* filename, float fps /*= -1*/ ):
	m_bufferSizeInBytes(0),
	m_videoWidth	( 0 ),
	m_videoHeight	( 0 ),
	m_videoFps		( 0 ),
	m_videoDuration	( 0 ),
	m_loop			( false ),
	m_loopPending	( false	),
	m_newBufferReady( false ),
	m_bIsValid		( false ),
	m_pixelFormat	( RGB	),
	m_mute			( false ),
	m_volume		( 1.0f  ),
	m_currentFrame	( 0 )
	{
		// Load the movie
		load( filename, RGB, fps );
	}
	
	/**
	 * Default destructor.
	 */
	MediaPlayerOCV::~MediaPlayerOCV()
	{
		end();
	}
	
	
	/**
	 * Inits the object to make it valid.
	 */
	bool MediaPlayerOCV::init()
	{
		return true;
	}
	
	/**
	 * Loads a movie file
	 * @param filename		Name of the movie to load (it can be a local path relative to the data folder, or a network path)
	 * @param requestedVideoFormat	Format in which the frames of the movie will be stored. Default RGB, which means that the
	 *								movie file video format will be used (regarding alpha channel: RGB vs RGBA)
	 * @param fps			Desired Frames per Second for the playback. -1 means to use the fps of the movie file.
	 * @return true if the video was succesfully loaded
	 */
	bool MediaPlayerOCV::load( const char* fileName, GraphicsType requestedVideoFormat /*= RGB*/, float fps /*= -1*/  )
	{
		// Build path to file
		bool result = buildPathToFile( fileName );
		if ( !result )
		{
			end();
			return false;
		}

		// Load video
		m_capture.open( m_filePath );
		if ( m_capture.isOpened() == false )
		{
			LOG_ERROR( "MediaPlayerOCV Could not load file %s", m_fileName.c_str() );
			return false;
		}
		
		m_videoWidth    = m_capture.get( CV_CAP_PROP_FRAME_WIDTH );
		m_videoHeight   = m_capture.get( CV_CAP_PROP_FRAME_HEIGHT );
		m_videoFps		= m_capture.get( CV_CAP_PROP_FPS );
		m_videoNFrames	= m_capture.get( CV_CAP_PROP_FRAME_COUNT );	
		m_videoDuration = (float)m_videoNFrames / m_videoFps;
		m_pixelFormat	= RGB;
		m_currentFrame	= 0;
		m_playing		= false;

		
		// Init the frame container to the video size
		m_frameImg.init( m_videoWidth, m_videoHeight, m_pixelFormat );
		m_bufferSizeInBytes = m_videoWidth * m_videoHeight * m_frameImg.getNChannels();
		
		// Check if the requested fps is different than the actual video fps -> if so, change it
 		if ( (fps > 0) && (equal(fps, m_videoFps) == false) )
		{
			m_capture.set( CV_CAP_PROP_FPS, fps );
			m_videoFps = m_capture.get( CV_CAP_PROP_FPS );
		}
		
		LOG( "MediaPlayer: File %s correctly loaded", m_fileName.c_str() );
		
		// The object is valid when the file is loaded
		m_bIsValid = true;
		return true;
	}
	
	
	/**
	 * Releases the object's resources
	 */
	void MediaPlayerOCV::end()
	{
		// Check if already released
		if ( !isValid() )
			return;
		
		// Clear resources
		m_capture.release();

		
		// Clear flags
		m_bIsValid			= false;
		m_newBufferReady	= false;
		m_loopPending		= false;
	}
	
	/**
	 * Updates media playback state
	 */
	void MediaPlayerOCV::update()
	{
		// Check if we have to loop
		if ( m_loopPending )
		{
			jump(0);
			m_loopPending = false;
		}
		
		
		// Set the playhead
		unsigned long millisPlayback = m_timer.getMilliseconds();
		double currentFrame = ((double)millisPlayback/1000.0) * m_videoFps;
		double ratio = currentFrame / (double)m_videoNFrames;
		
		// Request frames until the playhed is where we want
		// NOTE: This is done this way, as in mac / opencv video capture has bugs in CV_CAP_PROP_POS_MSEC, CV_CAP_PROP_POS_FRAMES and CV_CAP_PROP_POS_AVI_RATIO
		// So it is not possible to control the playhead
		m_newBufferReady = false;
		while( m_currentFrame < currentFrame )
		{
			m_capture.grab();
			m_newBufferReady = true;
			m_currentFrame++;
		}
		
		// Check loop
		if ( m_currentFrame >= m_videoNFrames )
		{
			// Looping
			if ( m_loop )
			{
				m_timer.reset();
				m_currentFrame = 0;
				m_capture.set( CV_CAP_PROP_POS_MSEC, 0 );
			}
			// Not looping
			else 
			{
				m_playing = false;
				m_currentFrame = 0;
			}
			
		}
	}
	
	/**
	 * Returns the internal Image (that contains the last buffer coming from the stream)
	 */
	Image& MediaPlayerOCV::getImage()
	{		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 1, "MediaPlayerOCV not corretly initialized. No new frame will be returned" );
			return m_frameImg;
		}
		
		// Update, just in case there are pending operations
		update();
		
		// Check if we have a new buffer to copy
		if ( m_newBufferReady )
			copyBufferIntoImage();
		
		return m_frameImg;
	}
	
	
	
	/**
	 * Returns true if the media is playing, false otherwise     
	 **/
	bool MediaPlayerOCV::isPlaying()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized" );
			return false;
		}
		
		return m_playing;
	}
	
	
	/**
	 * Returns the location of the play head in seconds (that is, the time it has played)     
	 **/
	float MediaPlayerOCV::time()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. Time cannot be obtained" );
			return 0;
		}

		return m_capture.get( CV_CAP_PROP_POS_MSEC ) / 1000.0;
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerOCV::play()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not play" );
			return;
		}
		
		// no loop mode
		m_loop = false;
		m_playing = true;
		
		// Timer that will control the playback
		m_timer.reset();
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerOCV::loop()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not loop" );
			return;
		}
		
		// loop mode
		m_loop = true;  
		m_playing = true;
		
		// Timer that will control the playback
		m_timer.reset();	
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerOCV::stop()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not stop (as it is not playing)" );
			return;
		}
		
		m_currentFrame = 0;
		m_playing = false;
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerOCV::pause()
	{
		LOG_ERROR( "Method not supported in this media player" );
		return;	
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not pause (as it is not playing)" );
			return;
		}

		m_playing = false;
	}
	
	/**
	 * Jumps to a specific location within a movie (specified in seconds)
	 * @param whereInSecs Where to jump in the movie (in seconds)
	 **/
	void MediaPlayerOCV::jump( float whereInSecs )
	{
		LOG_ERROR( "Method not supported in this media player" );
		return;
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not jump" );
			return;
		}
		
		// If we have a new buffer available, clear the flag, we don't want it anymore after the seek
		if ( m_newBufferReady )
			m_newBufferReady	= false;
		
		m_capture.set( CV_CAP_PROP_POS_MSEC, whereInSecs * 1000 );
	}
	
	/**
	 * Sets the relative playback speed of the movie (
	 * Examples: 1.0 = normal speed, 2.0 = 2x speed, 0.5 = half speed
	 * @param rate Speed rate to play the movie
	 **/
	void MediaPlayerOCV::speed( float rate )
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized" );
			return;
		}

		m_capture.set( CV_CAP_PROP_FPS, m_videoFps * rate );
	}
	
	/**
	 * Toggles the audio mute                                                                    
	 **/
	void MediaPlayerOCV::toggleMute()
	{
		LOG_ERROR( "Method not supported in this media player" );
		return;
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized" );
			return;
		}
		
		// Toggle mute
		m_mute = !m_mute;
		
		// if mute -> set volume to 0
		if ( m_mute )
			setVolume( 0 );
		
		// otherwise -> reset previous volume
		else
			setVolume( m_volume );
	}	
	
	/**
	 * Sets audio volume (0..1)
	 * @param volume new volume
	 **/
	void MediaPlayerOCV::setVolume( float volume )
	{
		LOG_ERROR( "Method not supported in this media player" );
		return;
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not play" );
			return;
		}
		
	}
	
	/**
	 * Returns the current audio volume (0..1)
	 * @return current volume
	 **/
	float MediaPlayerOCV::getVolume() const
	{
		LOG_ERROR( "Method not supported in this media player" );
		return 0;
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerOCV not corretly initialized. File will not play" );
			return 0;
		}
		
	}
	
	/**
	 * Builds an absolute path to the file that will be loaded into the player
	 * @return true if there was no problem
	 */
	bool MediaPlayerOCV::buildPathToFile( const String& path )
	{
		// Store incomming path as the filename (if it's a local file, the use would have passed the file path
		// relative to the data folder)
		m_fileName = path;
		
		m_filePath = dataFolder + path;
		if ( !fileExists( m_filePath ) )
		{
			LOG_ERROR( "MediaPlayer: File %s not found in data folder.", path.c_str() );
			return false;
		}
		
		return true;
	}
	
	/**
	 * Copies the last buffer that came from the stream into the Image (drawable)
	 */
	void MediaPlayerOCV::copyBufferIntoImage()
	{
		// Check if video is ok
		if ( !isValid() || !m_newBufferReady)
			return;
		
		// Retrieve frame
		m_capture.retrieve( m_frameImg.getCVMat(), 0 );
		m_frameImg.updateTexture();
		
		// Clear new buffer flag
		m_newBufferReady	= false;
	}
	
	
} // namespace
