#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows how to capture video images from the camera
 * and apply simple filters to it (grayscale, blur, threshold)
 */

// OpenCV module includes
#include "OpenCV/src/OpenCVIncludes.h"

Capture capture;
Image colorImg, grayImg, blurryImg, thresholdImg;

void setup()
{
	size( 640, 480 );
	applyCoordinateSystemTransform(PROCESSING);

	// Set OpenCV 2D renderer as the default
	enableOpenCVRenderer2D();

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
	// Get new frames from camera input
	capture.update();

	// color
	colorImg = capture.getImage(); // this is a weak copy, so both image have the same data. modifying either of them would affect the other.
	colorImg.draw( 0, 0) ;

	// grayscale
	grayImg = capture.getImage();	// this is a weak copy, but when calling toGray, the image format is changed and therefore new data is created (so the camera image is not affected by that toGrady)
	grayImg.toGray();
	grayImg.draw( 320, 0 );

	// blurry
	capture.getImage().copyTo( blurryImg ); // faster way to create a deep copy of the data, so that we can alter it without modifying the original (camera image)
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