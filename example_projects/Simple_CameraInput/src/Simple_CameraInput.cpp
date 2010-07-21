#include "Cing.h"
CREATE_APPLICATION( "Cing" );

Vector2d r( 320, 240 );

Capture capture;
Image color, gray, blurry, threshold;

void setup()
{
	size( r.x*2, r.y*2 );

	capture.init( 0, r.x, r.y, 25, RGB, true );

	color.init( r.x, r.y, RGB );
	gray.init( r.x, r.y, RGB );
	blurry.init( r.x, r.y, RGB );
	threshold.init( r.x, r.y, RGB );
}

void draw()
{
	capture.update();

	// color
	color = capture.getImage();
	color.draw( 0, 0, 0 );

	// grayscale
	gray = capture.getImage();
	gray.toGray();
	gray.draw( r.x, 0, 0 );

	// blurry
	blurry = capture.getImage();
	blurry.filter( BLUR, 5 );
	blurry.draw( 0, r.y, 0 );

	// threshold
	threshold = capture.getImage();
	threshold.filter( THRESHOLD );
	threshold.draw( r.x, r.y, 0 );

}

void end()
{
	capture.end();
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