/**
* InteractiveLib
* @file: ILSoundFMOD.cpp
*
* Implements the class for sounds for the Interactive Lib
*/

// Precompiled headers
#include "Cing-Precompiled.h"

//#include "windef.h"

// Includes from Audio
#include "SoundFMOD.h"
#include "SoundManagerFMOD.h"

// Common
#include "common/LogManager.h"
#include "common/ResourceManager.h"
#include "common/Exception.h"
#include "common/SystemUtils.h"

// Includes
#include <string>
#include <sstream>

// Ignore warning of converting to bool -> FMOD_BOOL
#if defined(WIN32) && defined(_MSC_VER)
#pragma warning(disable:4800)
#endif

namespace Cing
{

	/// Static member initialization
	FMOD_SYSTEM* SoundFMOD::m_sysPtr = 0;


	/**
	* @internal
	* @brief Constructor. Clears attributes
	*/
	SoundFMOD::SoundFMOD() :
	m_sound( 0 ),
		m_channel( 0 ),
		m_initialFreq( 0 ),
		m_soundVolume( 1.0f ), // Max volume
		m_soundPan( 0.5f ), // No pan
		m_soundSpeed( 1.0f ) // Normal speed
	{

	}

	/**
	* @brief Destructor. Calls release
	*/
	SoundFMOD::~SoundFMOD()
	{
		// Release if needed
		end();
	}

	/**
	* @brief Tries to load the audio file from given path
	* @param [in] filename Path to the file (relative to application)
	*/
	void SoundFMOD::load( const std::string& fileName )
	{
		// Check if the file exist
		std::string completePath = ResourceManager::userDataPath + fileName;
		if ( !fileExists( completePath ) )
		{
			end();
			LOG_ERROR( "Sound %s does NOT exist", completePath.c_str() );
			return;
		}

		// If was initialized previously -> release it first
		if( m_sound )
			end();

		// Init shared audio if needed
		if( !m_sysPtr )
			m_sysPtr = SoundManagerFMOD::getSingleton().init();

		// If was initialized previously -> release it first
		if( m_sound )
			end();

		// Log
		LOG( "Loading sound file '%s'", completePath.c_str() );

		// Create the sound
		if ( createSound( completePath.c_str() ) )
			LOG( "Sound %s successfully loaded", fileName.c_str() );
		else
			LOG( "Error loading sound %s", fileName.c_str() );
	}

	/**
	* @brief Releases sound resources
	* Stops current action if there is any.
	*/
	void SoundFMOD::end()
	{
		// If not initialized, skip
		if( !m_sysPtr )
			return;

		// Stop any action
		stop();

		// Release sound
		destroySound();

		// System pointer is not valid any more
		// NOTE: not setting this pointer to null as it is static and there might be other sounds in use.
		//m_sysPtr = NULL;
	}

	/**
	* @brief Checks if audio system is ok
	* May throw an exception if audio system is not ready for this sound.
	*/
	bool SoundFMOD::isValid() const
	{
		// If not initialized -> Critical exception
		return m_sysPtr != NULL;
	}

	/**
	* @brief Plays the sound.
	* It has to be previously loaded or it will fail.
	* @param [in] loop True to set loop mode on. False to stop playing
	* when the file ends.
	*
	* @sa load
	*/
	void SoundFMOD::play( bool loop )
	{
		// Check state
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to play a Sound that was not properly loaded." );
			return;
		}

		// Stop any previous action
		stop();

