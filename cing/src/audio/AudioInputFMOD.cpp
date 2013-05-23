/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

// Precompiled headers
#include "Cing-Precompiled.h"

// Includes from Audio
#include "AudioInputFMOD.h"
#include "SoundManagerFMOD.h"

// Common
#include "common/LogManager.h"

// Extern libs
#undef nil
#include "PTypes/include/pasync.h" // for portable sleep

namespace Cing
{

/**
 * @brief Constructor
 *
 **/
AudioInputFMOD::AudioInputFMOD() :
  m_sysPtr( 0 ),
  m_sound( 0 )
{

}

/**
 * @brief Destructor. Calls release
 *
 */
AudioInputFMOD::~AudioInputFMOD()
{
  // Release if needed
  end();
}

/**
 * @brief Initializes system AudioInputphone
 * it also initializes audio system.
 *
 */
void AudioInputFMOD::init()
{
  // Message
  LOG( "Starting AudioInputphone..." );

  // Init audio shared
	m_sysPtr = SoundManagerFMOD::getSingleton().init(); // save a pointer

  // Create a sound for the AudioInput
  createSound();

  // Message
  LOG( "AudioInputphone started..." );
}

/**
 * @brief Releases AudioInputphone resources
 * Stops current action if there is any.
 */
void AudioInputFMOD::end()
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
 * @brief Checks if AudioInputphone system is ok
 * If AudioInputphone systMm is ok it does nothing. If AudioInputphone
 * system was not initialize throws an ILException.
 *
 **/
bool AudioInputFMOD::isValid() const
{
  return m_sysPtr != NULL;
}

/**
 * @brief Starts AudioInputphone capturing.
 * @note: Stops any previous action the AudioInputphone
 * was doing.
 * @todo Provide a parameter to reproduce the input (it's quite easy,
 * we just have to avoid calling setvolume(0) )
 *
 **/
void AudioInputFMOD::startCapturing()
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
 * @brief Stops any capturing or recording action the AudioInputphone is
 * currently doing.
 */
void AudioInputFMOD::stop()
{
  // Not capturing or recording?
  if( !isRunning() )
      return;

  // Check state
	if ( !isValid() )
	{
		LOG_ERROR( "AudioInput not initialized. Must call init before using the AudioInputphone" );
		return;
	}

	// Stop the pseudo-output
  stopPlayback();

  // Stop the input
  stopRecord();
}

/**
 * @brief Checks if AudioInputphone is currently capturing/recording
 * @internal Does this affects to line-in ?
 *
 * @return True if AudioInput is recoring or capturing. Otherwise false.
 *
 **/
bool AudioInputFMOD::isRunning() const
{
  // Check state
	if ( !isValid() )
	{
		LOG_ERROR( "AudioInput not initialized. Must call init before using the AudioInputphone" );
		return false;
	}

  // Get state
  FMOD_BOOL rec;
  FMOD_RESULT result = FMOD_System_IsRecording( m_sysPtr, 0, &rec );

  // If fails
  if( failedFMODResultCode( result ) )
      // Crash app
      LOG_ERROR( getFmodMessageFromCode( result ) );

  return rec != 0;
}

/*
 * @brief Returns the current Audio input level in a range 0..1
 * @return the current Audio input level in a range 0..1
 */
float AudioInputFMOD::getCurrentLevel() const
{
  // Value to get
  float value = 0;

  // Needs to be running
  if( !isRunning() )
	{
      LOG_ERROR( "Trying to get input from AudioInputphone without capturing signal!" );
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
  This channel should not be lost if the AudioInput doesn't stop it,
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
void AudioInputFMOD::createSound()
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
 * @brief Releases current sound for the AudioInputphone.
 * The sound won't be available any more.
 *
 **/
void AudioInputFMOD::destroySound()
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

void AudioInputFMOD::startPlayback()
{
  // Needs a sound to be played
  if( !m_sound )
	{
      LOG_ERROR( "Trying to start AudioInputphone playback without sound system!" );
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

void AudioInputFMOD::stopPlayback()
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

void AudioInputFMOD::startRecord()
{
  // Needs a sound to be played
  if( !m_sound )
	{
      LOG_ERROR( "Trying to start recording without sound system initialized!" );
			return;
	}

  // Try to start recording
  FMOD_RESULT result = FMOD_System_RecordStart( m_sysPtr, 0, m_sound, true ); // loop

  // If cannot capture
  if( failedFMODResultCode( result ) )
      // Exception
      LOG_ERROR( getFmodMessageFromCode( result ) );
}

void AudioInputFMOD::stopRecord()
{
    // Try to stop recording
    FMOD_RESULT result = FMOD_System_RecordStop( m_sysPtr, 0 );

    // Check success
    if( failedFMODResultCode( result ) )
        // Message (but do not throw exception)
        LOG_ERROR( getFmodMessageFromCode( result ) );
}

} // namespace Cing

