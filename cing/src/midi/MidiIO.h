/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

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

#ifndef _MidiIO_H_
#define _MidiIO_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "midi/MidiPrereqs.h"

#include "gui/GUIManagerCEGUI.h"

namespace Cing
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
	void				printInputDevices();
	// Prints  output midi devices.
	void				printOutputDevices();
	// Prints  all midi devices.
	void				printDevices();

	// Use this Method to open an input device.
	void				openInput(int inputDeviceNumber);
	// Close an open MIDI connection (if one exists). 
	void				closeInput();

	static void	onMessage( double deltatime, std::vector< unsigned char > *message, void *userData );
	void				plug     ( void (*callbackFunction) ( double deltatime, std::vector< unsigned char > *message, void *userData ) );
	RtMidiIn*   m_MidiIn;

private:

	RtMidiOut*   m_MidiOut;
	bool         m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
};

} // namespace Cing

#endif // _MidiIO_H_
