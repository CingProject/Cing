#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

BackgroundSubtraction bg;
Capture								camera;
IplImage*							cvimage;
Image									img;
bool									drawCV;

void setup()
{
	drawCV = false;
	camera.init( 0, 320, 240, 25, GRAYSCALE );
	img.init( 320, 240, GRAYSCALE );
	bg.storeBackground( camera.getImage() );
}

void draw()
{
	// Update the camera
	camera.update();
	if ( drawCV )
		camera.getImage().draw2d( 0, 0 );

	// background subtraction
	bg.compute( camera.getImage(), img);

	if ( drawCV )
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
	else if ( key == 'b' )
		drawCV = !drawCV;
}