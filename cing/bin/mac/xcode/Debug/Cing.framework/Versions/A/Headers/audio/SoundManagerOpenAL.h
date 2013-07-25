#pragma once
#ifndef __CING_AUDIO_SOUNDMANAGEROPENAL_H__
#define __CING_AUDIO_SOUNDMANAGEROPENAL_H__

#include <set>

#include "OpenAL\include\al.h"
#include "OpenAL\include\alc.h"
#include "FreeALUT\include\AL\alut.h"

#include "common/Singleton.h"


namespace Cing
{

	//-------------------------------------------------------------------------------------------------
	// Implements the sound manager for the OpenAL libraries. It takes care of the common stuff to 
	// sound, such as creating the sound/audio input objects, and library initialization/tear down.
	//-------------------------------------------------------------------------------------------------
	class SoundManagerOpenAL : public SingletonStatic<SoundManagerOpenAL>
	{
		typedef SingletonStatic<SoundManagerOpenAL> base;

		// Singleton requirements
		friend class SingletonStatic<SoundManagerOpenAL>;

	public:
		SoundManagerOpenAL();
		virtual ~SoundManagerOpenAL();

		// Initialization/Tear down
		bool init();
		void end();

		bool isValid() const;

		// Update
		void update();

		// Accessors
		ALCdevice* GetCaptureDevice() { return mCaptureDevice; }

	private:
		ALCdevice* mCaptureDevice;
		ALCdevice* mSoundDevice;
		ALCcontext* mSoundContext;

		bool mInitialized;

		// Initial position, velocity and orientation of the listener (there's no support for changing this at the moment)
		ALfloat mPosition[3];
		ALfloat mVelocity[3];
		ALfloat mOrientation[6];

	public:
		const static int CaptureFreq;
		const static int CaptureBufferSize;
	};

}

#endif	// __CING_AUDIO_SOUNDMANAGEROPENAL_H__
