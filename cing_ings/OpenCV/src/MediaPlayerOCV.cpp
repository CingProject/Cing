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
#include "OpenCVUtils.h"

#include "opencv2/imgproc/imgproc.hpp"

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/XMLElement.h"

// Framework
#include "framework/UserAppGlobals.h"

// Extern
#undef nil
#include "PTypes/include/pasync.h"


namespace Cing
{
	
    void MediaPlayerOCV::OCVCaptureThread::execute()
	{
		//m_ocvCamera.m_capture.open( m_ocvCamera.getDeviceId() );
		if ( !m_player.m_capture.isOpened() )
		{
			LOG_ERROR( "[MediaPlayerOCV] Player not opened, capture thread will not load" );
			return;
		}

        
        // Capture thread loop
		while( !get_signaled() )
		{            
            // New frame flag
            bool newFrame = false;
            
            // If false, opencv controls the playhead. If false, it is controlled by our timer
            bool setPlayhead = false;
            
            // Set the playhead
            double currentActualFrame = 0.0f;
            if ( setPlayhead )
            {
                unsigned long millisPlayback = m_timer.getMilliseconds();
                double currentFrame = ((double)millisPlayback/1000.0) * m_fps;
                currentActualFrame = m_player.m_capture.get(CV_CAP_PROP_POS_FRAMES);
                
                // Have new frame?
                if ( currentFrame > currentActualFrame )
                {
                    m_player.m_capture.set(CV_CAP_PROP_POS_FRAMES, currentFrame);
                    currentActualFrame = currentFrame;
                }
            }
            
            // And grab new frame (it will be updated in the next call to getImage()
            {
                cv::Mat m_cvCaptureImage;
                newFrame = m_player.m_capture.read( m_cvCaptureImage );
                
                // Convert from BGR to RGB
                cv::cvtColor(m_cvCaptureImage, m_localMat, CV_BGR2RGB);
                
                // Get the current frame position
                if ( !setPlayhead )
                    currentActualFrame = m_player.m_capture.get(CV_CAP_PROP_POS_FRAMES);
                
                // Set the new frame to the player
                if ( newFrame )
                {
                    m_player.setNewFrame( currentActualFrame, m_localMat );
                    
                    // relax the thread
                    relax(20);
                }
            }
		}
	}
	
	void MediaPlayerOCV::OCVCaptureThread::cleanup()
	{
        // nothing to clean up or now
	}
	
	
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
    m_captureThread ( NULL  ),
    m_multithreaded ( false ),
	m_currentFrame	( 0 )
	{
	}
	
	/**
	 * Constructor to load a movie file.
	 * @param filename	Name of the movie to load (it can be a local path relative to the data folder, or a network path)
	 * @param fps		Desired Frames per Second for the playback. -1 means to use the fps of the movie file.
	 */
	MediaPlayerOCV::MediaPlayerOCV( const std::string& filename, float fps /*= -1*/ ):
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
    m_captureThread ( NULL  ),
	m_volume		( 1.0f  ),
    m_multithreaded ( false ),
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
	bool MediaPlayerOCV::load( const std::string& fileName, GraphicsType requestedVideoFormat /*= RGB*/, float fps /*= -1*/  )
	{
        // If this is re-load: release resources first
        if ( isValid() )
            end();
        
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
        
        // Create capture thread
        if ( m_multithreaded )
        {
            // Load the intermediate image
            m_bufferFromThread = toCVMat(m_frameImg).clone();
            
            // Start the thread
            m_captureThread = new OCVCaptureThread( *this, m_timer, m_videoFps );
            m_captureThread->start();
        }
        
		LOG( "MediaPlayer: File %s correctly loaded (Working in Multithreaded mode = [%s]", m_fileName.c_str(), toString(m_multithreaded).c_str());
		
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
		
        // Release thread
        if ( m_multithreaded && m_captureThread )
        {
            // Signal the thread and wait for it to be done
            m_captureThread->signal();
            Ogre::Timer timerLimit;
            timerLimit.reset();
            while( (m_captureThread->get_finished() == false) && (timerLimit.getMilliseconds() < 250) )
                ;
			if ( m_captureThread->get_finished() )
            {
                delete m_captureThread;
            }
            else
                LOG_CRITICAL( "MediaPlayerOCV: capture thread timer expired, and the thread did not finish. Something is wrong, there will be a memory leak" );
            m_captureThread = NULL;
        }
        
		// Clear resources
		m_capture.release();
        
        m_frameImg.end();
        
		// Clear flags
		m_bIsValid			= false;
		m_newBufferReady	= false;
		m_loopPending		= false;
	}
	
