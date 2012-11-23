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

#include "MediaPlayerGS.h"
#include "GStreamerManager.h"

// GStreamer
#include <gst/video/video.h>

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/XMLElement.h"
#include "common/LogManager.h"

// Framework
#include "framework/UserAppGlobals.h"

// Graphics
#include "graphics/Color.h"
#include "graphics/GraphicsUserAPI.h"

namespace Cing
{

	// GStreamer Callback functions

	/**
	 * @internal
	 * Called from GStreamer when there is new decoded frame from the playing video
	 * @param sink		GStremaer Sink that is giving us the new frame
	 * @param userData	Is the pointer to the MediaPlayerGS instance
	 */
	static GstFlowReturn onNewBufferHandler(GstAppSink *sink, gpointer userData)
	{ 
		MediaPlayerGS* mediaPlayer = reinterpret_cast<MediaPlayerGS*>(userData);
		if ( mediaPlayer == NULL )
		{
			LOG_ERROR( "MediaPlayerGS::onNewBufferHandler error. MediaPlayerGS is NULL. Cannot store new frame" );
			return GST_FLOW_OK;
		}

		// Get appSink state
		GstState currentState;
		GstStateChangeReturn ret = gst_element_get_state(GST_ELEMENT(mediaPlayer->getPlayBin()), &currentState, NULL, 0);

		// Get the new buffer (or the preroll if the stream is paused, as there should be no new buffer in this case)
		GstBuffer* buffer = NULL;
		if ( currentState == GST_STATE_PAUSED )
			buffer = gst_app_sink_pull_preroll(sink );
		else
			buffer = gst_app_sink_pull_buffer( sink );

		// Check new buffer
		if ( buffer == NULL )
		{
			LOG_ERROR( "GStreamer reported new buffer, but the new buffer is NULL" );
			return GST_FLOW_OK;
		}

		// Buffer is ok -> notify
		mediaPlayer->onNewBuffer( buffer );

		// All good
		return GST_FLOW_OK;
	}

	/**
	 * @internal
	 * Called from GStreamer when there is a new message comming from the Bus (like End of Stream)
	 * @param bus		GStremaer Bus that is giving us the message
	 * @param message	The GStreamer message itself
	 * @param userData	The Gstreamer player that initiated the playback
	 */
	static GstBusSyncReply onSyncBusMessageHandler(GstBus * bus, GstMessage * message, gpointer userData)
	{
		// Get pointer to the player
		MediaPlayerGS* player = reinterpret_cast<MediaPlayerGS*>(userData);
		if ( !player )
		{
			LOG_ERROR( "onSyncBusMessage Error: NULL player. Cannot process the GStreamer message" );
			return GST_BUS_PASS;
		}

		// Decode message
		switch (GST_MESSAGE_TYPE(message))
		{
		case GST_MESSAGE_EOS:
			//LOG_TRIVIAL( "End of stream" );
			player->onEndOfStream();
			break;

		case GST_MESSAGE_ERROR:
			player->stop();

			// Get error info
			GError*	error;
			gchar*	debugInfo;
			gst_message_parse_error(message, &error, &debugInfo);

			// Report it
			LOG_ERROR( "Gstreamer Error (%s): %s",  GST_OBJECT_NAME (message->src), error->message );

			// Free error resources
			g_error_free(error);
			g_free(debugInfo);

			// Stop the media player
			player->stop();

			break;

		default:
			break;
		}

		return GST_BUS_PASS;
	}


	/**
	 * Default constructor.
	 */
	MediaPlayerGS::MediaPlayerGS():
		m_pipeline		( NULL ),
		m_appSink		( NULL ),
		m_appBin		( NULL ),
		m_internalBuffer( NULL ),
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
		m_endOfFileReached	(false),
		m_paused			(false),
		m_playing			(false),
		m_useGrayScale		(false)
	{
	}

