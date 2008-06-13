/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

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
#include "SoundManagerFMOD.h"
#include "MicroFMOD.h"
#include "SoundFMOD.h"

// Common
#include "common/LogManager.h"

// Includes
#include <sstream>


/// Audio functionality (load and play sounds, get micro input...etc)
namespace Audio
{

/**
 * @internal
 * @brief Check result code for errorrs
 * Returns true if code represents any error.
 * @param [in] result Code to check for errors
 *
 * @return True if the code represents any error
 **/
bool failedFMODResultCode( FMOD_RESULT result )
{
  return result != FMOD_OK;
}

/**
 * @internal
 * @brief Check result code
 * Returns true if code represents an error related with
 * an invalid or stolen handler. This is due to the possibility
 * to lose the channel you are using because the sound ends.
 *
 * @return True if the code represents an error related to lost
 * channel
 **/
bool controledFMODChannelFailure( FMOD_RESULT result )
{
  // Two errors allowed in some cases
  return (result == FMOD_ERR_INVALID_HANDLE) ||
         (result == FMOD_ERR_CHANNEL_STOLEN);
}

/**
 * @internal
 * Returns a string with the error description
 *
 * @return Returns a string with the error description
 **/
char* getFmodMessageFromCode( FMOD_RESULT result )
{
   return FMOD_ErrorString(result);
}

/**
 * @brief Constructor. Defaults parameters to uninitialized
 *
 **/
SoundManagerFMOD::SoundManagerFMOD() :
    m_system( 0 )
{

}

/**
 * @brief Destructor. Releases audio system if initialized
 *
 **/
SoundManagerFMOD::~SoundManagerFMOD()
{
    // release
    end();
}

/**
 * @brief Initializes FMOD system.
 * Throws an exception if anything fails.
 *
 * @return Returns a pointer to the fmod shared audio
 * system if could be successfully initialized.
 **/
FMOD_SYSTEM* SoundManagerFMOD::init()
{
  // If already initialized, skip
  if( m_system )
      return m_system;

  // Some info
  LOG( "Starting shared audio subsystem..." );

  // Try to open FMOD
  FMOD_RESULT result = FMOD_System_Create( &m_system );
  // Check for unrecoverable error
  if( failedFMODResultCode( result ) )
	{
      LOG_ERROR( getFmodMessageFromCode( result ) );
			return NULL;
	}

  // Lets continue, now checking version
  unsigned int version = FMOD_VERSION;
  result = FMOD_System_GetVersion( m_system, &version );
  // Check for unrecoverable error
  if( failedFMODResultCode( result ) )
	{
      LOG_ERROR( getFmodMessageFromCode( result ) );
			return NULL;
	}

  // Should be equal to compiled version
  if( version != FMOD_VERSION )
	{
		LOG_ERROR( "Invalid FMOD library version. Check DLL file." );
	}

  // TODO Revise these parameters (set to default)
  result = FMOD_System_Init( m_system, 32, FMOD_INIT_NORMAL, 0 ); // Initialize FMOD.
  // Check for unrecoverable error
  if( failedFMODResultCode( result ) )
	{
      LOG_ERROR( getFmodMessageFromCode( result ) );
			return NULL;
	}

  // Ok message
  LOG( "Shared audio subsystem successfully initialized." );
  return m_system;
}

/**
 * @brief Checks if audio system is ok
 * If audio system is ok it does nothing. If audio
 * system was not initialize throws an ILException. It also
 * checks if the audio pointer that has been passed is
 * the shared one (it would fail if someone releases shared
 * audio resources and uses the pointer later)
 * @param systemPtr System pointer to check if it's the shared one
 *
 **/
bool SoundManagerFMOD::isValid( FMOD_SYSTEM* systemPtr ) const
{
  // If not initialized
  if( !m_system )
	{
      LOG( "Shared audio system has not been initialized." );
			return false;
	}
  // If not valid shared pointer
  if( systemPtr != m_system )
	{
      LOG_ERROR( "Corrupted audio system. Shared resources have been corrupted somehow." );
			return false;
	}

	return true;
}

/**
 * @brief Releases audio resources from fmod
 * The audio api won't be usable after calling
 * this method.
 *
 **/
void SoundManagerFMOD::end()
{
    // If theres no system, skip release
    if( !m_system )
        return;

    // Messsage
    // TODO Leads to error. Logger has been probably released
    // PrintOnConsole( "Releasing shared audio resources..." );

		// Clear the container (note: we cannot call here end() for the sounds, because we would
		// need to iterate through the m_systemSounds container, and sounds, call the unregisterSound during their end()
		// which modifies the m_systemSounds container. BE CAREFULL
		m_systemSounds.clear();
	
    // Try to release (show any error releasing)
    FMOD_RESULT result = FMOD_System_Release( m_system );
    if( failedFMODResultCode( result ) ) {
        // Show message
        LOG_ERROR( getFmodMessageFromCode( result ) );
    }
    m_system = 0; // done

    // Bye!
    // TODO Leads to error. Logger has been probably released
    // PrintOnConsole( "Shared audio resources released" );

}

/**
 * @brief Registers a new sound so it can be released before
 * the audio system does.
 * @param [in] soundPtr Pointer for the sound to register
 *
 **/
void SoundManagerFMOD::registerSound( SoundFMOD* soundPtr )
{
    // If already on the container
    if( m_systemSounds.find( soundPtr ) != m_systemSounds.end() )
		{
        // Major error
        LOG_ERROR( "Tried to register a sound that was already registered" );
				return;
		}

    // Insert into the container
    m_systemSounds.insert( soundPtr );
}

/**
 * @brief Unregisters a sound from the container so it is not called
 * to release when system is.
 * @param [in] soundPtr Pointer for the sound to unregister
 *
 **/
void SoundManagerFMOD::unregisterSound( SoundFMOD* soundPtr )
{
    // Search
    SoundsContainer::iterator it = m_systemSounds.find( soundPtr );

    // If din't find it
    if( it == m_systemSounds.end() )
		{
        LOG_ERROR( "Couldn't find sound to unregister!" );
				return;
		}

    // Remove from the container
    m_systemSounds.erase( soundPtr );
}

} // namespace Audio