		// Now start the channel
		startPlayback( loop );
	}

	/**
	* @brief Pauses the sound.
	* It has to be playing, or it will fail. It will remember
	* the sound's position, so you can play it from where it was
	* paused.
	*
	* @sa Play
	**/
	void SoundFMOD::pause()
	{
		THROW_EXCEPTION( "todo" );
	}

	/**
	* @brief Stops playing the sound.
	* It has to be playing, but won't fail if it is not. It won't remember
	* the current sound's position, so if you call play later,
	* it will start from the beginning.
	*
	* @sa play
	*/
	void SoundFMOD::stop()
	{
		// Is it being played? -> skip
		if( !isPlaying() )
			return;

		// Check system
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to stop a Sound that was not properly loaded." );
			return;
		}

		// Stop the pseudo-output
		stopPlayback();
	}

	/**
	* @brief Checks if microphone is currently capturing/recording
	* @internal Does this affects to line-in ?
	*
	* @return True if micro is recoring or capturing. Otherwise false.
	*
	*/
	bool SoundFMOD::isPlaying() const
	{
		// Check system
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to stop a Sound that was not properly loaded." );
			return false;
		}

		// If has not been created or has no channel
		if( !m_sound || !m_channel )
			// No, not being played
			return false;

		// Get state
		FMOD_BOOL playing;
		FMOD_RESULT result = FMOD_Channel_IsPlaying( m_channel, &playing );

		// If it fails
		if( failedFMODResultCode( result ) ) {

			// If it's due to invalid handler the sound stopped
			if( controledFMODChannelFailure( result ) ) {
				// And return as no playing
				return false;
			}
			// Ok, the error is not controled
			else {
				// Crash app
				LOG_ERROR( getFmodMessageFromCode( result ) );
			}

		} // failed

		// Return the channel's state
		return playing;
	}

	/**
	* @brief Changes the volume for the sound.
	* @param [in] value The new volume value from 0 to 1.
	* 0 is silence, 1 is maximum volume.
	*
	*/
	void SoundFMOD::setVolume( float value )
	{
		// Check Value
		if( value < 0 || value > 1.0f )
			LOG_ERROR( "Bad volumen value for sound" );

		// Check system
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to set the volume to a Sound that was not properly loaded." );
			return;
		}

		// Always change parameter
		m_soundVolume = value;

		// If it's playing, change channel
		if( isPlaying() )
			FMOD_Channel_SetVolume( m_channel, value );
	}

	/**
	* @brief Returns current volume value.
	*
	* @return Current volume value from 0 to 1.
	* 0 is silence, 1 is maximum volume.
	*/
	float SoundFMOD::getVolume() const
	{
		return m_soundVolume;
	}

	/**
	* @brief Changes playback speed.
	* @param [in] value New speed value. You can pass any value.
	* Positive values will increase the speed, while negative
	* values will increase the speed playing the sound backwards!
	* It's a multiplier for the original file frequency.
	*
	*/
	void SoundFMOD::setSpeed( float value )
	{
		// Check system
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to set the speed to a Sound that was not properly loaded." );
			return;
		}

		// Always change parameter
		m_soundSpeed = value;

		// If it's playing, change channel
		if( isPlaying() )
			FMOD_Channel_SetFrequency( m_channel, value*m_initialFreq );
	}

	/**
	* @brief Gets current playback speed.
	*
	* @return Current playback speed value.
	*/
	float SoundFMOD::getSpeed() const
	{
		return m_soundSpeed;
	}

	/**
	* @brief Sets the pan level for the sound.
	* @param [in] value New pan value for this sound, from 0 to 1.
	* 0 is to the left, 1 is to the right.
	*
	*/
	void SoundFMOD::setPan( float value )
	{
		// Check system
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to set the pan to a Sound that was not properly loaded." );
			return;
		}

		// Always change parameter
		m_soundPan = value;

		// If it's playing, change channel
		if( isPlaying() )
			FMOD_Channel_SetPan( m_channel, value );
	}

	/**
	* @brief Gets current pan value.
	*
	* @return Current pan value from 0 to 1.
	* 0 is to the left, 1 is to the right.
	*/
	float SoundFMOD::getPan() const
	{
		return m_soundPan;
	}

	/**
	* @brief Gets current input level from sound wave.
	* The sound has to be playing or it will fail, so be sure
	* you check IsPlaying before calling this method!
	* @param [in] channelNum Offset into multichannel data. Mono
	* channels use 0. Stereo channels use 0 = left, 1 = right.
	* More than stereo use the appropriate index.
	*
	* @return Value from 0 to 1 for the input level of the wave
	*/
	float SoundFMOD::getCurrentLevel( int channelNum /*= 0*/ ) const
	{
		// Has to be playing
		if( !isPlaying() )
		{
			LOG_ERROR( "Tried to get data from not playing sound!" );
			return 0.0f;
		}

		float value = 0;

		// Get the data from the channel (may have some latency)
		// get value, for 1 float, from channel 0 (mono)
		FMOD_RESULT result = FMOD_Channel_GetWaveData( m_channel, &value, 1, channelNum );

		// Check for unrecoverable error
		if( failedFMODResultCode( result ) )
			LOG_ERROR( getFmodMessageFromCode( result ) );

		// The value
		return fabsf( value );
	}

	unsigned int SoundFMOD::getDuration( ) const
	{
		unsigned int value = 0;

		// Check system
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to get duration of a Sound that was not properly loaded." );
			return 0;
		}
		// Get the data from the sound
		FMOD_RESULT result = FMOD_Sound_GetLength( m_sound, &value, FMOD_TIMEUNIT_MS );

		// Check for unrecoverable error
		if( failedFMODResultCode( result ) )
			LOG_ERROR( getFmodMessageFromCode( result ) );

		// The value
		return value;
	}

	/**
	* @brief Creates the fmod sound from filename
	* @param [in] filename Path to filename to load
	*
	*/
	bool SoundFMOD::createSound( const char* filename )
	{
		// Already has a sound? ->Do not allow it!
		if( m_sound )
		{
			LOG_ERROR( "Tried to reload a blocked sound" );
			return false;
		}

		// Create sound
		FMOD_MODE mode = FMOD_SOFTWARE | FMOD_2D; // TODO Check this mode
		FMOD_RESULT result = FMOD_System_CreateSound( m_sysPtr, filename, mode, 0, &m_sound );

		// Check for success
		if( failedFMODResultCode( result ) ) {
			// Better with the filename
			std::string message = getFmodMessageFromCode( result );
			message += "'";
			message += filename;
			message += "'";
			// Failed
			LOG_ERROR( message.c_str() );
			return false;
		}

		// Register into audio shared
		SoundManagerFMOD::getSingleton().registerSound( this );

		return true;

	}

	/**
	* @brief Releases current sound for the microphone.
	* The sound won't be available any more.
	*
	*/
	void SoundFMOD::destroySound()
	{
		// No sound to release?
		if( !m_sound  || !SoundManagerFMOD::getSingleton().isValid(m_sysPtr) )
			return; // Then skip

		// Unregister from audio shared
		if ( SoundManagerFMOD::getSingletonPtr() )
		{
			SoundManagerFMOD::getSingletonPtr()->unregisterSound( this );

			// Try to release the sound
			FMOD_RESULT result = FMOD_Sound_Release( m_sound );

			// If failed to release the sound
			if( failedFMODResultCode( result ) )
				LOG_ERROR( getFmodMessageFromCode( result ) );
		}

		// No sound
		m_sound = NULL;
	}

	/**
	* @brief Request a channel for the sound and plays it
	* @param [in] loop Loop mode. True to loop forever, false to play once
	*
	*/
	void SoundFMOD::startPlayback( bool loop )
	{
		// Needs a sound to be played
		if( !m_sound )
		{
			LOG_ERROR( "Could not play sound!" ); // TODO Spew sound name!
			return;
		}

		// Loop?
		// TODO (provide access to bidirectional loop)
		FMOD_Sound_SetMode( m_sound, loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF );

		// Request a channel to play the sound so it can be processed
		FMOD_RESULT result = FMOD_System_PlaySound( m_sysPtr, FMOD_CHANNEL_FREE, m_sound, (FMOD_BOOL)false, &m_channel );

		// Check for success
		if( failedFMODResultCode( result ) )
		{
			LOG_ERROR( getFmodMessageFromCode( result ) );
			return;
		}

		// Check for success
		if( failedFMODResultCode( result ) )
			LOG_ERROR( getFmodMessageFromCode( result ) );

		// If frequency has not been set, get it now
		if( m_initialFreq == 0 )
		{
			// Get frequency
			FMOD_Channel_GetFrequency( m_channel, &m_initialFreq );
			// Check for success
			if( failedFMODResultCode( result ) )
			{
				// Better with the filename
				std::string message = "Could not get sound frequency. ";
				message += getFmodMessageFromCode( result );
				// Failed
				LOG_ERROR( message.c_str() );
			}
		}

		// Update parameters
		setSpeed( getSpeed() );
		setPan( getPan() );
		setVolume( getVolume() );
	}

	/**
	* @brief Tries to stop the channel associated to this sound
	*
	*/
	void SoundFMOD::stopPlayback()
	{
		// No channel to release?
		if( !m_channel )
			return; // Then skip

		// Try yo stop the channel
		FMOD_RESULT result = FMOD_Channel_Stop( m_channel );

		// If failed to release the sound
		if( failedFMODResultCode( result ) )
			// Message (but do not crash)
			LOG_ERROR( getFmodMessageFromCode( result ) );

		// Let's free the channel I can request a new one later
		m_channel = 0;
	}
} // namespace Cing

