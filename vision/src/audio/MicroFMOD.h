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

#ifndef _IL_MICRO_FMOD_H
#define _IL_MICRO_FMOD_H

#include "BaseMicro.h"
#include "AudioPrereqs.h"

namespace Audio
{

/**
 * @internal
 * \class MicroFMOD
 * Implements behavior to handle system's microphone
 * using fmod library.
 *
 * @todo Current version uses playback to get input
 * information. May be implemented getting the information
 * directly from sound source, instead of channel. Revise
 * DoStartPlayback and DoStopPlayback (may not be needed if you
 * can get the info from DoStartRecord)
 *
 **/
class MicroFMOD : public BaseMicro
{
public:
  // Constructor/Destructor
          MicroFMOD   ();
  virtual ~MicroFMOD  ();

  // Init / end
  void init   ();
  void end		();

	bool isValid() const;

  // Actions
  void startCapturing ();
  void stop           ();
  bool isRunning      () const;   // TODO Move to Micro interface

  // Get data
  float getCurrentLevel() const;

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
  FMOD_SYSTEM*		m_sysPtr;  ///< Owns a copy to shared system (should be synchronized)
  FMOD_SOUND*			m_sound;   ///< Sound when capturing/recording
  FMOD_CHANNEL*		m_channel; ///< Use a channel for processing

}; // class MicroFMOD

} // namespace Audio

#endif // _IL_MICRO_FMOD_H

