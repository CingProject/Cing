#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Movie myMovie;
Image	img;

void setup()
{
	myMovie.load( "test.avi" );
	myMovie.loop();
}

void draw()
{
	// get a frame from the video
	myMovie.read( img );

	img.draw( 0, 0 );
}

void end()
{
}

void mousePressed()
{
}

void keyPressed()
{
}