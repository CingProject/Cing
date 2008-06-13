#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Sound				sound;					// To play a sound
Sphere			sphere;					// 3d sphere
Plane				plane;					// 3d plane
PointLight	light;					// point light

Average			average( 10 );	// To average the levels of the sound

void setup()
{
	// load sound
	sound.load( "loop.ogg" );
	sound.play( true );

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
	// get the sound level and map it to the range 0..255 to use as color
	float soundLevel = sound.getCurrentLevel();
	LOG( "Sound: %f", soundLevel );
	soundLevel = constrain( soundLevel * 10, 0, 1 );
	float soundLevelRamapped = map( soundLevel, 0, 1, 0, 255 );

	// calculate sound level average (to smooth results)
	average.addValue( soundLevelRamapped );
	float soundLevelAv = average.getValue();

	// change color of the light
	light.setDiffuseColor( soundLevelRamapped, soundLevelRamapped, soundLevelRamapped );
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