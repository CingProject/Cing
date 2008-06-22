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
#include "MidiIO.h"

namespace Midi
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
MidiIO::MidiIO():
	m_bIsValid( false ),
	m_MidiIn( NULL ),
	m_MidiOut( NULL )
{
	init();
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
MidiIO::~MidiIO()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @return true if the initialization was ok | false otherwise
 */
bool MidiIO::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

	// RtMidiIn constructor
	try {
		m_MidiIn = new RtMidiIn();
	}
	catch ( RtError &error ) {
		error.printMessage();
		exit( EXIT_FAILURE );
	}

	// RtMidiOut constructor
	try {
		m_MidiOut = new RtMidiOut();
	}
	catch ( RtError &error ) {
		error.printMessage();
		exit( EXIT_FAILURE );
	}

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void MidiIO::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// Clear RtMidi objects
	delete m_MidiIn;
	m_MidiIn  =  NULL;
	delete m_MidiOut;
	m_MidiOut =  NULL;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Updates the class state
 */
void MidiIO::update()
{
}

/**
* @internal
* @brief Returns a string with the first input midi port
*/
std::string MidiIO::printInputDevices()
{

	// Check if the class is initialized
	if ( !isValid() )
		init();

	std::string portName;
	portName = m_MidiIn->getPortName(0);

	return portName;
}

} // namespace Midi