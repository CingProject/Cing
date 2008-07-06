#include "Vision.h"
// boid simulation

#include "OpenSteerDemo.h"
OpenSteer::OpenSteerDemo	boidSimulation;

PointLight								light;
Box												myFloor;

CREATE_APPLICATION( "Vision Demo" );

void setup()
{	

	showFps( true );

	// Init lights and set the ambient light
	setBackgroundColor( Color(0,0,0) );
	ambientLight( 55, 55, 55 );
	light.init( 255, 255, 255, width / 2, height / 2, 200 );

	// Set camera position and direction
	ogreCamera->setDirection( 0, -0.99, -0.00001 );
	ogreCamera->setPosition( -width/2, 6120, -height );

	// Create a boid simulation
	boidSimulation.initialize();

}

void draw()
{

	// Updates simulation
	boidSimulation.updateSimulationAndRedraw();

}

void end()
{
}

void mousePressed()
{
	//if ( mouseButton == LEFT )
		//gBoidsPlugIn.addBoidToFlock( mouseX - width/2, 18, mouseY - height/2);
	
	//if ( mouseButton == RIGHT )
//		gBoidsPlugIn.addBoidToFlock( mouseX - width/2, 18, mouseY - height/2);

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

/*
// the MidiIO interface
MidiIO midiIO;
*/

/*
// init

// Print all Midi devices
midiIO.printDevices();
// Open the first input device
midiIO.openInput(0);
midiIO.m_MidiIn->setQueueSizeLimit(4096);

// loop

// First check if there are new notes
std::vector<unsigned char> *message = new  std::vector<unsigned char>;
midiIO.m_MidiIn->getMessage(message);
unsigned int nBytes = message->size();
// Create new boid
if (nBytes==3)
{
println("note:%i",(int)message->at(1));
println("vel:%i",(int)message->at(2));
if ((int)message->at(2)!= 0)
{
gBoidsPlugIn.addBoidToFlock((int)message->at(1)*3-160, (int)message->at(2) ,368 );
}
}
delete message;	
*/