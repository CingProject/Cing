/*
	This source file is part of the Vision project
	For the latest info, see http://www.playthemagic.com/vision

	Copyright (c) 2008 XXX

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

// Includes from Audio
#include "MicroFMOD.h"
#include "SoundManagerFMOD.h"

// Common
#include "common/LogManager.h"

// Extern libs
#include "externLibs/PTypes/include/pasync.h" // for portable sleep

namespace Audio
{

/**
 * @brief Constructor
 *
 **/
MicroFMOD::MicroFMOD() :
  m_sysPtr( 0 ),
  m_sound( 0 )
{

}

/**
 * @brief Destructor. Calls release
 *
 */
MicroFMOD::~MicroFMOD()
{
  // Release if needed
  end();
}

/**
 * @brief Initializes system microphone
 * it also initializes audio system.
 *
 */
void MicroFMOD::init()
{
  // Message
  LOG( "Starting microphone..." );

  // Init audio shared
	m_sysPtr = SoundManagerFMOD::getSingleton().init(); // save a pointer

  // Create a sound for the micro
  createSound();

  // Message
  LOG( "Microphone started..." );
}

/**
 * @brief Releases microphone resources
 * Stops current action if there is any.
 */
void MicroFMOD::end()
{
  // If not initialized, skip
  if( !m_sysPtr )
      return;

  // Stop any action
  stop();

  // Release sound
  destroySound();

  // System pointer is not valid any more
  m_sysPtr = NULL;
}

/**
 * @brief Checks if microphone system is ok
 * If microphone systMm is ok it does nothing. If microphone
 * system was not initialize throws an ILException.
 *
 **/
bool MicroFMOD::isValid() const
{
  return m_sysPtr != NULL;
}

/**
 * @brief Starts microphone capturing.
 * @note: Stops any previous action the microphone
 * was doing.
 * @todo Provide a parameter to reproduce the input (it's quite easy,
 * we just have to avoid calling setvolume(0) )
 *
 **/
void MicroFMOD::startCapturing()
{
  // Check state
	if ( !isValid() )
		init();

  // Stop any previous action
  stop();

  // Ok, sound is ready, let's start
  startRecord();

  /*
      TODO The sleep ensures there's some audio to play (check buffer size)
      From FMOD doc, when creating a sound:
      decodebuffersize
      [in] Optional. Specify 0 to ignore. For streams.
      This determines the size of the double buffer (in PCM samples) that a stream uses.
      Use this for user created streams if you want to determine the size
      of the callback buffer passed to you. Specify 0 to use FMOD's default
      size which is currently equivalent to 400ms of the sound format created/loaded.

      "DSP Plugin Viewer Example" has the following code:
      if (output != FMOD_OUTPUTTYPE_ASIO)
      {
          Sleep(100);
      }

      Try ASIO (needs sepcial driver)
  */
  // Lets fill the buffer
	pt::psleep( 100 );

  // Now start the channel
  startPlayback();

}

/**
 * @brief Stops any capturing or recording action the microphone is
 * currently doing.
 */
void MicroFMOD::stop()
{
  // Not capturing or recording?
  if( !isRunning() )
      return;

  // Check state
	if ( !isValid() )
	{
		LOG_ERROR( "Micro not initialized. Must call init before using the microphone" );
		return;
	}    
	
	// Stop the pseudo-output
  stopPlayback();

  // Stop the input
  stopRecord();
}

/**
 * @brief Checks if microphone is currently capturing/recording
 * @internal Does this affects to line-in ?
 *
 * @return True if micro is recoring or capturing. Otherwise false.
 *
 **/
bool MicroFMOD::isRunning() const
{
  // Check state
	if ( !isValid() )
	{
		LOG_ERROR( "Micro not initialized. Must call init before using the microphone" );
		return false;
	}

  // Get state
  FMOD_BOOL rec;
  FMOD_RESULT result = FMOD_System_IsRecording( m_sysPtr, &rec );

  // If fails
  if( failedFMODResultCode( result ) )
      // Crash app
      LOG_ERROR( getFmodMessageFromCode( result ) );

  return rec != 0;
}

/*
 * @brief Returns the current microphone input level in a range 0..1
 * @return the current microphone input level in a range 0..1
 */
