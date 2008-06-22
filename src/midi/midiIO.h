/*
This source file is part of the Vision project
For the latest info, see http://www.XXX.org

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

#ifndef _MidiIO_H_
#define _MidiIO_H_

#include"midi\MidiPrereqs.h"

#include "gui\GUIManagerCEGUI.h"

namespace Midi
{
/**
 * @internal
 * TODO: maybe we can make the same interface of MidiIO??
 * MidiIO is the base class for managing the available midi ports.
 * It provides you with methods to get information on your ports 
 * and to open them. To start use the printDevices
 * method to get all devices available on your system.
 */
class MidiIO
{
public:

	// Constructor / Destructor
	MidiIO();
	~MidiIO();

	// Init / Release / Update
	bool        init        ();
	void        end         ();
	void        update      ();

	// Query methods
	bool        isValid     () const	{ return m_bIsValid; }
	
	// Prints  input midi devices.
	void	printInputDevices();
	// Prints  output midi devices.
	void	printOutputDevices();
	// Prints  all midi devices.
	void	printDevices();

private:
	RtMidiIn*    m_MidiIn;
	RtMidiOut*   m_MidiOut;
	bool         m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Midi

#endif // _MidiIO_H_
