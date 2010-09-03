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

#ifndef _IL_AudioInput_FMOD_H
#define _IL_AudioInput_FMOD_H

#include "BaseAudioInput.h"
#include "AudioPrereqs.h"
#include "common/MathUtils.h"
#include "vector"
namespace Cing
{

	/**
	* @internal
	* \class AudioInputFMOD
	* Implements behavior to handle system's AudioInputphone
	* using fmod library.
	*
	* @todo Current version uses playback to get input
	* information. May be implemented getting the information
	* directly from sound source, instead of channel. Revise
	* DoStartPlayback and DoStopPlayback (may not be needed if you
	* can get the info from DoStartRecord)
	*
	**/
	class AudioInputFMOD : public BaseAudioInput
	{
	public:
		// Constructor/Destructor
		AudioInputFMOD   ();
		virtual ~AudioInputFMOD  ();

		// Init / end
		void init   ();
		void end		();

		bool isValid() const;

		// Actions
		void startCapturing ();
		void stop           ();
		bool isRunning      () const;   // TODO Move to AudioInput interface

		// Get data
		float					getCurrentLevel() const;

		int						getSamples     () { return m_samples;    };
		int						getSampleRate  () { return m_sampleRate; };
		FMOD_SOUND*		getSound       () { return m_sound;      };
		FMOD_CHANNEL*	getChannel     () { return m_channel;    };

		// Set methods

	private:

		// Sound
		void createSound    ();
		void destroySound   ();

		// Playback
		void startPlayback   ();
		void stopPlayback    ();

		// Record
		void startRecord    ();
		void stopRecord     ();

		// Attributes
		FMOD_SYSTEM*    m_sysPtr;      ///< Owns a copy to shared system (should be synchronized)
		FMOD_SOUND*     m_sound;       ///< Sound when capturing/recording
		FMOD_CHANNEL*   m_channel;     ///< Use a channel for processing
		int             m_samples;     ///< Number of time samples
		int             m_sampleRate;  ///< Sample rate

	}; // class AudioInputFMOD

} // namespace Cing

#endif // _IL_AudioInput_FMOD_H

