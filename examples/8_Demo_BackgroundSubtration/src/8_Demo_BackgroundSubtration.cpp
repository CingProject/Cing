#include "Vision.h"
CREATE_APPLICATION( "Vision Demo" );

/*
 * This example shows the computer vision technique background subtraction working.
 * It can be used to detect presence and extract properties about it (such as area, position...etc)
 * Here the background subtraction is performed over black & white camera images
 * Keys 
 *	- space bar: store current camera image as background
 */

BackgroundSubtraction bgSubtraction;// Object to perform Background subtraction
Capture								camera;				// Camera to capture video input
Image									img;					// Image to draw the result of the background subtraction

void setup()
{
	// Init camera and image
	camera.init( 0, 320, 240, 25, GRAYSCALE );
	img.init( 320, 240, GRAYSCALE );
}

void draw()
{
	// Update the camera
	camera.update();
	camera.getImage().draw2d( 0, 0 );

	// background subtraction
	bgSubtraction.update( camera.getImage(), img);
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
	// Store background when space bar is pressed
	if ( key == ' ' )
		bgSubtraction.storeBackground( camera.getImage() );
}