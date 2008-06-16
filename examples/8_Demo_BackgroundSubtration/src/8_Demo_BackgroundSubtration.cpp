#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

BackgroundSubtraction bg;
Capture								camera;
IplImage*							cvimage;
Image									img;

void setup()
{
	camera.init( 0, 320, 240, 25, GRAYSCALE );
	img.init( 320, 240, GRAYSCALE );
	bg.storeBackground( camera.getImage() );
}

void draw()
{
	// Update the camera
	camera.update();
	camera.getImage().draw2d( 0, 0 );

	// background subtraction
	bg.compute( camera.getImage(), img);

	img.draw2d( 320, 0 );
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
	if ( key == ' ' )
		bg.storeBackground( camera.getImage() );
}