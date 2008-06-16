#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

BackgroundSubtraction bgSubtraction;
BlobFinder						bfinder;
Capture								camera;	// Camera to capture video input
Image									img;		// Image to draw the result of the background subtraction
Image									imgColor;

void setup()
{
	// Init camera and image
	camera.init( 0, 320, 240, 25, GRAYSCALE );
	img.init( 320, 240, GRAYSCALE );
	imgColor.init( 320, 240, RGB );

	// Store background 
	bgSubtraction.storeBackground( camera.getImage() );

	// Show debug output
	showDebugOutput( true ); 
	showFps( true );
}

void draw()
{
	// Update the camera
	camera.update();

	// background subtraction
	// Convert image to gray again (as we are doing background subtraction in gray scale
	img.toGray();
	bgSubtraction.update( camera.getImage(), img);

	// Find blobs
	bfinder.update( img );

	// Convert image to color to draw on in
	img.toColor();

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


	// Print number of blobs
	println( "N Blobs: %d", nBlobs );

	// Draw images
	camera.getImage().draw2d( 0, 0 );
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
		bgSubtraction.storeBackground( camera.getImage() );
}