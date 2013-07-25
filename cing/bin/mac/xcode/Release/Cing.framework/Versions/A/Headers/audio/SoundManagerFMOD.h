/**
 * InteractiveLib
 * @file: ILSoundManagerFMOD.h
 *
 * Declares funcions common to audio input and output
 * and a class to handle common state (such as library
 * initializatoin/release).
 *
 **/

#ifndef _IL_AUDIO_SHARED_H
#define _IL_AUDIO_SHARED_H

// Precompiled headers
#include "Cing-Precompiled.h"


#include "AudioPrereqs.h"

// Includes
#include <set>

// Common
#include "common/Singleton.h"

// Fmod
#include "Fmod/include/fmod_errors.h"

namespace Cing
{
// Forward declaration
class SoundFMOD;

// Error checking
bool    failedFMODResultCode        ( FMOD_RESULT result );
bool    controledFMODChannelFailure ( FMOD_RESULT result );
const char*   getFmodMessageFromCode      ( FMOD_RESULT result );

// TODO Drivers information
//void ShowOutputDriversInformation ();
//void ShowInputDriversInformation  ();

/**
 * @internal
 * @brief Common objects need by audio input and output.
 *
 **/
class SoundManagerFMOD: public SingletonStatic< SoundManagerFMOD >
{
public:

	// Singleton requirements
	friend class SingletonStatic< SoundManagerFMOD >;

	// Initialization/Release
	FMOD_SYSTEM*    init    ();
	void            end			();

	bool            isValid ( FMOD_SYSTEM* ) const;

	// Update
	void						update();

	// Sounds (needed to release the sounds before the system)
	void registerSound  ( SoundFMOD* soundPtr );
	void unregisterSound( SoundFMOD* soundPtr );

private:
	/// Type for sounds' container
	typedef std::set< SoundFMOD* > SoundsContainer;

	// Construction/Destruction (private to ensure singleton)
	SoundManagerFMOD();
	~SoundManagerFMOD();

	// Attributes
	FMOD_SYSTEM*        m_system;       ///< Variable FMOD_System
	SoundsContainer			m_systemSounds; ///< Container for current loaded sounds
};

} // namespace Cing


#endif // _IL_AUDIO_SHARED_H
