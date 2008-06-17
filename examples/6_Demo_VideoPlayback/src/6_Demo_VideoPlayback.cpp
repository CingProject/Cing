#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Movie myMovie;
Image	img;

void setup()
{
	// Load the video and play in loop mode
	myMovie.load( "test.avi" );
	myMovie.loop();
}

void draw()
{
	// Get a frame from the video
	myMovie.read( img );

	// Draw the image that contains the read frame
	img.draw2d( mouseX, mouseY );
}

void end()
{
}

void mousePressed()
{
	// Change the frames per second of the video playback
	if ( mouseButton == LEFT )
		myMovie.setFps( 5 );
	else
		myMovie.setFps( 30 );
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