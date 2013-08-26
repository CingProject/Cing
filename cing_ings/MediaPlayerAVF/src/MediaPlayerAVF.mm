//
//  MediaPlayerAVF.cpp
//  Cing
//
//  Created by Julio Obelleiro on 8/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#include "MediaPlayerAVF.h"

// Common
#include "common/CommonUtilsIncludes.h"

#include "AVFVideoWrapper.h"

namespace Cing
{
	
	/**
	 * Default constructor.
	 */
	MediaPlayerAVF::MediaPlayerAVF():
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
        m_player = NULL;
	}
	
	
	/**
	 * Default destructor.
	 */
	MediaPlayerAVF::~MediaPlayerAVF()
	{
		end();
	}
	
	
	/**
	 * Inits the object to make it valid.
	 */
	bool MediaPlayerAVF::init()
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
	bool MediaPlayerAVF::load( const std::string& fileName, GraphicsType requestedVideoFormat /*= RGB*/, float fps /*= -1*/  )
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
		
        // Allocate AVPlayer wrapper
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        m_player = [[AVFVideoWrapper alloc] init];
        
        // Set desired format
        AVFPixelFormat avfFormat = AVF_RGB;
        [m_player setPixelFormat:avfFormat];
        m_pixelFormat	= RGB;
        
        // Loa the file
        [m_player loadFile:[NSString stringWithUTF8String:m_filePath.c_str()]];        
        [pool release];
        
        m_playing = false;
        m_desiredFps = fps;
        
		LOG( "MediaPlayer: File %s correctly loaded.", m_fileName.c_str() );
		
		// The object is valid when the file is loaded (in this case loading is asynchronous, so not valid yet! (update() checks for it to be ready)
		m_bIsValid = false;
		return true;
	}
	
	
	/**
	 * Releases the object's resources
	 */
	void MediaPlayerAVF::end()
	{
		// Check if already released
		if ( !isValid() )
			return;
		
    
		// TODO: Clear resources
        
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
	void MediaPlayerAVF::update( bool updateTexture /*= false*/ )
	{
        // Check if the player is ready
        bool ready = [m_player ready];
        
        if ( !ready )
            return;
        
        // Finally file loaded? (load is asynchronous)
        if ( ready && !isValid() )
        {
            m_videoWidth    = [m_player size].width;
            m_videoHeight   = [m_player size].height;
            m_videoFps		= [m_player fps];
            m_videoDuration = CMTimeGetSeconds([m_player duration]);
            m_videoNFrames	= m_videoFps * m_videoDuration;
            m_currentFrame	= 0;
            m_playing		= false;
            
            
            // Init the frame container to the video size
            m_frameImg.init( m_videoWidth, m_videoHeight, m_pixelFormat );
            m_bufferSizeInBytes = m_videoWidth * m_videoHeight * m_frameImg.getNChannels();
            
            // Check if the requested fps is different than the actual video fps -> if so, change it
            
             if ( (m_desiredFps > 0) && (equal(m_desiredFps, m_videoFps) == false) )
             {
                 // Try to set the desired fps
                 [m_player setFps:m_desiredFps];
                 m_videoFps = m_desiredFps;
             }
            

            m_bIsValid = true;
        }
        
        if ( isValid() )
        {
            // Get new frames
            [m_player update];
            bool newFrameReady = [m_player newFrameReady];
            if ( newFrameReady )
            {
                const unsigned char *newFrameBuffer = (const unsigned char*)[m_player lastFrameBuffer];
                if ( newFrameBuffer )
                {
                    // We have new frame! copy contents to our image buffer
                    unsigned int widthStep = [m_player widthStep];
                    m_frameImg.setData( newFrameBuffer, m_videoWidth, m_videoHeight, m_pixelFormat, widthStep );
                    if ( updateTexture )
                        m_frameImg.updateTexture();
                    else
                        m_frameImg.setUpdateTexture();
                }
                
            }
            
        
        }
        
        
		// Check if we have to loop
		/*if ( m_loopPending )
		{
			jump(0);
			m_loopPending = false;
		}
       	*/
        
        
		// Check loop
		/*if ( m_currentFrame >= m_videoNFrames )
		{
			// Looping
			if ( m_loop )
			{
				m_timer.reset();
				m_currentFrame = 0;
                // m_capture.set( CV_CAP_PROP_POS_FRAMES, 0 );
			}
			// Not looping
			else
			{
				m_playing = false;
				m_currentFrame = 0;
			}
		}
         */
	}
	
	/**
	 * Returns the internal Image (that contains the last buffer coming from the stream)
	 */
	Image& MediaPlayerAVF::getImage()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 1, "MediaPlayerAVF not corretly initialized. No new frame will be returned" );
			return m_frameImg;
		}
		
		return m_frameImg;
	}
	
	
	
	/**
	 * Returns true if the media is playing, false otherwise
	 **/
	bool MediaPlayerAVF::isPlaying()
	{
		return m_playing;
	}
	
	
	/**
	 * Returns the location of the play head in seconds (that is, the time it has played)
	 **/
	float MediaPlayerAVF::time()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized. Time cannot be obtained" );
			return 0;
		}
        
        //float currentTime = m_capture.get( CV_CAP_PROP_POS_MSEC ) / 1000.0;
        return [m_player time];
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerAVF::play()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized. File will not play" );
			return;
		}
		
		// no loop mode
		m_loop = false;
		m_playing = true;
		
		// Timer that will control the playback
		m_timer.reset();
        [m_player play];
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerAVF::loop()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized. File will not loop" );
			return;
		}
		
		// loop mode
		m_loop = true;
		m_playing = true;
		
		// Timer that will control the playback
		m_timer.reset();
        [m_player play];
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerAVF::stop()
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized. File will not stop (as it is not playing)" );
			return;
		}
		
		m_currentFrame = 0;
		m_playing = false;
        [m_player stop];
	}
	
	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerAVF::pause()
	{
		LOG_ERROR( "Method not supported in this media player" );
		return;
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized. File will not pause (as it is not playing)" );
			return;
		}
        
		m_playing = false;
	}
	
	/**
	 * Jumps to a specific location within a movie (specified in seconds)
	 * @param whereInSecs Where to jump in the movie (in seconds)
	 **/
	void MediaPlayerAVF::jump( float whereInSecs )
	{
		/*LOG_ERROR( "Method not supported in this media player" );
		return;
		
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized. File will not jump" );
			return;
		}
		
		// If we have a new buffer available, clear the flag, we don't want it anymore after the seek
		if ( m_newBufferReady )
			m_newBufferReady	= false;
		
        m_capture.set( CV_CAP_PROP_POS_MSEC, whereInSecs * 1000 );
         */
		
	}
	
	/**
	 * Sets the relative playback speed of the movie (
	 * Examples: 1.0 = normal speed, 2.0 = 2x speed, 0.5 = half speed
	 * @param rate Speed rate to play the movie
	 **/
	void MediaPlayerAVF::speed( float rate )
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerAVF not corretly initialized" );
			return;
		}
        
	}
    
    /**
	 * Builds an absolute path to the file that will be loaded into the player.
     * Private attributes m_fileName and m_filePath are updated accordingly
	 * @return true if there was no problem
	 */
	bool MediaPlayerAVF::buildPathToFile( const std::string& path )
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
    
}
