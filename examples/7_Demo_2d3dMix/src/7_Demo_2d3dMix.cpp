#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image		img, img2;		// image to load from disk and draw
bool		draw2d;	// to control 2d or 3d drawing

void setup()
{
	// Lets start with first image in 2d
	draw2d = true;

	// Load images
	img.load( "BeachStones.jpg" );
	img2.load( "BeachStones.jpg" );
}

void draw()	
{
	// draw images in 2d and 3d mixed
	if ( draw2d )
		img.draw2d( mouseX, mouseY, 100, 100 );
	else 
		img.draw( width / 2, height / 2, 0, 100, 100 );

	// Second image always in 3d
	img2.draw( 0, 0, 0, 100, 100 );
}

void end()
{
}

void mousePressed()
{
	// When mouse is pressed change first image from 2d to 3d or vice versa
	draw2d = !draw2d;
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
