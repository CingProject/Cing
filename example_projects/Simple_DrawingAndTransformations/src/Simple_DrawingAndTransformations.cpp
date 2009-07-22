#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

// This example shows the use of transformations
// and 2d drawing on images. It draw a rotating 
// rectangle on the screen. 

// Used to rotate the rectangle
float angle = 0;

// Create our background image
Image backgroundImage;

void setup()
{
	// Init size and smooth
	size(800, 800);
	smooth();

	// Init the image to clear the background
	backgroundImage.init(width, height, RGB);

	// Fill the image with white
	backgroundImage.fill( Color(255) );
}

void draw()
{
	// Blend the background with our backgroundImage
    GraphicsManager::getSingleton().m_canvas->blend( backgroundImage, 4 );

	// Instead of above code use:  (without the smooth transition)
	//background ( Color(255) );
	
	// Apply transforms before the drawing call
	translate (width/2,height/2);
	angle += 0.1;
	rotate( angle);
	scale(mouseX/400.0,mouseY/400.0);
	translate (100,100);

	// Finally draw the rectangle
	rect(0,0,50,50);
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