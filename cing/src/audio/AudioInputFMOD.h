/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#ifndef _IL_AudioInput_FMOD_H
#define _IL_AudioInput_FMOD_H

// Precompiled headers
#include "Cing-Precompiled.h"

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

