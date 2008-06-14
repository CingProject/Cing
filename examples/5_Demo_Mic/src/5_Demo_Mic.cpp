#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Micro				mic;
Sphere			sphere;
Plane				plane;	
PointLight	light;

Average			average( 20 );

void setup()
{
	// start microphone
	mic.startCapturing();

	// setup plane and sphere
	plane.init( 100000 );
	plane.setPosition( 0, -500, 0 );	
	sphere.init( 100 );
	sphere.setPosition( 0, 0, 0 );

	// setup light
	ambientLight( 20, 20, 20 );
	light.init( 0, 0, 0, 0, 0, 200 );
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

void keyPressed()
{
}