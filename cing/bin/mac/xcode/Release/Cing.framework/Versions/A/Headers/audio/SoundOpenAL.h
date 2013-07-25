#pragma once
#ifndef __CING_AUDIO_SOUNDOPENAL_H__
#define __CING_AUDIO_SOUNDOPENAL_H__

#include "BaseSound.h"

#include "FreeALUT\include\AL\alut.h"


namespace Cing
{

	//-------------------------------------------------------------------------------------------------
	// Implements the BaseSound interface using OpenAL. It's the equivalent to SoundFMOD but for
	// OpenAL (which is a free library as opposed to FMOD).
	//-------------------------------------------------------------------------------------------------
	class SoundOpenAL : public BaseSound
	{
	public:
		SoundOpenAL();
		virtual ~SoundOpenAL();


		// Release the sound
		void end();

	/// BaseSound interface
	public:
        /**
         * @brief Loads a sound file.
				 * If already had a file loaded, it will be discarded, loading
				 * the new file.
         **/
        virtual void load(const char* filename);

        /**
         * @brief Plays the sound.
         * It has to be previously loaded or it will fail.
         **/
        virtual void play(bool loop);

        /**
         * @brief Pauses the sound.
         * It has to be playing, or it will fail. It will remember
         * the sound's position, so you can play it from where it was
         * paused.
         **/
        virtual void pause();

        /**
         * @brief Stops playing the sound.
         * It has to be playing, or it will fail. It won't remember
         * the current sound's position, so if you call play later,
         * it will start from the beginning.
         **/
        virtual void stop();


        /**
         * @brief Tells whether the sound is currently being played
         * or not.
         **/
        virtual bool isPlaying() const;

        // Parameters

        /**
         * @brief Changes the volume for the sound.
         * @param [in] value The new volume value from 0 to 1.
         * 0 is silence, 1 is maximum volume.
         **/
        virtual void setVolume(float value);

        /**
         * @brief Returns current volume value.
         */
        virtual float getVolume() const;

        /**
         * @brief Changes playback speed.
         * @param [in] value New speed value. You can pass any value.
         * Positive values will increase the speed, while negative
         * values will increase the speed playing the sound backwards!
         **/
        virtual void setSpeed(float value);

        /**
         * @brief Gets current playback speed.
         **/
        virtual float getSpeed() const;

        /**
         * @brief Sets the pan level for the sound.
         * @param [in] value New pan value for this sound, from 0 to 1.
         * 0 is to the left, 1 is to the right.
         **/
        virtual void setPan(float value);

        /**
         * @brief Gets current pan value.
         **/
        virtual float getPan() const;

        /**
         * @brief Gets current input level from sound wave.
         * The sound has to be playing or it will fail, so be sure
         * you check IsPlaying before calling this method!
         * @param [in] channelNum Offset into multichannel data. Mono
         * channels use 0. Stereo channels use 0 = left, 1 = right.
         * More than stereo use the appropriate index.
         *
         * @return Value from 0 to 1 for the input level of the wave
         **/
        virtual float getCurrentLevel(int channelNum) const;


	private:
		ALuint bufferId;
		ALuint sourceId;


		// Static position and velocity for the sounds to play at
		static ALfloat sPosition[3];
		static ALfloat sVelocity[3];

		static const float PosPanFactor;

		bool	m_isValid;
	};

}

#endif	// __CING_AUDIO_SOUNDOPENAL_H__
