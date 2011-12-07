#include "Cing-Precompiled.h"

#include "SoundManagerOpenAL.h"
#include "common\CommonConstants.h"


namespace Cing
{

	const int SoundManagerOpenAL::CaptureFreq = 44100;
	const int SoundManagerOpenAL::CaptureBufferSize = 44100 * 2 * 10;

	//-------------------------------------------------------------------------------------------------
	SoundManagerOpenAL::SoundManagerOpenAL()
		: base()
		, mCaptureDevice(NULL)
	{
		// Initial position of the listener 
		mPosition[0] = 0.0; 
		mPosition[1] = 0.0; 
		mPosition[2] = 0.0;  
		// Initial velocity of the listener 
		mVelocity[0] = 0.0; 
		mVelocity[1] = 0.0; 
		mVelocity[2] = 0.0;  
		// Initial orientation of the listener = direction + direction up 
		mOrientation[0] = 0.0; 
		mOrientation[1] = 0.0; 
		mOrientation[2] = -1.0; 
		mOrientation[3] = 0.0; 
		mOrientation[4] = 1.0; 
		mOrientation[5] = 0.0; 
	}

	//-------------------------------------------------------------------------------------------------
	SoundManagerOpenAL::~SoundManagerOpenAL()
	{
		end();
	}

	//-------------------------------------------------------------------------------------------------
	bool SoundManagerOpenAL::init()
	{

/*
		// Open an audio device 
		mSoundDevice = alcOpenDevice(NULL);		// TODO ((ALubyte*) "DirectSound3D"); 
		// mSoundDevice = alcOpenDevice( "DirectSound3D" ); 

		// Check for errors 
		if (!mSoundDevice) 
		{ 
			LOG_ERROR("SoundManager::init error : No sound device.\n"); 
			return false; 
		}  

		mSoundContext = alcCreateContext(mSoundDevice, NULL);

		// Check for errors
		if (mSoundContext)
		{ 
			LOG_NORMAL( "Opened '%s' Device", alcGetString( mSoundDevice, ALC_DEVICE_SPECIFIER ) );
		} else {
			LOG_ERROR( "SoundManager::init error : No sound context created.\n");
			alcCloseDevice(mSoundDevice);
			return false; 
		} 
		
		// Make the context current and active 
		alcMakeContextCurrent(mSoundContext);

		if (alcGetError(mSoundDevice) != ALC_NO_ERROR) 
		{ 
			LOG_ERROR("SoundManager::init error : could not make sound context current and active.\n"); 
			return false; 
		}

		if (mInitialized)
			return true;
*/

		// Initialize it using ALUT
		mInitialized = (alutInit(NULL, NULL) == AL_TRUE);

		// Try to initialize the capturing device
		const char* openDeviceList = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
		mCaptureDevice = alcCaptureOpenDevice(openDeviceList, CaptureFreq, AL_FORMAT_MONO16, CaptureBufferSize);

		return mInitialized;
	}

	//-------------------------------------------------------------------------------------------------
	void SoundManagerOpenAL::end()
	{
		//if (mSoundDevice == NULL)
		//	return;

		//// Delete teh sources and buffers
		////...

		//// Destroy the sound context and device 
		//mSoundContext = alcGetCurrentContext(); 
		//mSoundDevice = alcGetContextsDevice(mSoundContext); 
		//alcMakeContextCurrent(NULL);
		//alcDestroyContext(mSoundContext); 
		//if (mSoundDevice)
		//	alcCloseDevice(mSoundDevice);

		//mSoundDevice = NULL;
		//mSoundContext = NULL;

		//printf("SoudManager Destroyed.\n");

		// Release the capture device
		alcCaptureCloseDevice(mCaptureDevice);

		alutExit();
		mInitialized = false;
	}

	//-------------------------------------------------------------------------------------------------
	bool SoundManagerOpenAL::isValid() const
	{
		return mInitialized;
	}

	//-------------------------------------------------------------------------------------------------
	void SoundManagerOpenAL::update()
	{
		// place listener at camera
		alListener3f(AL_POSITION, mPosition[0], mPosition[1], mPosition[2]);

		// Update the orientation
		alListenerfv(AL_ORIENTATION, mOrientation);
	}

}
