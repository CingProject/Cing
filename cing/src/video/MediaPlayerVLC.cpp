/*
This source file is part of the Cing project
For the latest info, see http://www.cing.cc

Copyright (c) 2008-2009 Julio Obelleiro and Jorge Cano

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

#include "MediaPlayerVLC.h"

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/XMLElement.h"

// Framework
#include "framework/UserAppGlobals.h"




namespace Cing
{

	// Static members
	String MediaPlayerVLC::m_pluginsPath = "";


	// VLC Callback functions

	/**
	* @internal
	* Called from VLC when there is a new frame to be stored  
	* @param frameContainer   Contains the image where the frame will be stored by VLC
	* @param pp_ret  Pointer that VLC will use to copy the frame data
	**/
	static void lock( VLCFrameContainer *frameContainer,  void** pp_ret )
	{
		// lock image data access
		frameContainer->mutex.lock();
		*pp_ret = frameContainer->image.getData();
	}

	/**
	* @internal
	* Called from VLC when the frame copy has finished -> we can now use the frame image
	* @param frameContainer   Contains the image where the frame has been stored by VLC
	**/
	static void unlock( VLCFrameContainer *frameContainer )
	{
		// set texture to update in case of draw and unlock the mutex
		frameContainer->image.setUpdateTexture( true );
		frameContainer->mutex.unlock();
	}

	/**
	* @internal
	* Called from VLC when media being played has reached the end
	* @param eventInfo VLC Event information
	* @mediaPlayerVLC  Media player that is playing the media. Should be informed about the event
	**/
	void vlcEventEndOfMedia( const libvlc_event_t *eventInfo, void *mediaPlayerVLC )
	{
		// Report end of file event to media player
		if ( mediaPlayerVLC )
			((MediaPlayerVLC*)mediaPlayerVLC)->endOfFileReached();
	}


	/**
	* Default constructor.
	*/
	MediaPlayerVLC::MediaPlayerVLC():
	m_libvlc      ( NULL ),
		m_media       ( NULL ),
		m_mediaPlayer ( NULL ),
		m_eventManager( NULL ),
		m_videoWidth  ( 0 ),
		m_videoHeight ( 0 ),
		m_loopOnNextUpdate( false ),
		m_bIsValid    ( false )
	{
	}

	/**
	* Default destructor.
	*/
	MediaPlayerVLC::~MediaPlayerVLC()
	{
		end();
	}



	/**
	* Inits the object to make it valid.
	*/
	void MediaPlayerVLC::init()
	{
		// VLC Init

		// Plugins path not loaded yet?
		if ( m_pluginsPath == "" )
		{
			// Load Cing Config file
			XMLElement xml;
			xml.load( "CingConfig.xml" );

			// Get cing data folder (the root is CingConfig)
			if ( xml.isValid() )
			{
				XMLElement pluginsFolder = xml.getChild("VLC_Plugins_Folder");
#if defined(_DEBUG)
				m_pluginsPath = pluginsFolder.getStringAttribute("relativePath_Debug");
#else
				m_pluginsPath = pluginsFolder.getStringAttribute("relativePath_Release");
#endif
			}
			else
			{
				m_pluginsPath = "PluginsVLC";
				LOG( "CingConfig.xml not found in data folder -> using default plugins paths (PluginsVLC in same dir as exe file)" );
			}
		}

		LOG( "VLC plugins path: %s", m_pluginsPath.toChar() );

		// VLC config values (for more info check: http://wiki.videolan.org/VLC_command-line_help)
		char const *vlc_argv[] =
		{
			"--verbose", "0",
			"--plugin-path", m_pluginsPath.toChar(),
			"--ignore-config", //Don't use VLC's config files 
			"--no-video-title-show",
			"--disable-screensaver",
			"--no-osd"
		};
		int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

		// Init VLC Exception handling
		libvlc_exception_init(&m_vlcException);

		// Init VLC
		m_libvlc = libvlc_new(vlc_argc, vlc_argv, &m_vlcException);

		// TODO: need to know when plugins are loaded
		pt::psleep( 100 );
		

		LOG( "MediaPlayer Initialized correcly" );

		checkVLCException();
	}

	/**
	* Loads a movie file
	*/
	void MediaPlayerVLC::load( const char* fileName )
	{
		// Build path to file
		m_fileName = fileName;

		// Check if it a network path
		if ( m_fileName.find( "://" ) != std::string::npos )
			m_filePath = m_fileName;
		
		// It looks like a local path -> check if it exists
		else
		{		
			m_filePath = dataFolder + fileName;
			if ( !fileExists( m_filePath ) )
			{
				LOG_ERROR( "MediaPlayer: File %s not found in data folder.", fileName );
				return;
			}
		}

		// If VLC has not been initialized yet -> initialize it
		if ( !m_libvlc )
			init();

		// Get Video size
		bool isOk = getVideoSize();
		if ( !isOk )
		{
			LOG_ERROR( "MediaPlayer: File %s could not be loaded. Is it in the data folder?", m_fileName.c_str() );
			return;
		}

		// Init the frame container to the video size
		m_frameContainer.image.init( m_videoWidth, m_videoHeight );

		// Config file media options
		char clock[64], cunlock[64], cdata[64];
		char width[32], height[32], pitch[32], chroma[32];

		// Media options
		sprintf(clock,   ":vmem-lock=%lld", (long long int)(intptr_t)lock);
		sprintf(cunlock, ":vmem-unlock=%lld", (long long int)(intptr_t)unlock);
		sprintf(cdata,   ":vmem-data=%lld", (long long int)(intptr_t)&m_frameContainer);
		sprintf(width,   ":vmem-width=%i", m_videoWidth);
		sprintf(height,  ":vmem-height=%i", m_videoHeight);
		sprintf(chroma,  ":vmem-chroma=%s", "RV24" );
		sprintf(pitch,   ":vmem-pitch=%i", m_videoWidth * 3);

		char const *media_argv[] =
		{
			":vout=vmem",
			width,
			height,
			pitch,
			chroma,
			clock,
			cunlock,
			cdata
		};
		int media_argc = sizeof(media_argv) / sizeof(*media_argv);

		// Create new media
		m_media = libvlc_media_new( m_libvlc, m_filePath.c_str(), &m_vlcException );
		checkVLCException();

		// Add media options
		for (int i = 0; i < media_argc; i++ )
		{
			libvlc_media_add_option( m_media, media_argv[i], &m_vlcException );
			checkVLCException();
		}

		// Create the player for this media
		m_mediaPlayer = libvlc_media_player_new_from_media(m_media, &m_vlcException);
		checkVLCException();


		// Get Event manager to manage looping and other relevant events
		m_eventManager = libvlc_media_player_event_manager( m_mediaPlayer, &m_vlcException );

		// Attach to end of media event
		libvlc_event_attach( m_eventManager, libvlc_MediaPlayerEndReached, vlcEventEndOfMedia, this, &m_vlcException );
		checkVLCException();

		LOG( "MediaPlayer: File %s correctly loaded", m_fileName.c_str() );

		// The object is valid when the file is loaded
		m_bIsValid = true;
	}


	/**
	* Releases the object's resources
	*/
	void MediaPlayerVLC::end()
	{
		// Check if already released
		if ( !isValid() )
			return;

		// Stop stream and clean up libVLC
		libvlc_media_player_stop(m_mediaPlayer, &m_vlcException);
		checkVLCException();

		if ( m_eventManager )
			libvlc_event_detach( m_eventManager, libvlc_MediaPlayerEndReached, vlcEventEndOfMedia, this, &m_vlcException );

		if ( m_media )
			libvlc_media_release(m_media);

		if ( m_mediaPlayer )
			libvlc_media_player_release(m_mediaPlayer);

		if ( m_libvlc )
			libvlc_release(m_libvlc);

		// Clear pointers
		m_libvlc      = NULL;
		m_media       = NULL;
		m_mediaPlayer = NULL;
		m_eventManager= NULL;

		// The object is not valid anymore
		m_bIsValid = false;
	}

	/**
	* Updates media playback state
	*/
	void MediaPlayerVLC::update()
	{
		// Check if we need to restart playback
		if ( m_loopOnNextUpdate )
		{
			// Restart (loop) playback
			stop();
			loop();

			// clear the flag
			m_loopOnNextUpdate = false;
		}
	}

	/**
	* Returns true if the media is playing, false otherwise     
	**/
	bool MediaPlayerVLC::isPlaying()
	{
		// Check if video is ok
		if ( !isValid() )
			return false;

		bool result = false;
		if ( libvlc_media_player_is_playing( m_mediaPlayer, &m_vlcException ) == 1 )
			result = true;
		return result;
	}


	/**
	* Returns the location of the play head in seconds (that is, the time it has played)     
	**/
	float MediaPlayerVLC::time()
	{
		// Check if video is ok
		if ( !isValid() )
			return 0;

		return libvlc_media_player_get_time( m_mediaPlayer, &m_vlcException ) / 1000.0f;
	}

	/**
	* Checks if there has been an error in VLC                                                                    
	**/
	void MediaPlayerVLC::checkVLCException()
	{
		// Check for errors
		if(libvlc_exception_raised( &m_vlcException ))
		{
			LOG_ERROR("MediaPlayerVLC Error: VLC Error: %s\n", libvlc_exception_get_message( &m_vlcException ));
		}

		// Clean the exception container
		libvlc_exception_clear( &m_vlcException );
	}


	/**
	* Gets the size of a video
	* TODO: This method is slow because there is no way for now to get the size of a video in VLC
	*       without playing it... so here we play it until we get the video size and then we release the used resources.
	**/
	bool MediaPlayerVLC::getVideoSize()
	{
		// Config file media options
		char const *media_argv[] =
		{
			":no-audio",
			":vout=dummy",
		};
		int media_argc = sizeof(media_argv) / sizeof(*media_argv);

		// Create new media to obtain the size
		if ( m_media )
			libvlc_media_release( m_media );
		m_media = libvlc_media_new( m_libvlc, m_filePath.c_str(), &m_vlcException );
		checkVLCException();

		// Add media options
		for (int i = 0; i < media_argc; i++ )
		{
			libvlc_media_add_option( m_media, media_argv[i], &m_vlcException );
			checkVLCException();
		}

		// Create the player for this media
		if ( m_mediaPlayer) 
			libvlc_media_player_release( m_mediaPlayer );
		m_mediaPlayer = libvlc_media_player_new_from_media(m_media, &m_vlcException);
		checkVLCException();

		// play the media
		libvlc_media_player_play(m_mediaPlayer, &m_vlcException);
		checkVLCException();

		// Query video width until the data becomes available (vlc needs to decode at least one frame)
		m_videoWidth = libvlc_video_get_width( m_mediaPlayer, &m_vlcException );
		m_timer.reset();
		while ( m_videoWidth == 0 && (m_timer.getMilliseconds() < 1000 ) )
		{
			pt::psleep( 100 );
			m_videoWidth = libvlc_video_get_width( m_mediaPlayer, &m_vlcException );
		}

		// Store the data
		if ( m_videoWidth != 0 )
		{
			m_videoDuration = libvlc_media_player_get_length( m_mediaPlayer, &m_vlcException );
			m_videoFps      = libvlc_media_player_get_fps( m_mediaPlayer, &m_vlcException );
			m_videoHeight   = libvlc_video_get_height( m_mediaPlayer, &m_vlcException );
		}
		// Error, video size could not be retreived
		else
		{
			LOG_ERROR( "MediaPlayerVLC Error: Video Size could not be retreived" );
			return false;
		}

		// Stop and release the player
		libvlc_media_release(m_media);
		libvlc_media_player_stop(m_mediaPlayer, &m_vlcException);
		libvlc_media_player_release( m_mediaPlayer );

		// all ok
		return true;
	}

	/**
	* Plays the media with no loop
	**/
	void MediaPlayerVLC::play()
	{
		// Check if video is ok
		if ( !isValid() )
			return;

		// play the media in no loop mode
		m_loop = false;
		libvlc_media_player_play(m_mediaPlayer, &m_vlcException);
		checkVLCException();
	}

	/**
	* Plays the media with no loop
	**/
	void MediaPlayerVLC::loop()
	{
		// Check if video is ok
		if ( !isValid() )
			return;

		m_loop = true;  

		libvlc_media_player_play(m_mediaPlayer, &m_vlcException);  
		checkVLCException();
	}

	/**
	* Plays the media with no loop
	**/
	void MediaPlayerVLC::stop()
	{
		// Check if video is ok
		if ( !isValid() )
			return;

		// play the media
		libvlc_media_player_stop(m_mediaPlayer, &m_vlcException);
		checkVLCException();
	}

	/**
	* Plays the media with no loop
	**/
	void MediaPlayerVLC::pause()
	{
		// Check if video is ok
		if ( !isValid() )
			return;

		// play the media
		libvlc_media_player_pause(m_mediaPlayer, &m_vlcException);
		checkVLCException();
	}

	/**
	* Jumps to a specific location within a movie (specified in seconds)
	* @param whereInSecs Where to jump in the movie (in seconds)
	**/
	void MediaPlayerVLC::jump( float whereInSecs )
	{
		// Check if video is ok
		if ( !isValid() )
			return;

		// Seek specified position
		libvlc_media_player_set_time( m_mediaPlayer, libvlc_time_t(whereInSecs * 1000.0f), &m_vlcException );
		checkVLCException();
	}

	/**
	* Sets the relative playback speed of the movie (
	* Examples: 1.0 = normal speed, 2.0 = 2x speed, 0.5 = half speed
	* @param rate Speed rate to play the movie
	**/
	void MediaPlayerVLC::speed( float rate )
	{
		// Check if video is ok
		if ( !isValid() )
			return;
		
		if ( rate < 0 )
			LOG_ERROR( "Negative rates not supported yet" );
		else
		{
			libvlc_media_player_set_rate( m_mediaPlayer, rate, &m_vlcException );
			checkVLCException();
		}
	}

	/**
	* Toggles the audio mute                                                                    
	**/
	void MediaPlayerVLC::toggleMute()
	{
		// Check if video is ok
		if ( !isValid() )
			return;
		
		libvlc_audio_toggle_mute( m_libvlc, &m_vlcException );
		checkVLCException();
	}

	/**
	* Sets audio volume (0..1)
	* @param volume new volume
	**/
	void MediaPlayerVLC::setVolume( float volume )
	{
		// Check if video is ok
		if ( !isValid() )
			return;
		
		libvlc_audio_set_volume( m_libvlc, (int)(volume * 100), &m_vlcException );
		checkVLCException();
	}


	/**
	* Called from the event manager callback when the media being played has reached the end.
	* It will loop if loop mode is active
	**/
	void MediaPlayerVLC::endOfFileReached()
	{
		// Check if video is ok
		if ( !isValid() )
			return;
		
		// Restart if loop more active
		if ( m_loop )
		{
			// We cannot restart the video during the VLC callback because it gets locked..
			// -> so we set this flag to loop the video on next update
			m_loopOnNextUpdate = true;
		}
	}

} // namespace