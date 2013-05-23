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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "MidiIO.h"

namespace Cing
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
	GUIManagerCEGUI::getSingleton().getDebugOutput().print( "Input Ports:" );

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

		print()
		// Print it to debug output
		GUIManagerCEGUI::getSingleton().getDebugOutput().println("");
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( "  | Port" );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);

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
	GUIManagerCEGUI::getSingleton().getDebugOutput().print( "Outut Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiOut->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		// Print it to debug output
		GUIManagerCEGUI::getSingleton().getDebugOutput().println("");

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);
		/*
		// Convert to string
		itoa(i,portNumber,10);

		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( "  | Port" );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);
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
	GUIManagerCEGUI::getSingleton().getDebugOutput().println( "Input Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiIn->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		// Print it to debug output
		GUIManagerCEGUI::getSingleton().getDebugOutput().println("");

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);
		/*
		// Convert to string
		itoa(i,portNumber,10);

		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( "\t\t  | Port" );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);
		*/

		// Delete the buffer
		delete[] portNumber;
	}

	// Output Ports
	nPorts = m_MidiOut->getPortCount();
	GUIManagerCEGUI::getSingleton().getDebugOutput().println( "Output Ports:" );

	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			portName = m_MidiOut->getPortName(i);
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		// Print it to debug output
		GUIManagerCEGUI::getSingleton().getDebugOutput().println("");

		char *portNumber;
		// Create a new char array
		portNumber = new char[128];
		// Set it to empty
		memset(portNumber,'\0',100);
		/*
		// Convert to string
		itoa(i,portNumber,10);

		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portName.c_str() );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( "\t\t  | Port" );
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( ":	");
		GUIManagerCEGUI::getSingleton().getDebugOutput().print( portNumber);
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
		GUIManagerCEGUI::getSingleton().getDebugOutput().println(messageChunk);

		sprintf(messageChunk, "Byte 1: %i ,", byte1);
		GUIManagerCEGUI::getSingleton().getDebugOutput().println(messageChunk);

		sprintf(messageChunk, "Byte 2: %i ,", byte2);
		GUIManagerCEGUI::getSingleton().getDebugOutput().println(messageChunk);
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
} // namespace Cing
