#include "Cing.h"

/*
 * This example shows of create 3d primitives with physics properties
 * It creates a grid of boxes and spheres and activates their physics.
 * Some of the primitives created are create with static physics properties
 * so they don't move although they collide with the rest of the objects
 *
 * Mouse
 *	- move mouse to rotate the camera
 * Keys
 *	- w,a,s,d + mouse: control the 3d camera
 *	- Space bar enables physics on all objects
 *	- e/r: enable/disable physics 
 *  - f: apply vertical (slightly random) force to all objects
 *  - t: apply velocity to all boxes towards an atractor (little sphere in the center)
 */

CREATE_APPLICATION( "Cing" );

// Number of spheres and boxes that are going to be created
#define MAX_ELEMENTS 101

// 3d primitives
PhysicsSphere		sphere[MAX_ELEMENTS];	// Spheres
PhysicsBox			box[MAX_ELEMENTS];		// Boxes 
PhysicsPlane		plane;					// Plane 

PhysicsSphere		atractor;				// Sphere to simulate an atractor

PointLight			light;					// Lights in the scene

void setup()
{
	size(1024,768);
	//setFrameRate(60);

	// Open GL coordinate system (Y axis grows up and 0,0 is at the center of the screen)
	applyCoordinateSystemTransform(OPENGL3D);

	// Setup physics
	enablePhysics();
	setGravity(-50);


	// init spheres and boxes
	int coordX = -1000;
	int coordY = 100;
	for ( int i = 0; i < MAX_ELEMENTS; i++ )
	{
		// Control coordinates to place them in a grid-like arrangement
		coordY += 150;
		if ( (i % 10) ==  0 )
		{
			coordX += 200;
			coordY = 100;
		}

		// Set size and position (with some random)
		sphere[i].init( 100 );
		sphere[i].setPosition( coordX + random(60), coordY, 200 );

		box[i].init( 100 );
		box[i].setPosition( coordX + random(60), coordY, 0 );
		box[i].setAmbientColor( 255, 0, 0 );
	}

	// Atractor sphere
	atractor.init(50);
	atractor.setPosition(0, 1000, 1000);
	atractor.setDiffuseColor(255, 127, 0);

	// plane
	plane.init( 100000, 100000 );
	plane.setPosition( 0, -1200, 0 );
	plane.rotate(Vector::UNIT_X, PI);

	// Enable physics plate as static object (that comes from the true parameter passed)
	plane.enablePhysics( true );

	// Init lights and set the ambient light
	ambientLight( 70 );
	light.init( 100, 100, 100, 0, 0, 200 );	

	// Activate default 3d camera controler (mouse+keys)
	enableDefault3DCameraControl();

	// Little helper to show the Frames per Second (vertical sync is on, so max is 60fps)
	showFps();
}

void draw()
{
	// Print the demo keys on the screen
	text("wasd + mouse: control the camera", 10, 30);
	text("e/r: enable/disable physics", 10, 50);
	text("SPACE: enable physics for all objects", 10, 70 );
	text("f: apply random vertical force to objects", 10, 90);
	text("t: apply velocit to all boxes towards the atractor", 10, 110);
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
	// Space bar is pressed: Apply force to all objects when 
	if ( key == ' ' )
	{
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			box[i].enablePhysics();
			sphere[i].enablePhysics();
		}
	}
	// e key: enable physics
	else if ( key == 'e' )
	{
		enablePhysics();
	}
	// r key: disable physics
	else if ( key == 'r' )
	{
		disablePhysics();
	}
	// f key: apply vertical randomized impulse to all objects
	else if ( key == 'f' )
	{
		//float forceScale = 50000000;
		float forceScale = 5000;
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			 box[i].applyCentralForce( Vector::UNIT_Y * forceScale );
			sphere[i].applyCentralForce( Vector::UNIT_Y * forceScale );
		}
	}
	// t key: set velocity on all the objects towards the atractor
	else if ( key == 't' )
	{
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			Vector atractorPos  = atractor.getPosition();
			Vector boxPos		= box[i].getPosition();
			Vector velocity		= atractorPos - boxPos;
			velocity.normalise();

			box[i].setLinearVelocity( velocity * 300 );
		}
	}
}