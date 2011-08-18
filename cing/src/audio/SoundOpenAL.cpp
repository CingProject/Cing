#include "Cing-Precompiled.h"

#include "common/LogManager.h"
#include "common/ResourceManager.h"

#include "SoundOpenAL.h"
#include "SoundManagerOpenAL.h"


namespace Cing
{

	ALfloat SoundOpenAL::sPosition[3] = { 0.0f, 0.0f, 0.0f };
	ALfloat SoundOpenAL::sVelocity[3] = { 0.0f, 0.0f, 0.0f };

	const float SoundOpenAL::PosPanFactor = 2.0f;


	//-------------------------------------------------------------------------------------------------
	SoundOpenAL::SoundOpenAL()
		: bufferId(AL_NONE)
		, sourceId(AL_NONE)
		, m_isValid(false)
	{
	}

	//-------------------------------------------------------------------------------------------------
	SoundOpenAL::~SoundOpenAL()
	{
		end();
	}

	//-------------------------------------------------------------------------------------------------
	void SoundOpenAL::end()
	{
		// check we actually have something to release
		if (bufferId == AL_NONE)
			return;

		// Stop the sound
		stop();
	}


	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::load(const char* filename)
	{
		if (!SoundManagerOpenAL::getSingleton().isValid())
		{
			SoundManagerOpenAL::getSingleton().init();
		}

		// Get the complete path for loading the file
		std::string completePath = ResourceManager::userDataPath + filename;

		// load the sound using FreeALUT
		bufferId = alutCreateBufferFromFile(completePath.c_str());
		if (bufferId == AL_NONE)
		{
			LOG_ERROR("Sound %s couldn't be loaded", completePath.c_str());
			LOG_ERROR("Error: %s", alutGetErrorString(alutGetError()));
			m_isValid = false;
			return;
		}else{
			m_isValid = true;
		}
	}

	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::play(bool loop)
	{
		if ( m_isValid )
		{	
			// stop it if already playing
			if (sourceId != AL_NONE)
				stop();

			// and play it again
			alGenSources(1, &sourceId);
			alSourcei(sourceId, AL_BUFFER, bufferId);
			alSourcefv(sourceId, AL_POSITION, sPosition);
			alSourcefv(sourceId, AL_VELOCITY, sVelocity);
			alSourcef(sourceId, AL_PITCH, 1.0f);
			alSourcef(sourceId, AL_GAIN, 1.0f);
			alSourcei(sourceId, AL_LOOPING, loop ? 1 : 0);
			alSourcePlay(sourceId);
		}
	}

	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::pause()
	{
		alSourcePause(sourceId);
	}

	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::stop()
	{
		alSourceStop(sourceId);
		sourceId = AL_NONE;
	}


	//-------------------------------------------------------------------------------------------------
    bool SoundOpenAL::isPlaying() const
	{
		ALint status;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &status);

		return (status == AL_PLAYING);
	}

	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::setVolume(float value)
	{
		alSourcef(sourceId, AL_GAIN, value);
	}

	//-------------------------------------------------------------------------------------------------
    float SoundOpenAL::getVolume() const
	{
		float volume;
		alGetSourcef(sourceId, AL_GAIN, &volume);

		return volume;
	}

	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::setSpeed(float value)
	{
		alSourcef(sourceId, AL_PITCH, value);
	}

	//-------------------------------------------------------------------------------------------------
    float SoundOpenAL::getSpeed() const
	{
		float speed;
		alGetSourcef(sourceId, AL_PITCH, &speed);

		return speed;
	}

	//-------------------------------------------------------------------------------------------------
    void SoundOpenAL::setPan(float value)
	{
		ALfloat x = PosPanFactor * 2.0f * (value - 0.5f);
		ALfloat pos[3] = { x, 0.0f, 0.0f };
		alSourcefv(sourceId, AL_POSITION, pos);
	}

	//-------------------------------------------------------------------------------------------------
    float SoundOpenAL::getPan() const
	{
		ALfloat pos[3] = { 0.0f, 0.0f, 0.0f };
		alGetSourcefv(sourceId, AL_POSITION, pos);

		float pan = (pos[0] / (2.0f * PosPanFactor)) + 0.5f;
		return pan;
	}

	//-------------------------------------------------------------------------------------------------
    float SoundOpenAL::getCurrentLevel(int channelNum) const
	{
		assert(!"Not yet implemented!");
		return 0.0f;
	}

}