float MicroFMOD::getCurrentLevel() const
{
  // Value to get
  float value = 0;

  // Needs to be running
  if( !isRunning() )
	{
      LOG_ERROR( "Trying to get input from microphone without capturing signal!" );
			return 0.0f;
	}

  // Get the data from the channel (may have some latency)
  // get value, for 1 float, from channel 0 (mono)
  FMOD_RESULT result = FMOD_Channel_GetWaveData( m_channel, &value, 1	, 0 );

  // Check for unrecoverable error
  if( failedFMODResultCode( result ) )
	{
      LOG_ERROR( getFmodMessageFromCode( result ) );
			return 0.0f;
	}

  /*
  This channel should not be lost if the micro doesn't stop it,
  but if you see that it fails when it should not, maybe you
  want to try this code instead (to check for error):
  // If fails
  if( FailedFMODResultCode( result ) ) {

      // If it's due to invalid handler the sound stopped
      if( ControledFMODChannelFailure( result ) ) {
          // Just forget it
          m_channel = 0;
          // And return as no recording
          return false;
      }
      // Ok, the error is not controled
      else {
          // Crash app
          THROW_IL_EXCEPTION( getFmodMessageFromCode( result ) );
      }

  } // failed
  */

  // Return the value
  return fabsf( value );
}

/**
 * @brief Creates a sound to record to
 *
 **/
void MicroFMOD::createSound()
{
  // Already has a sound?
  if( m_sound )
      return; // Then skip

  // Extended information
  FMOD_CREATESOUNDEXINFO exInfo;
  memset(&exInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
  exInfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
  exInfo.numchannels      = 1;
  exInfo.format           = FMOD_SOUND_FORMAT_PCM16;
  exInfo.defaultfrequency = 44100;
  exInfo.length           = exInfo.defaultfrequency * sizeof(short) * exInfo.numchannels * 5;

  // Create sound
  FMOD_MODE mode = FMOD_2D | FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER;
  FMOD_RESULT result = FMOD_System_CreateSound( m_sysPtr, 0, mode, &exInfo, &m_sound );

  // Check for success
  if( failedFMODResultCode( result ) )
      // Failed
      LOG_ERROR( getFmodMessageFromCode( result ) );

}

/**
 * @brief Releases current sound for the microphone.
 * The sound won't be available any more.
 *
 **/
void MicroFMOD::destroySound()
{
  // No sound to release?
  if( !m_sound )
      return; // Then skip

	if ( SoundManagerFMOD::getSingletonPtr() )
	{
		// Try to release the sound
		FMOD_RESULT result = FMOD_Sound_Release( m_sound );

		// If failed to release the sound
		if( failedFMODResultCode( result ) )
				// Message (but do not crash)
				LOG_ERROR( getFmodMessageFromCode( result ) );
	}

  // No sound
  m_sound = 0;

}

void MicroFMOD::startPlayback()
{
  // Needs a sound to be played
  if( !m_sound )
	{
      LOG_ERROR( "Trying to start microphone playback without sound system!" );
			return;
	}

  // Request a channel to play the sound so it can be processed
  FMOD_RESULT result = FMOD_System_PlaySound( m_sysPtr, FMOD_CHANNEL_REUSE, m_sound, FALSE, &m_channel );

  // Check for success
  if( failedFMODResultCode( result ) )
	{
      LOG_ERROR( getFmodMessageFromCode( result ) );
			return;
	}

  // Playback should not be audible (just get the data from mic)
  result = FMOD_Channel_SetVolume( m_channel, 0 );

  // Check for success
  if( failedFMODResultCode( result ) )
      // But dont crash
      LOG_ERROR( getFmodMessageFromCode( result ) );

}

void MicroFMOD::stopPlayback()
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

void MicroFMOD::startRecord()
{
  // Needs a sound to be played
  if( !m_sound )
	{
      LOG_ERROR( "Trying to start recording without sound system initialized!" );
			return;
	}

  // Try to start recording
  FMOD_RESULT result = FMOD_System_RecordStart( m_sysPtr, m_sound, true ); // loop

  // If cannot capture
  if( failedFMODResultCode( result ) )
      // Exception
      LOG_ERROR( getFmodMessageFromCode( result ) );
}

void MicroFMOD::stopRecord()
{
    // Try to stop recording
    FMOD_RESULT result = FMOD_System_RecordStop( m_sysPtr );

    // Check success
    if( failedFMODResultCode( result ) )
        // Message (but do not throw exception)
        LOG_ERROR( getFmodMessageFromCode( result ) );
}

} // namespace Audio