	/**
	 * Constructor to load a movie file.
	 * @param filename	Name of the movie to load (it can be a local path relative to the data folder, or a network path)
	 * @param fps		Desired Frames per Second for the playback. -1 means to use the fps of the movie file.
	 */
	MediaPlayerGS::MediaPlayerGS( const char* filename, float fps /*= -1*/ ):
		m_pipeline		( NULL ),
		m_appSink		( NULL ),
		m_appBin		( NULL ),
		m_internalBuffer( NULL ),
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
		m_endOfFileReached	(false),
		m_paused			(false),
		m_playing			(false),
		m_useGrayScale		(false)
	{
		// Load the movie
		load( filename, RGB, fps );
	}

	/**
	 * Default destructor.
	 */
	MediaPlayerGS::~MediaPlayerGS()
	{
		end();
	}


	/**
	 * Inits the object to make it valid.
	 */
	bool MediaPlayerGS::init()
	{
		LOG_ENTER_FUNCTION;
	
		// GStreamer Init
		return GStreamerManager::initGStreamer();

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Loads a movie file
	 * @param filename		Name of the movie to load (it can be a local path relative to the data folder, or a network path)
	 * @param requestedVideoFormat	Format in which the frames of the movie will be stored. Default RGB, which means that the
	 *								movie file video format will be used (regarding alpha channel: RGB vs RGBA)
	 * @param fps			Desired Frames per Second for the playback. -1 means to use the fps of the movie file.
	 * @return true if the video was succesfully loaded
	 */
	bool MediaPlayerGS::load( const std::string& fileName, GraphicsType requestedVideoFormat /*= RGB*/, float fps /*= -1*/  )
	{
		LOG_ENTER_FUNCTION;
	
		// Init the player (to make sure GStreamer is initialized)
		bool result = init();
		if ( !result )
		{
			LOG_ERROR( "MediaPlayerGS::load. Error loading %s, GStreamer could not load it correctly.", fileName.c_str() );
			end();
			return false;
		}

		// Build path to file
		result = buildPathToFile( fileName );
		if ( !result )
		{
			LOG_ERROR( "MediaPlayerGS::load. File Not Found: %s", fileName.c_str() );
			end();
			return false;
		}

		// Create the Gstreamer Pipeline
		result = createPipeline();
		if ( !result )
		{
			LOG_ERROR( "MediaPlayerGS::load. Error loading %s, GStreamer could create the pipeline to play it.", fileName.c_str() );
			end();
			return false;
		}

		// By default, playbin creates its own window to which it streams
		// video output.  We create an appsink element to allow video to be
		// streamed to our own Cing Image
		result = createVideoSink();
		if ( !result )
		{
			LOG_ERROR( "MediaPlayerGS::load. Error loading %s, GStreamer could create the sink to play it.", fileName.c_str() );
			end();
			return false;
		}

		// Configure the output video format 
		result = configureVideoFormat( requestedVideoFormat );
		if ( !result )
		{
			LOG_ERROR( "MediaPlayerGS::load. Error loading %s, Image format could not be configured.", fileName.c_str() );
			end();
			return false;
		}

		// Init the frame container to the video size
		m_frameImg.init( m_videoWidth, m_videoHeight, m_pixelFormat );
		m_bufferSizeInBytes = m_videoWidth * m_videoHeight * m_frameImg.getNChannels();
		m_internalBuffer	= new unsigned char[m_bufferSizeInBytes];

		// Init grayscale image if it was requested
		if ( m_useGrayScale )
			m_frameImgGray.init( m_videoWidth, m_videoHeight, GRAYSCALE );


		// Query video duration
		GstFormat format = GST_FORMAT_TIME;
		gint64 duration;
		gboolean res = gst_element_query_duration( GST_ELEMENT(m_pipeline), &format, &duration );
		if ( res != 1 )
		{
			LOG_ERROR( "MediaPlayerGS::time() Error getting video duration" );
			end();
			return false;
		}
		m_videoDuration = duration / 1000000000.0;

		// Also store the number of frames
		m_nFrames  =  m_videoFps * m_videoDuration;

		LOG( "MediaPlayer: File correctly loaded: %s", m_fileName.c_str() );

		// Init other vars
		m_endOfFileReached	= false;
		m_loopPending		= false;
		m_paused			= false;
		m_playing			= false;
	
		// The object is valid when the file is loaded
		m_bIsValid = true;

		// Check if the requested fps is different than the actual video fps -> if so, change it
 		if ( (fps > 0) && (equal(fps, m_videoFps) == false) )
		{
			float newSpeed = fps/m_videoFps;
			speed( newSpeed );
		}


		LOG_EXIT_FUNCTION;

		return true;
	}


	/**
	* Releases the object's resources
	*/
	void MediaPlayerGS::end()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if already released
		if ( !isValid() )
			return;

		// Clean Gstreamer stuff
		gst_element_set_state (m_pipeline, GST_STATE_NULL);
		gst_object_unref (GST_OBJECT (m_pipeline));

		// Clear pointers
		delete []m_internalBuffer;
		m_internalBuffer	= NULL;
		m_pipeline			= NULL;
		m_appSink			= NULL;
		m_appBin			= NULL;
			
		// Clear flags
		m_bIsValid			= false;
		m_newBufferReady	= false;
		m_loopPending		= false;
		m_paused			= false;
		m_playing			= false;

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Updates media playback state
	 */
	void MediaPlayerGS::update()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if we have to loop
		if ( m_loopPending )
		{
			jump(0);
			m_loopPending = false;
		}

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Returns the internal Image (that contains the last buffer coming from the stream)
	 */
	Image& MediaPlayerGS::getImage()
	{		
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. No valid will be returned" );
		}
		else
		{
			// Update, just in case there are pending operations
			update();

			// Check if we have a new buffer to copy
			if ( m_newBufferReady )
				copyBufferIntoImage();
		}

		LOG_EXIT_FUNCTION;

		// Return color or gray image
		if ( m_useGrayScale )
		{
			// Convert it to gray and regurn it
			m_frameImgGray = m_frameImg;
			return m_frameImgGray;
		}
		// Returning color image
		return m_frameImg;
	}



