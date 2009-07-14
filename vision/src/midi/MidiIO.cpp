/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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
		m_MidiIn->setQueueSizeLimit(4096);
	}
	catch ( RtError &error ) {
		error.printMessage();
		return false;
	}

	// RtMidiOut constructor
	try {
		m_MidiOut = new RtMidiOut();
	}
	catch ( RtError &error ) {
		error.printMessage();
		return false;
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

	// Close all open ports

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
 * @external
 * @brief Print input devices
 */
void MidiIO::printInputDevices()
{

	// Check if the class is initialized
	if ( !isValid() )
		init();

	// Print Input Ports
	std::string portName;
	unsigned int nPorts = m_MidiIn->getPortCount();
	GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( "Input Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiIn->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);

		/*
		// Convert to string
		itoa(i,portNumber,10);

		Common::print()
		// Print it to debug output
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println("");
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( "  | Port" );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);

		*/
		// Delete the buffer
		delete portNumber;
	}
}

/**
 * @external
 * @brief Print Output devices
 */
void MidiIO::printOutputDevices()
{
	// Check if the class is initialized
	if ( !isValid() )
		init();

	// Input Ports
	std::string portName;
	unsigned int nPorts = m_MidiOut->getPortCount();
	GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( "Outut Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiOut->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		// Print it to debug output
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println("");

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);
		/*
		// Convert to string
		itoa(i,portNumber,10);

		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( "  | Port" );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);
		*/
		// Delete the buffer
		delete portNumber;
	}
}

/**
 * @external
 * @brief Print all devices
 */
void MidiIO::printDevices()
{
	// Check if the class is initialized
	if ( !isValid() )
		init();

	// Input Ports
	std::string portName;
	unsigned int nPorts = m_MidiIn->getPortCount();
	GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println( "Input Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiIn->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		// Print it to debug output
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println("");

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);
		/*
		// Convert to string
		itoa(i,portNumber,10);

		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( "\t\t  | Port" );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);
		*/

		// Delete the buffer
		delete[] portNumber;
	}

	// Output Ports
	nPorts = m_MidiOut->getPortCount();
	GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println( "Output Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiOut->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		// Print it to debug output
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println("");

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);
		/*
		// Convert to string
		itoa(i,portNumber,10);

		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( "\t\t  | Port" );
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);
		*/
		// Delete the buffer
		delete portNumber;
	}

}

/**
 * @external
 * @brief  Use this Method to open an input device.
 */
void MidiIO::openInput(int inputDeviceNumber)
{

	// Check if the class is initialized
	if ( !isValid() )
		init();

	m_MidiIn->openPort(inputDeviceNumber);
	// Ignore sysex, timing, or active sensing messages.
	m_MidiIn->ignoreTypes( true, true, true );

}

/**
 * @internal
 * @brief  Use this Method to close an input device.
 */
void MidiIO::closeInput()
{
	if ( isValid() )
		m_MidiIn->closePort();
}

/**
 * @internal
 * @brief  TODO
 */
void MidiIO::onMessage( double deltatime, std::vector< unsigned char > *message, void *userData )
{
	unsigned int nBytes = (int) message->size();

	// for debug
	char* messageChunk;
	messageChunk = new char[128];
	memset(messageChunk,'\0',100);

	if ( nBytes == 3 ) {

		unsigned int byte0 = message->at(0);
		unsigned int byte1 = message->at(1);
		unsigned int byte2 = message->at(2);

		byte0 = (unsigned int)(message->at(0));
		byte1 = (unsigned int)(message->at(1));
		byte2 = (unsigned int)(message->at(2));

		/*
		sprintf(messageChunk, "Byte 0: %i ,", byte0);
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println(messageChunk);

		sprintf(messageChunk, "Byte 1: %i ,", byte1);
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println(messageChunk);

		sprintf(messageChunk, "Byte 2: %i ,", byte2);
		GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println(messageChunk);
		*/
	}
	delete messageChunk;
}
/**
 * @internal
 * @brief  TODO
 */
void MidiIO::plug(  void (*callbackFunction)( double deltatime, std::vector< unsigned char > *message, void *userData )  )
{
	m_MidiIn->setCallback( callbackFunction );
}
} // namespace Midi
