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

