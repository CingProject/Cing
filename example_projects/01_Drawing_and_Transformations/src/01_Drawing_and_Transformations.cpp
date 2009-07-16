#include "Cing.h"

CREATE_APPLICATION( "Cing Demo" );

// Background image
Image backgroundImage;

// Used to rotate the rectangle
float angle = 0;

void setup()
{
	// Init values for size and smooth
	size(800, 800);
	smooth();

	// Create an image to clear the background
	backgroundImage.init(width, height, RGB);

	// Fill with white
	backgroundImage.fill( Color(255) );
}

void draw()
{
	// Blend the background of screen with our backgroundImage
    //GraphicsManager::getSingleton().m_canvas->blend( backgroundImage, 4);

	// Instead of above code, you can use:  (without the smooth transition)
	 background ( Color(255) );
	
	// Apply transforms before the draw calls
	translate (width/2,height/2);
	angle += 0.1;
	rotate( angle);
	scale(mouseX/400.0,mouseY/400.0);
	translate (100,100);

	// Finally draw a rectangle
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