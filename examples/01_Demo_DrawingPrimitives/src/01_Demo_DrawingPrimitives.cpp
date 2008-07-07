#include "Vision.h"
CREATE_APPLICATION( "Vision Demo" );

/*
 * This example shows of to use the 2d drawing primitives to modify a loaded image
 */

Image img, img2, img3;
float timeVar = 0;

void setup()
{
	// Load image from file
	img.load( "BeachStones.jpg" );

	// We can copy an image just by assigning it to another one
	img2 = img;
	img3 = img;
}

void draw()
{
	// variable to modify drawn shapes over time
	timeVar += 0.01;

	// restore original images (to draw over them)
	img = img3;
	img2 = img3;

	// draw a line in the first image
	strokeWeight( 8 );
	stroke( 0, 0, 0 );
	img.line( 256, 256, cos(timeVar)*150 + 256, sin(timeVar)*150 + 256 );

	// draw some shapes in the second
	strokeWeight(4);

	stroke( 205, 205, 15 );
	img2.ellipse( 256, 256, 50 + cos(timeVar)*20, 50 + cos(timeVar)*20 );

	stroke( 205, 0, 15 );
	img2.arc( 256, 256, 40, 40, 0, cos(timeVar)*360 );

	// Size to draw the images
	int size = 400;

	// Draw a label in the first image
	stroke( 255, 0, 0 );
	img.text( 20, 20, "Loaded Image");
	img.draw2d( 0, 100, size, size );

	// Draw a label in the second image
	stroke( 0, 255, 0 );
	img2.text( 20, 20, "Cloned image" );
	img2.draw2d( size, 100, size, size );
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
