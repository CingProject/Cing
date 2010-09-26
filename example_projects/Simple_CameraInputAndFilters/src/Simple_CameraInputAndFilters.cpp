#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows how to capture video images from the camera
 * and apply simple filters to it (grayscale, blur, threshold)
 */

Capture capture;
Image color, gray, blurry, threshold;

void setup()
{
	size( 640, 480 );

	// Init camera
	capture.init( 0, 320, 240, 25, RGB );

	// Init images
	color.init( 320, 240, RGB );
	gray.init( 320, 240, RGB );
	blurry.init( 320, 240, RGB );
	threshold.init( 320, 240, RGB );
}

void draw() 
{
	capture.update();

	// color
	color = capture.getImage();
	color.draw( 0, 0);

	// grayscale
	gray = capture.getImage();
	gray.toGray();
	gray.draw( 320, 0, 0 );

	// blurry
	blurry = capture.getImage();
	blurry.filter( BLUR, 5 );
	blurry.draw( 0, 240, 0 );

	// threshold
	threshold = capture.getImage();
	threshold.toGray();
	threshold.filter( THRESHOLD, 50 );
	threshold.draw( 320, 240, 0 );
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