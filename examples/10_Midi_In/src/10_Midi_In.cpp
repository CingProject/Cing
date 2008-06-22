#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

// the MidiIO interface
MidiIO midiIO;

void setup()
{
	showDebugOutput(true);
	print(midiIO.printInputDevices().c_str());
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