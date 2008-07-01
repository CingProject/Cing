#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

// TODO: There´s a memory leak somewhere...

// the MidiIO interface
MidiIO midiIO;

// This function is call when new Midi note is received
void noteOn ( double deltatime, std::vector< unsigned char > *message, void *userData ) 
{
	println("stamp:%f",deltatime);
	println("note:%i",(int)message->at(1));
	println("vel:%i",(int)message->at(2));
}

void setup()
{
	showDebugOutput(true);

	// Print all Midi devices
	midiIO.printDevices();

	// Open the first input device
	midiIO.openInput(0);


	// Add a callback function for noteOn events.

	//midiIO.plug(&noteOn);  
	midiIO.m_MidiIn->setQueueSizeLimit(128);

}

void draw()  
{	

}


void end()
{
}

void mousePressed()
{
}

void mouseMoved()
{
}

void mouseReleased()
{
}

void keyPressed()
{
}