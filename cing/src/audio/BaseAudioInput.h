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
#ifndef _IL_AudioInput_H
#define _IL_AudioInput_H

// Precompiled headers
#include "Cing-Precompiled.h"


namespace Cing
{

/**
 * \class BaseAudioInput
 * Interface to handle system's AudioInputphone.
 *
 **/
class BaseAudioInput
{
public:

		// Initialization

		/**
		 * @brief Forces AudioInput initialization by user.
		 * AudioInput auto initializes when first action is
		 * requested. However we shall give the user a way to
		 * init it, as it may take a while to init the driver.
		 * If the user wants a lighting action response, should
		 * call this method on his initialization code.
		 *
		 * @return True if AudioInput is initialized. False if AudioInput can not
		 * be initialized (reporting error message).
		 **/
		virtual void init() = 0;

		// Actions

		/**
		* @brief Starts capturing sound from AudioInputphone
		*
		**/
		virtual void startCapturing() = 0;

		/**
		 * @brief Stops current AudioInputphone action (capturing
		 * or recording) if there is any.
		 *
		 **/
		virtual void stop() = 0;

		/**
		 * @brief Gets current input level from AudioInputphone wave
		 *
		 * @return Value from 0 to 1 for the input level of the wave
		 **/
		 virtual float getCurrentLevel() const = 0;

}; // class AudioInput

} // namespace Cing


#endif // _IL_AudioInput_H

