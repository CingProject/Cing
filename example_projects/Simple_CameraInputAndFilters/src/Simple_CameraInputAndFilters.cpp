#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows how to capture video images from the camera
 * and apply simple filters to it (grayscale, blur, threshold)
 */

Capture capture;
Image colorImg, grayImg, blurryImg, thresholdImg;

void setup()
{
	size( 640, 480 );
	applyCoordinateSystemTransform(PROCESSING);

	// Init camera
	capture.init( 0, 320, 240, 25, RGB );

	// Init images
	colorImg.init( 320, 240, RGB );
	grayImg.init( 320, 240, RGB );
	blurryImg.init( 320, 240, RGB );
	thresholdImg.init( 320, 240, RGB );
}

void draw() 
{
	capture.update();

	// color
	colorImg = capture.getImage();
	colorImg.draw(0, 0);

	// grayscale
	grayImg = capture.getImage();
	grayImg.toGray();
	grayImg.draw( 320, 0 );

	// blurry
	blurryImg = capture.getImage();
	blurryImg.filter( BLUR, 5 );
	blurryImg.draw( 0, 240 );

	// threshold
	thresholdImg = capture.getImage();
	thresholdImg.toGray();
	thresholdImg.filter( THRESHOLD, 50 );
	thresholdImg.draw( 320, 240 );
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