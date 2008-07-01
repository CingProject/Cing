#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

// boid simulation
OpenSteer::OpenSteerDemo	boidSimulation;

// the MidiIO interface
MidiIO midiIO;
PointLight								light;

void setup()
{	
	showDebugOutput(true);
	// Create a boid simulation
	boidSimulation.initialize();
	// Init lights and set the ambient light
	ambientLight( 10, 10, 55 );
	light.init( 255, 255, 255, width / 2, height / 2, 200 );	
	// Print all Midi devices
	midiIO.printDevices();
	// Open the first input device
	midiIO.openInput(0);
	midiIO.m_MidiIn->setQueueSizeLimit(4096);
}

void draw()
{

	// First check if there are new notes
	std::vector<unsigned char> *message = new  std::vector<unsigned char>;
	midiIO.m_MidiIn->getMessage(message);
	unsigned int nBytes = message->size();
	delete message;	

	// Create new boid
	if (nBytes==3){
		gBoidsPlugIn.addBoidToFlock();
	}

	// Updates simulation
	boidSimulation.updateSimulationAndRedraw();
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
