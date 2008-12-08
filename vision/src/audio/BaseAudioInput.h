/*
	This source file is part of the Vision project
	For the latest info, see http://www.playthemagic.com/vision

	Copyright (c) 2008 XXX

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
#ifndef _IL_AudioInput_H
#define _IL_AudioInput_H

namespace Audio
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

} // namespace Audio


#endif // _IL_AudioInput_H

