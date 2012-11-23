/*
	This source file is part of the Cing project
	For the latest info, see http://www.cing.cc

	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

#ifndef _SOUND_FMOD_H
#define _SOUND_FMOD_H

// Precompiled headers
#include "Cing-Precompiled.h"


#include "BaseSound.h"
#include "AudioPrereqs.h"

// External Libs


namespace Cing
{

/**
 * @internal
 * Implements the class for sounds using FMOD library. Gives
 * access to one sound that may be played, paused and stopped
 * any time.
 *
 **/
class SoundFMOD: public	BaseSound
{
public:
	// @interal Provides access to Release method
	friend class SoundManagerFMOD;

	// Constructor/Destructor
			SoundFMOD   ();
	virtual ~SoundFMOD  ();

  // Actions
	void	load		( const std::string& fileName );
	void	play		( bool loop )           ;
	void	pause		()                      ;
	void	stop		()                      ;
	bool	isPlaying	()                 const;
	void	end			();

	bool	isValid		() const;
	bool	isLoaded	() const { return m_sound != 0 ; }


  // Parameters
	void	setVolume   ( float value ) ;
	float	getVolume	()              const;
	void	setSpeed	( float value ) ;
	float	getSpeed	()              const;
	void	setPan		( float value ) ;
	float	getPan		()              const;

  // Get info from played sound
  float				getCurrentLevel	( int channelNum = 0 ) const;
	unsigned int	getDuration		() const;

private:

  // Sound
  bool createSound    ( const char* filename );
  void destroySound   ();

  // Playback
  void startPlayback   ( bool loop );
  void stopPlayback    ();

	// Static attributes
	static FMOD_SYSTEM  *m_sysPtr;  ///< Save a shared copy to the audio system

  // Attributes
   FMOD_SOUND*			m_sound;   ///< Sound to be play
   FMOD_CHANNEL*		m_channel; ///< Channel where the sound is being played

   float    			m_soundVolume;  ///< Because fmod handles it by channel instead of sound
   float    			m_soundPan;     ///< Because fmod handles it by channel instead of sound
   float    			m_soundSpeed;   ///< Because fmod handles it by channel instead of sound

   float    			m_initialFreq;  ///< Initial file frequency

}; // class SoundFMOD


} // namespace Cing


#endif // _IL_SOUND_FMOD_H

