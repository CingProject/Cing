#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows of is possible to mix 2d and 3d rendering in the same scene
 * Keys 
 *	- w,a,s,d,q,e,r and arrows: control the 3d camera
 * Mouse
 *	- move mouse to rotate the camera
 */

Image				img;		// image to load from disk and draw
Sphere			sphere;	// Sphere 3d primitive
PointLight	light;	// light to light up the sphere

void setup()
{
	// Load image
	img.load( "BeachStones.jpg" );

	// set sphere properties
	sphere.init( 100 );
	sphere.setPosition( (width / 2), height / 2, 0 );
	sphere.setDiffuseColor( 100, 100, 100 );

	// set lighting
	ambientLight( 30, 30, 30  );
	light.init( 0, 255, 0, width/2 - 200, height / 2, -200 );	
	//light.drawDebug( true );

	// allow mouse control
	//useMouseCameraControl( true );
}

void draw()	
{
	img.draw( mouseX, mouseY, 0, 100, 100 );
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
