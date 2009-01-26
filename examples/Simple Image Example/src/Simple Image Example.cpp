#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image img;
void setup()
{

	//Image img = createImage(66, 66, RGB);

	//img.load( "SIMO.jpg" );
	img = loadImage("SIMO.jpg");

	/*
	for(int i=0; i < img.pixels.length; i++) {
		img.pixels[i] = color(0, 90, 102); 
	}
	image(img, 17, 17);
  */

}

void draw()
{
	// Draw the image in 2d where the mouse is
	img.draw2d( mouseX, mouseY, 100, 100 );
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