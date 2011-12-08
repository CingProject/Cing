#pragma once
#ifndef __CING_AUDIO_AUDIOINPUTOPENAL_H__
#define __CING_AUDIO_AUDIOINPUTOPENAL_H__

#include "OpenAL\include\al.h"
#include "OpenAL\include\alc.h"

#include "BaseAudioInput.h"
#include "SoundManagerOpenAL.h"


namespace Cing
{

	//-------------------------------------------------------------------------------------------------
	// Implements the BaseAudioInput interface using OpenAL. 
	//
	// Note: This class records a sound into a buffer but there's no interface in the SoundOpenAL to
	// play that sound. There should be an interface to create a sound based on a buffer and the size
	// (number of samples) of that buffer in SoundOpenAL for this to make sense
	//-------------------------------------------------------------------------------------------------
	class AudioInputOpenAL : public BaseAudioInput
	{
	/// Constructor & Destructor
	public:
		AudioInputOpenAL();
		virtual ~AudioInputOpenAL();

		void end();

		int GetNumSamples() const { return mNumSamples; }
		ALbyte* GetBuffer() { return mBuffer; }

	/// BaseAudioInput
	public:

		/**
		 * @brief Forces AudioInput initialization by user.
		 * AudioInput auto initializes when first action is
		 * requested. However we shall give the user a way to
		 * init it, as it may take a while to init the driver.
		 * If the user wants a lighting action response, should
		 * call this method on his initialization code.
		 **/
		virtual void init();

		// Actions

		/**
		* @brief Starts capturing sound from AudioInputphone
		**/
		virtual void startCapturing();

		/**
		 * @brief Stops current AudioInputphone action (capturing
		 * or recording) if there is any.
		 **/
		virtual void stop();

		/**
		 * @brief Gets current input level from AudioInputphone wave
		 *
		 * @return Value from 0 to 1 for the input level of the wave
		 **/
		 virtual float getCurrentLevel() const;


	/// Attributes
	private:
		ALCdevice* mRecordingDevice;
		int mNumSamples;
		ALbyte* mBuffer;
	};

}

#endif	// __CING_AUDIO_AUDIOINPUTOPENAL_H__