	/**
	 * Updates media playback state
     *
     * @param updateTexture If this is false (default), the player is updated but the new frame is not updated
     * in m_frameImg (nor in its texture) until the next call to getImage. With updateTexture set to true, the image and textures are updated if
     * there is a new frame regardless of teh getImage() call. The latter might be slower if you won't need or use the image or texture, but if for example
     * you are using the player just as a texture for an object that use it but don't need to draw the video on screen, you can pass updateTexture to true so that the
     * texture is always up to date.
	 */
	void MediaPlayerOCV::update( bool updateTexture /*= false*/ )
	{
		// Check if we have to loop
		if ( m_loopPending )
		{
			jump(0);
			m_loopPending = false;
		}
       	
		// Set the playhead and get new frame if we are in sigle thread mode
		if ( !m_multithreaded )
		{
            m_newBufferReady = false;
            
			unsigned long millisPlayback = m_timer.getMilliseconds();
			double currentFrame = ((double)millisPlayback/1000.0) * m_videoFps;
        
        	double currentActualFrame = m_capture.get(CV_CAP_PROP_POS_FRAMES);
        	if ( currentFrame > currentActualFrame )
        	{
           		m_capture.set(CV_CAP_PROP_POS_FRAMES, currentFrame);
        
            	// And grab new frame (it will be updated in the next call to getImage()
            	m_newBufferReady = m_capture.grab();
        	}
        
        	// If we got a new frame
        	if ( m_newBufferReady )
        	{
            	m_currentFrame = currentFrame;
        
            	// Update image and texture if necessary
            	copyBufferIntoImage(updateTexture);
	        }
		}
        // We are in multithreaded mode
        else if ( m_newBufferReady )
        {
            // Update playhead
            unsigned long millisPlayback = m_timer.getMilliseconds();
			double currentFrame = ((double)millisPlayback/1000.0) * m_videoFps;
            
        	double currentActualFrame = m_capture.get(CV_CAP_PROP_POS_FRAMES);
            //LOG( "** Current = %f, current actual = %f", currentFrame, currentActualFrame);

        	if ( currentFrame > currentActualFrame )
        	{
                m_currentFrame = currentFrame;
                if (m_currentFrame >= m_videoNFrames)
                {
                    // Looping
                    if ( m_loop )
                    {
                        m_timer.reset();
                        m_currentFrame = 0;
                        m_capture.set( CV_CAP_PROP_POS_FRAMES, 0 );
                    }
                    // Not looping
                    else
                    {
                        m_playing = false;
                        m_currentFrame = 0;
                    }
                }
                
                if ( m_playing )
                    m_capture.set(CV_CAP_PROP_POS_FRAMES, m_currentFrame);
            }
            
            // Get new frames
            if ( m_playing )
            {
                copyFrameFromThread();
                if ( updateTexture )
                    m_frameImg.updateTexture();
            }
        }
        
		// Check loop
		if ( !m_multithreaded && (m_currentFrame >= m_videoNFrames) )
		{
			// Looping
			if ( m_loop )
			{
				m_timer.reset();
				m_currentFrame = 0;
                if ( m_multithreaded )
                {
                    //boost::mutex::scoped_lock lock(m_mutex);
                    m_capture.set( CV_CAP_PROP_POS_FRAMES, 0 );
                }
                else
                    m_capture.set( CV_CAP_PROP_POS_FRAMES, 0 );
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

        float currentTime = m_capture.get( CV_CAP_PROP_POS_MSEC ) / 1000.0;
        return currentTime;
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
        m_capture.set( CV_CAP_PROP_POS_MSEC, 0 );
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
		m_capture.set( CV_CAP_PROP_POS_MSEC, 0 );
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
	bool MediaPlayerOCV::buildPathToFile( const std::string& path )
	{
		// Store incomming path as the filename (if it's a local file, the use would have passed the file path
		// relative to the data folder)
		 m_fileName = path;
        
        if ( isPathAbsolute(path) )
            m_filePath = path;
        else
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
	 * @param updateTexture if false, only the ram buffer of m_frameImg is udpate, if true, it's GPU texture is updated as well.
     */
	void MediaPlayerOCV::copyBufferIntoImage( bool updateTexture /*= false*/)
	{
		// Check if video is ok
		if ( !isValid() || !m_newBufferReady)
			return;
		
		// Retrieve frame (different paths windows/mac)
        // TODO: review why they differ
#ifdef WIN32
        cv::Mat videoFrame;
        cv::Mat outMat = toCVMat(m_frameImg);
		bool result = m_capture.retrieve( videoFrame );
        if ( result )
        {
            // Convert from BGR to RGB
            cv::cvtColor(videoFrame, outMat, CV_BGR2RGB);
            
            if ( updateTexture )
                m_frameImg.updateTexture();
        }
#elif __APPLE__
        cv::Mat outMat = toCVMat(m_frameImg);
        cv::Mat videoFrame;
        
        // NOTE: for some reason, the capture stores the image in a new mat that is RGBA format instead of RGB, so we are copying to
        // a temp image to fix this for now (less optimized but...)
		bool result = m_capture.retrieve( videoFrame );
        if ( result )
        {
            // Convert from BGR to RGB
            cv::cvtColor(videoFrame, outMat, CV_BGR2RGB);
            
            if ( updateTexture )
                m_frameImg.updateTexture();
            else
                m_frameImg.setUpdateTexture();
        }
#endif
        
		// Clear new buffer flag
		m_newBufferReady	= false;
	}
    
    /*
     * Called from the capture thread to store a new video frame coming from the capture (movie file).
     */
    void MediaPlayerOCV::setNewFrame( unsigned int currentFrameNumber, cv::Mat& currentFrame )
    {
        if ( !isValid() )
            return;
        
        //LOG( "**^ MediaPlayerOCV::setNewFrame: BEFORE LOCK. Frame: %d", currentFrameNumber);
        boost::mutex::scoped_lock lock(m_mutex);
        if ( lock )
        {
            currentFrame.copyTo(m_bufferFromThread);
            m_currentFrame = currentFrameNumber;
            m_newBufferReady = true;
        }
    }
    
    /*
     * Copies the new frame received from the capture thread into the MediaPlayerOCV internal image (the one that will be accessed from the outside).
     */
    void MediaPlayerOCV::copyFrameFromThread()
    {
        if ( !isValid() )
            return;
    
        // Try lock (instead of lock) to avoid stalling the main thread
        boost::mutex::scoped_try_lock lock(m_mutex);
        if ( lock )
        {
            // Copy the new frame
            cv::Mat imageMat = toCVMat(m_frameImg);
            m_bufferFromThread.copyTo(imageMat);
            m_frameImg.setUpdateTexture();
            m_newBufferReady = false;
        }
    }
	
	
} // namespace