	/**
	 * Returns true if the media is playing, false otherwise     
	 **/
	bool MediaPlayerGS::isPlaying()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized" );
			return false;
		}

		LOG_EXIT_FUNCTION;

		return m_playing;
	}

	/**
	 * Returns true if the media is paused, false otherwise     
	 **/
	bool MediaPlayerGS::isPaused()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized" );
			return false;
	}

		LOG_EXIT_FUNCTION;

		return m_paused;
	}


	/**
	 * Returns the location of the play head in seconds (that is, the time it has played)     
	 **/
	float MediaPlayerGS::time()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. Time cannot be obtained" );
			return 0;
		}

		GstFormat format = GST_FORMAT_TIME;
		gint64 currentPosition;
		gboolean result = gst_element_query_position( GST_ELEMENT(m_pipeline), &format, &currentPosition );
		if ( result != 1 )
		{
			LOG_ERROR( "MediaPlayerGS::time() Error getting curret position" );
			return 0;
		}

		LOG_EXIT_FUNCTION;

		return currentPosition / 1000000000.0f;
	}

	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerGS::play()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not play" );
			return;
		}

		// Clean bus to avoid accumulation of messages
		flushBusMsg();

		// If the end of the file was reached, reset the playhead to the beginning
		if ( m_endOfFileReached )
		{
			jump(0);
			m_endOfFileReached	= false;
		}

		// no loop mode
		m_loop = false;
		m_paused			= false;
		m_playing			= true;
		
		// Play Stream
		setPipelineState(GST_STATE_PLAYING);

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerGS::loop()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not loop" );
			return;
		}

		// Clean bus to avoid accumulation of messages
		flushBusMsg();

		// If the end of the file was reached, reset the playhead to the beginning
		if ( m_endOfFileReached )
		{
			jump(0);
			m_endOfFileReached	= false;
		}


		// loop mode
		m_loop = true;  
		m_paused		= false;
		m_playing		= true;

		// Play Stream
		setPipelineState(GST_STATE_PLAYING);

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerGS::stop()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not stop (as it is not playing)" );
			return;
		}

		// Reset playhead and pause
		jump(0);
		setPipelineState(GST_STATE_PAUSED);
		
		m_paused	= false;
		m_playing	= false;

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Plays the media with no loop
	 **/
	void MediaPlayerGS::pause()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not pause (as it is not playing)" );
			return;
		}

		// pause the media
		setPipelineState(GST_STATE_PAUSED);
		m_paused	= true;
		m_playing	= false;

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Jumps to a specific location within a movie (specified in seconds)
	 * @param whereInSecs Where to jump in the movie (in seconds)
	 **/
	void MediaPlayerGS::jump( float whereInSecs )
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not jump" );
			return;
		}

		// Clamp time position
		whereInSecs = constrain( whereInSecs, 0, duration() );

		// Clean bus to avoid accumulation of messages
		flushBusMsg();

		// If we have a new buffer available, clear the flag, we don't want it anymore after the seek
			m_newBufferReady	= false;

		// Perform the seek
		GstSeekFlags	flags		= (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE);
		gint64			currentPos	= whereInSecs * GST_SECOND; /// Time in Gstreamer is in nano seconds
		double			speed		= 1.0;
		GstFormat		format		= GST_FORMAT_TIME;
		gboolean ret = gst_element_seek(GST_ELEMENT(m_pipeline), speed, format, flags, GST_SEEK_TYPE_SET, currentPos, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
		if ( ret != 1 )
		{
			LOG_ERROR( "MediaPlayerGS::jump. Error performing GStreamer seek" );
		}

		// Get Current state
		GstState currentState, pendingState;
		GstStateChangeReturn currentRet = gst_element_get_state(GST_ELEMENT(m_pipeline), &currentState, &pendingState, 2000 * GST_MSECOND );

		// Play the pipeline (to get the new frame after the seek), but tell it to return to pause if it's not currently playing
		if ( currentState == GST_STATE_PAUSED )
		{
			setPipelineState(GST_STATE_PLAYING);
			setPipelineState(GST_STATE_PAUSED);
		}

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Jumps to a specific location within a movie (specified in frame number)
	 * @param whereInSecs Where to jump in the movie (in seconds)
	 **/
	void MediaPlayerGS::jumpToFrame	( unsigned int frameNumber )
	{
		LOG_ENTER_FUNCTION;
	
		// Clamp time position
		frameNumber = constrain( frameNumber, 0, numberOfFrames()-1 );

		// Calculate time in seconds for this frame
		double whereInSecs = (double)frameNumber / fps();
		
		// Jump to that second
		jump( whereInSecs );

		LOG_EXIT_FUNCTION;

	}

	/**
	 * Sets the relative playback speed of the movie (
	 * Examples: 1.0 = normal speed, 2.0 = 2x speed, 0.5 = half speed
	 * @param rate Speed rate to play the movie
	 **/
	void MediaPlayerGS::speed( float rate )
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized" );
			return;
		}
		
		// Perform the seek
		GstSeekFlags	flags		= (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_SKIP | GST_SEEK_FLAG_ACCURATE);
		int				currentPos	= 0;
		GstFormat		format		= GST_FORMAT_TIME;
		gboolean ret = gst_element_seek(GST_ELEMENT(m_pipeline), rate, format, flags, GST_SEEK_TYPE_NONE, currentPos, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
		if ( ret != 1 )
		{
			LOG_ERROR( "MediaPlayerGS::speed. Error performing GStreamer seek" );
		}

		LOG_EXIT_FUNCTION;

	}

	/**
	 * Toggles the audio mute                                                                    
	 **/
	void MediaPlayerGS::toggleMute()
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized" );
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

		LOG_EXIT_FUNCTION;
	}	

	/**
	 * Sets audio volume (0..1)
	 * @param volume new volume
	 **/
	void MediaPlayerGS::setVolume( float volume )
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not play" );
			return;
		}

		g_object_set (G_OBJECT(m_pipeline), "volume", volume, (const char*)NULL);

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Returns the current audio volume (0..1)
	 * @return current volume
	 **/
	float MediaPlayerGS::getVolume() const
	{
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 5, "MediaPlayerGS not corretly initialized. File will not play" );
			return 0;
		}

		float volume;
		g_object_get (G_OBJECT(m_pipeline), "volume", &volume, (const char*)NULL);

		LOG_EXIT_FUNCTION;

		return volume;
	}

	/**
	 * Notifies about a new buffer ready from the stream.
	 * The buffer is released (unreffed) when it has been copied, which happens when the image is requested from the user)
	 * @param newBuffer New GStreamer buffer ready to be copied
	 */
	void MediaPlayerGS::onNewBuffer( GstBuffer* newBuffer )
	{
		//LOG_ENTER_FUNCTION;
	
		// Check buffer
		if ( newBuffer == NULL )
			return;

		// Set new buffer flag (so that it's copied into the image when it is required)
		m_newBufferReady = true;

		// Obtain the dimensions of the video.
		GstCaps* caps = gst_buffer_get_caps(newBuffer);

		GstVideoFormat currentVideoFormat;
		int width		= 0;
		int height		= 0;
		int ratioNum	= 0;
		int ratioDen	= 0;
		float pixelRatio	= 1.0;
		for (size_t i = 0; i < gst_caps_get_size(caps); ++i)
		{
			GstStructure* structure = gst_caps_get_structure(caps, i);

			// Get current video format and dimensions
			gst_video_format_parse_caps( caps, &currentVideoFormat, &width, &height );

			// Aspect Ratio (even if it's not used for now)
			if (gst_structure_get_fraction(structure, "pixel-aspect-ratio",&ratioNum, &ratioDen))
			{
				pixelRatio = ratioNum / static_cast<float>(ratioDen);
			}
		}

		// Check buffer dimensions (to make sure they match with the Image before copying)
		if ( (m_videoWidth != width) || (m_videoHeight != height) )
		{
			LOG_ERROR( "MediaPlayerGS::copyBufferIntoImage. New frame's dimensions do not match with stored dimensions. New frame will not be loaded" );
			return;
		}

		// Copy Gstreamer buffer into our internal temporary buffer
		// We do this instead of keeping gstreamer buffer reference, as doing so, gstreamer crashes (or throws errors)
		m_bufferMutex.lock();
		memcpy( m_internalBuffer, (char*)GST_BUFFER_DATA(newBuffer), m_bufferSizeInBytes );

		// We have the image -> the buffer is ready to be reused by GStreamer
		gst_buffer_unref(newBuffer);

		// operation done
		m_bufferMutex.unlock();

		//LOG_EXIT_FUNCTION;
	}

 
	/**
	 * Notifies that the stream that is being player has reached its end
	 */
	void MediaPlayerGS::onEndOfStream()
	{
		LOG_ENTER_FUNCTION;

		// Check if we need to loop
		if ( m_loop )
		{
			m_loopPending = true;
		}
		// End of playback
		else
		{
			// Flag the end of the stream
			m_endOfFileReached	= true;
			m_paused			= false;
			m_playing			= false;
		}
		LOG_EXIT_FUNCTION;
	}

	/**
	 * Builds an absolute path to the file that will be loaded into the player
	 * @return true if there was no problem
	 */
	bool MediaPlayerGS::buildPathToFile( const String& path )
	{
		LOG_ENTER_FUNCTION;
	
		// Store incomming path as the filename (if it's a local file, the use would have passed the file path
		// relative to the data folder)
		m_fileName = path;

		// Check if it is a network path
		if ( m_fileName.find( "://" ) != std::string::npos )
			m_filePath = m_fileName;
		
		// It looks like a local path -> check if it exists
		else
		{		
			m_filePath = dataFolder + path;
			if ( !fileExists( m_filePath ) )
			{
				LOG_ERROR( "MediaPlayer: File %s not found in data folder.", path.c_str() );
				LOG_ERROR( "Absolute path to file: %s", m_filePath.c_str() );
				return false;
			}

			// Build uri in the formtat tha GStreamer expects it
			m_filePath = "file:///" + m_filePath;
		}

		LOG_EXIT_FUNCTION;

		return true;
	}

	/**
	 * Creates and configures the GStreamer pipeline that will allow the media playback and control
	 * @return true if there was no problem
	 */
	bool MediaPlayerGS::createPipeline()
	{
		LOG_ENTER_FUNCTION;
	
		// Create the GStreamer pipeline (playbin2 is an all-in-one audio and video player)
		m_pipeline = gst_element_factory_make ("playbin2", NULL);
		if ( m_pipeline == NULL )
		{
			LOG_ERROR( "GStreamer Error: Could not create pipeline playbin2" );
			return false;
		}

		// Set the location of the file to load
		g_object_set (G_OBJECT (m_pipeline), "uri", m_filePath.c_str(), NULL);

		// Listen for messages on the playbin pipeline bus (to detect events like end of stream)
		GstElement *bus = (GstElement *)gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
		gst_bus_set_sync_handler (GST_BUS(bus), (GstBusSyncHandler)onSyncBusMessageHandler, this);
		gst_object_unref(bus); 

		LOG_EXIT_FUNCTION;

		return true;
	}

	/**
	 * Sets a specific state to the pipeline
	 * @state state to be set. The state change might not syncrhonous.
	 */	
	void MediaPlayerGS::setPipelineState( GstState state )
	{
		LOG_ENTER_FUNCTION;
	
		// Check if video is ok
		if ( !isValid() )
		{
			LOG_ERROR( "MediaPlayerGS is not valid yet. Cannot change the pipeline state. Did you call load()?" );
			return;
		}

		// Change the state
		GstStateChangeReturn ret = gst_element_set_state(GST_ELEMENT(m_pipeline), state);
		if ( (ret != GST_STATE_CHANGE_SUCCESS) && (ret != GST_STATE_CHANGE_ASYNC) )
			LOG_ERROR( "GStreamer Error, State could not be changed in the pipeline. Error Code %d", ret );

		LOG_EXIT_FUNCTION;
	}



	/**
	 * Creates and configures the GStreamer video sink that will allow our app to get the decoded video frames
	 * @return true if there was no problem
	 */
	bool MediaPlayerGS::createVideoSink()
	{
		LOG_ENTER_FUNCTION;
	
		// Create our video sink
		m_appSink = gst_element_factory_make("appsink", NULL);
		if ( m_appSink == NULL )
		{
			LOG_ERROR( "GStreamer Error: Could not create the video app sync" );
			return false;
		}

		// Set the new sink as the pipeline video sink
		g_object_set (G_OBJECT(m_pipeline), "video-sink", m_appSink, (const char*)NULL);

		// Configure the video sink

		// Make it play the media in real time
		gst_base_sink_set_sync(GST_BASE_SINK(m_appSink), true);
		g_object_set(G_OBJECT(m_appSink), "drop", true, NULL);
		//< NOTE: this should be set to one (as we only want 1 buffer), but the performance is better disabling it 
		//-> check memory hit
		//g_object_set(G_OBJECT(m_appSink), "max-buffers", 1, NULL); 

		//Listen for new-buffer signals
		g_object_set(G_OBJECT(m_appSink), "emit-signals", true, NULL);
		g_signal_connect(G_OBJECT(m_appSink), "new-buffer", G_CALLBACK(onNewBufferHandler), this);

		LOG_EXIT_FUNCTION;

		// all fine
		return true;
	}

	/**
	 * Detects and set the most suitable video format trying to match the requested video format, and the movie video format
	 * @param requestedVideoFormat	Requested video format for the Image video frames
	 * @param movieVideoFormat		Movie file video format
	 */
	bool MediaPlayerGS::configureVideoFormat( GraphicsType requestedVideoFormat )
	{
		LOG_ENTER_FUNCTION;

		// Define the video format in GStreamer terms (according to the requested Cing video format)
		m_outputGstVideoFormat = cingToGstPixelFormat( requestedVideoFormat );

		// Create custom caps, to force a certain video output format
		GstCaps* caps = gst_caps_from_string(m_outputGstVideoFormat.c_str());
		if ( caps == NULL )
		{
			LOG_ERROR( "GStreamer Error: Could not create the simple caps" );
			return false ;
		}

		gst_app_sink_set_caps(GST_APP_SINK(m_appSink), caps);
		gst_caps_unref(caps);

		// Store the image cing pixel format in which the frames will be stored
		m_pixelFormat = gstToCingPixelFormat(m_outputGstVideoFormat);

		// Pause Stream (this will allow us to start getting frames as soon as the user plays the video)
		GstState currentState, pendingState;
		gst_element_set_state (GST_ELEMENT(m_pipeline), GST_STATE_PAUSED); 
		GstStateChangeReturn ret = gst_element_get_state(GST_ELEMENT(m_pipeline), &currentState, &pendingState, 2000 * GST_MSECOND );
		if ( ret == GST_STATE_CHANGE_FAILURE )
		{
			LOG_ERROR( "MediaPlayerGS: Error setting GST_STATE_PAUSED state. The video dimensions might not be retrieved correctly." );
		}

		// Security check: check that video format was set succesfully

		// Get the player buffer to retrieve its caps (to get info about the video format)
		GstBuffer* buffer = NULL;
		g_object_get( GST_ELEMENT(m_pipeline), "frame", &buffer, NULL);
		
		// Get caps from buffer
		caps = gst_buffer_get_caps(buffer);
		gst_buffer_unref(buffer);

		// Get current video format and dimensions
		GstVideoFormat currentVideoFormat;
		gst_video_format_parse_caps( caps, &currentVideoFormat, &m_videoWidth, &m_videoHeight );

		// Get Buffer size for each component
		unsigned int nChannels = numberOfChannels( m_pixelFormat );
		int stride = 0;
		for ( unsigned int i = 0; i < nChannels; ++i )
		{
			stride = gst_video_format_get_row_stride( currentVideoFormat, i, m_videoWidth );
		}

		// Get current fps
		gint  fps_n, fps_d;
		gboolean res = gst_video_parse_caps_framerate( caps, &fps_n, &fps_d );
		m_videoFps = (float)fps_n / (float)fps_d;

		gst_caps_unref(caps);

		// Check format
		if ( checkVideoFormatMatch( currentVideoFormat, requestedVideoFormat ) == false )
		{
			LOG_ERROR( "MediaPlayerGS: Error: Video format could not be set correctly to the requested format" );
			return false;
		}

		LOG_EXIT_FUNCTION;

		// all good
		return true;
	}


	/**
	 * Copies the last buffer that came from the stream into the Image (drawable)
	 */
	void MediaPlayerGS::copyBufferIntoImage()
	{
		LOG_ENTER_FUNCTION;

		// Check if video is ok
		if ( !isValid() || !m_newBufferReady)
			return;

		// Set image data (and upload it to the texture)
		m_frameImg.setData( m_internalBuffer, m_videoWidth, m_videoHeight, m_frameImg.getFormat() );
		m_frameImg.updateTexture();

		// Clear new buffer flag
		m_newBufferReady	= false;
		
		LOG_EXIT_FUNCTION;
	}

	/**
	 * Flushes the pending messages on the bus
	 */
	void MediaPlayerGS::flushBusMsg()
	{
		LOG_ENTER_FUNCTION;
	
        // Flush messages on the bus
        GstBus *bus = gst_element_get_bus(GST_ELEMENT(m_pipeline));
        if(bus)
        {
            GstMessage *msg;
            while((msg=gst_bus_pop(bus)) != NULL)
                gst_message_unref(msg);

            gst_object_unref(bus);
            bus = NULL;
        }

		LOG_EXIT_FUNCTION;
	}

	/**
	 * Helper to convert Cing pixel format into Gstreamer video format
	 * @note: it only converts formats supported by Cing Images
	 * @param	cingPixelFormat Cing Pixel format to be translated into Gstreamer pixel format	
	 * @return	Gstreamer video format
	 */
	const char* MediaPlayerGS::cingToGstPixelFormat( GraphicsType cingPixelFormat )
	{
		LOG_ENTER_FUNCTION;
	
		switch( cingPixelFormat )
		{
			case RGB: 
				return GST_VIDEO_CAPS_RGB;
			break;
			
			case RGBA:
				return GST_VIDEO_CAPS_RGBA;
			break;

			case BGR:
				return GST_VIDEO_CAPS_BGR;
			break;

			case BGRA:
				return GST_VIDEO_CAPS_BGRA;
			break;

			case GRAYSCALE:
				LOG_ERROR("MediaPlayerGS Gstreamer 0.10.28 does not support Gray scale videos. Making conversion every frame" );
				m_useGrayScale = true;
				return GST_VIDEO_CAPS_RGB;
			break;
			default: 
				LOG_ERROR( "MediaPlayerGS Could not set the right pixel format (unknown). Defaulting to RGB" );
				break;
		};

		LOG_EXIT_FUNCTION;

		// Not recognized -> default to RGB
		return GST_VIDEO_CAPS_RGB;
	}

	/**
	 * Helper to convert GStreamer video format into Cing pixel format. 
	 * @note: it only converts formats supported by Cing Images
	 * @param	cingPixelFormat Cing Pixel format to be translated into Gstreamer pixel format	
	 * @return	Gstreamer pixel format
	 */
	GraphicsType MediaPlayerGS::gstToCingPixelFormat( const String& gstVideoFormat )
	{
		LOG_ENTER_FUNCTION;
	
		if ( gstVideoFormat == GST_VIDEO_CAPS_RGB )		return RGB;
		if ( gstVideoFormat == GST_VIDEO_CAPS_RGBA )	return RGBA;
		if ( gstVideoFormat == GST_VIDEO_CAPS_BGR )		return BGR;
		if ( gstVideoFormat == GST_VIDEO_CAPS_BGRA )	return BGRA;
		
		LOG_EXIT_FUNCTION;

		// Format not supported by Cing Images -> default to RGB
		return RGB;
	}

	/**
	 * Helper to check if a GStreamer video format matches with a Cing pixel format. 
	 * @note: it only compares formats supported by Cing Images
	 * @param	gstVideoFormat	GStreamer video format
	 * @param	cingVideoFormat Cing video (image) format
	 * @return	true if both formats match. false otherwise
	 */
	bool MediaPlayerGS::checkVideoFormatMatch( GstVideoFormat gstVideoFormat, GraphicsType cingVideoFormat )
	{
		LOG_ENTER_FUNCTION;
	
		if ( (gstVideoFormat == GST_VIDEO_FORMAT_RGB)	&& ( cingVideoFormat == RGB ) )		return true;
		if ( (gstVideoFormat == GST_VIDEO_FORMAT_RGBA)	&& ( cingVideoFormat == RGBA ) )	return true;
		if ( (gstVideoFormat == GST_VIDEO_FORMAT_BGR)	&& ( cingVideoFormat == BGR ) )		return true;
		if ( (gstVideoFormat == GST_VIDEO_FORMAT_BGRA)	&& ( cingVideoFormat == BGRA ) )	return true;

		// NOTE: This is a hack tu support grayscale without gstreamer support, so we convert the buffer every frame
		// TODO: Change this when a GStreamer upadte gives this support
		if ( (gstVideoFormat == GST_VIDEO_FORMAT_RGB)	&& ( cingVideoFormat == GRAYSCALE ) ) return true;

		LOG_EXIT_FUNCTION;

		// Formats do not match
		return false;
	}


} // namespace
