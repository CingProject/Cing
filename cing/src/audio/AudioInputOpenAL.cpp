#include "Cing-Precompiled.h"

#include "common/LogManager.h"
#include "common/ResourceManager.h"

#include "AudioInputOpenAL.h"
#include "SoundManagerOpenAL.h"


namespace Cing
{

	//-------------------------------------------------------------------------------------------------
	AudioInputOpenAL::AudioInputOpenAL()
		: mRecordingDevice(NULL)
		, mBuffer(NULL)
	{
	}

	//-------------------------------------------------------------------------------------------------
	AudioInputOpenAL::~AudioInputOpenAL()
	{
		end();
	}

	//-------------------------------------------------------------------------------------------------
	void AudioInputOpenAL::end()
	{
		if (mBuffer != NULL)
		{
			delete[] mBuffer;
			mBuffer = NULL;
		}
	}


	//-------------------------------------------------------------------------------------------------
	void AudioInputOpenAL::init()
	{
		if (!SoundManagerOpenAL::getSingleton().isValid())
			SoundManagerOpenAL::getSingleton().init();

		mRecordingDevice = SoundManagerOpenAL::getSingleton().GetCaptureDevice();

		if (!mRecordingDevice)
		{
			LOG_ERROR("The Capturing device is not valid!");
			return;
		}

		mBuffer = new ALbyte[SoundManagerOpenAL::CaptureBufferSize];
	}

	//-------------------------------------------------------------------------------------------------
	void AudioInputOpenAL::startCapturing()
	{
		if (!mRecordingDevice)
		{
			LOG_ERROR("The Capturing device is not valid!");
			return;
		}

		alcCaptureStart(mRecordingDevice);
	}

	//-------------------------------------------------------------------------------------------------
	void AudioInputOpenAL::stop()
	{
		alcCaptureStop(mRecordingDevice);

		// Get the number of samples
		alcGetIntegerv(mRecordingDevice, ALC_CAPTURE_SAMPLES, 4, &mNumSamples);
		alcCaptureSamples(mRecordingDevice, (ALCvoid*)mBuffer, mNumSamples);
	}

	//-------------------------------------------------------------------------------------------------
	float AudioInputOpenAL::getCurrentLevel() const
	{
		assert(!"Not yet implemented!");
		return 0.0f;
	}

}
