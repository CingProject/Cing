#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows the computer vision techniques background subtraction and blob detection working.
 * Where blobs (elements in the scene) are detected, red rectangles are drawn in the image.
 * Here the background subtraction is performed over color camera images
 * Keys 
 *	- space bar: store current camera image as background
 */

BackgroundSubtraction bgSubtraction;	// Object to perform Background subtraction
BlobFinder						bfinder;				// Object to perform blob finding
Capture								camera;					// Camera to capture video input
Image									img;						// Image to draw the result of the background subtraction

void setup()
{
	// Init camera and image
	camera.init( 0, 320, 240, 30, RGB, false );
	img.init( 320, 240, RGB );
}

void draw()
{
	// Update the camera
	camera.update();

	// background subtraction
	bgSubtraction.update( camera.getImage(), img);

	// Find blobs
	bfinder.update( img );

	// Process found blobs
	stroke( 255, 0, 0 );
	strokeWeight( 2 );
	int nBlobs = bfinder.getNumBlobs();
	for ( int i = 0; i < nBlobs; i++ )
	{
		// get blob
		Blob& blob = bfinder.getBlobN( i );

		// draw it in the image
		img.rect( blob.bbox.x, blob.bbox.y, blob.bbox.x + blob.bbox.width, blob.bbox.y + blob.bbox.height );
	}

	// Draw images
	camera.getImage().draw( 0, 0, 0);
	img.draw( 640, 0, 0 );

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