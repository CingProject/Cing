#include "Vision.h"
CREATE_APPLICATION( "Vision Demo" );

/*
 * This example shows of to capture microphone input
 * It also uses the input volume of the mic to modify the intensity of a light in the scene
 */


Micro				mic;
Sphere			sphere;
Plane				plane;	
PointLight	light;

Average			average( 15 );

void setup()
{
	// start microphone
	mic.startCapturing();

	// setup plane and sphere
	plane.init( 100000 );
	plane.setPosition( width/2, height/2 + 500, 0 );	
	sphere.init( 100 );
	sphere.setPosition( width/2, height/2, 0 );

	// setup light
	ambientLight( 20, 20, 20 );
	light.init( 0, 0, 0, width/2, height/2, -200 );
}

void draw()
{
	// get the mic level and map it to the range 0..255 to use as color
	float soundLevel = mic.getCurrentLevel();
	soundLevel = constrain( soundLevel * 10, 0, 1 );
	float soundLevelRamapped = map( soundLevel, 0, 1, 0, 255 );

	// calculate mic level average (to smooth results)
	average.addValue( soundLevelRamapped );
	float soundLevelAv = average.getValue();

	// change color of the light
	light.setDiffuseColor( soundLevelAv, soundLevelAv, soundLevelAv );
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