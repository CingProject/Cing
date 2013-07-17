#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows of to capture an Audio input 
 * Change the source (Mic, LineIn, WaveOut, ...) from yours OS audio menu
 * It also uses the input volume of the mic to modify the intensity of a light in the scene
 */

AudioInput	mic;		// To get the line in
Sphere		sphere;
Plane		plane;	
PointLight	light;

AverageFloat		average( 10 ); // This object allows you to easily calculate averages of incoming numbers

void setup()
{
	// start AudioInputphone
	mic.startCapturing();

	// setup plane and sphere
	plane.init( 100000 );
	plane.setPosition( width/2, height/2 + 500, 0 );	
	sphere.init( 100 );
	sphere.setPosition( width/2, height/2, 0 );

	// setup light
	ambientLight( 20, 20, 20 );
	light.init( 0, 0, 0, width/2, height/2, 200 );
}

void draw()
{
	// get the mic level and map it to the range 0..255 to use as color
	// the sensitivity variable allows you to adjust how sensitive it is (depends on the audio card input level)
	float soundLevel	= mic.getCurrentLevel();
	float sensitivity	= 250;
	soundLevel = constrain( soundLevel * sensitivity, 0, 1 );
	println( "Sound Level: %f", soundLevel );
	float soundLevelRamapped = map( soundLevel, 0, 1, 0, 255 );

	// calculate mic level average (to smooth out results)
	average.addValue( soundLevelRamapped );
	float soundLevelAv = average.getValue();

	// change color of the light
	println( "Sound Level AV: %f", soundLevelAv );
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