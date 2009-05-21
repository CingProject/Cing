#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

void setup()
{
  size( 800, 600 );

  // Enable camera control
  useKeyboardCameraControl(true);
  useMouseCameraControl(true);

	// Load Collada file
	bool success = loadCollada( "DoorAnimation.dae" );

	// Check for success
	if ( !success )
		LOG_ERROR( "Error loading %s. It should be in the data folder" );
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