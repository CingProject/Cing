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

#ifndef _Sound_h
#define _Sound_h


// Precompiled headers
#include "Cing-Precompiled.h"


namespace Cing
{

    /**
		 * @internal
     * BaseSound
     * Interface to deal with sounds (load, play...etc).
     * @note Please note this interface is called BaseSound, instead
     * of Sound. If you are using Cing to build your application
     * you should use Sound type directly instead of Sound, which is for
     * internal definition purposes.
     *
     **/
    class BaseSound
    {
    public:

			virtual ~BaseSound() {}

        /**
         * @brief Loads a sound file.
				 * If already had a file loaded, it will be discarded, loading
				 * the new file.
         * @param [in] fileName Filename (relative to application)
         *
         **/
        virtual void load( const std::string& fileName ) = 0;

        /**
         * @brief Plays the sound.
         * It has to be previously loaded or it will fail.
         * @param [in] loop True to set loop mode on. False to stop playing
         * when the file ends.
         *
         * @sa Load
         **/
        virtual void play( bool loop ) = 0;

        /**
         * @brief Pauses the sound.
         * It has to be playing, or it will fail. It will remember
         * the sound's position, so you can play it from where it was
         * paused.
         *
         * @sa Play
         **/
        virtual void pause() = 0;

        /**
         * @brief Stops playing the sound.
         * It has to be playing, or it will fail. It won't remember
         * the current sound's position, so if you call play later,
         * it will start from the beginning.
         *
         * @sa Play
         **/
        virtual void stop() = 0;


        /**
         * @brief Tells whether the sound is currently being played
         * or not.
         *
         * @return True if the sound is currently being played. Otherwise
         * it returns false.
         **/
        virtual bool isPlaying() const = 0;

        // Parameters

        /**
         * @brief Changes the volume for the sound.
         * @param [in] value The new volume value from 0 to 1.
         * 0 is silence, 1 is maximum volume.
         *
         **/
        virtual void setVolume( float value ) = 0;

        /**
         * @brief Returns current volume value.
         *
         * @return Current volume value from 0 to 1.
         * 0 is silence, 1 is maximum volume.
         */
        virtual float getVolume() const = 0;

        /**
         * @brief Changes playback speed.
         * @param [in] value New speed value. You can pass any value.
         * Positive values will increase the speed, while negative
         * values will increase the speed playing the sound backwards!
         *
         **/
        virtual void setSpeed( float value ) = 0;

        /**
         * @brief Gets current playback speed.
         *
         * @return Current playback speed value.
         **/
        virtual float getSpeed() const = 0;

        /**
         * @brief Sets the pan level for the sound.
         * @param [in] value New pan value for this sound, from 0 to 1.
         * 0 is to the left, 1 is to the right.
         *
         **/
        virtual void setPan( float value ) = 0;

        /**
         * @brief Gets current pan value.
         *
         * @return Current pan value from 0 to 1.
         * 0 is to the left, 1 is to the right.
         **/
        virtual float getPan() const = 0;

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
        virtual float getCurrentLevel( int channelNum ) const = 0;

    };

} // namespace Cing

#endif // _SOUND_H

