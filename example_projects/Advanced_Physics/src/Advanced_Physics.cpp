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
 *	- Space bar applies a vertical force to all the objects( those non static) if
 *		the physics is activated
 *	- w,a,s,d,q,e,r and arrows: control the 3d camera
 */

CREATE_APPLICATION( "Cing" );

// Number of spheres and boxes that are going to be created
#define MAX_ELEMENTS 300

// 3d primitives
PhysicsSphere		sphere[MAX_ELEMENTS];	// SphereS
PhysicsBox			box[MAX_ELEMENTS];		// Boxes 
PhysicsPlane		plane;								// Plane 
PointLight			light;								// Lights in the scene

void setup()
{
	size(1024,768);

	// init spheres and boxes
	int coordX = -5000;
	int coordY = 100;
	for ( int i = 0; i < MAX_ELEMENTS; i++ )
	{
		// Set size and position (with some random)
		sphere[i].init( 100 + random( -50, 50 ) );
		sphere[i].setPosition( coordX + random ( -20, 20 ), coordY, 200 );
		sphere[i].enablePhysics( false );

		box[i].init( 100 + random( -50, 50 ) );
		box[i].setPosition( coordX + random ( -20, 20 ), coordY, 0 );
		box[i].enablePhysics( false );

		// Control coordinates to place them in a grid way
		coordY += 150;
		if ( (i % 10) ==  0 )
		{
			coordX += 200;
			coordY = 100;
		}
	}

	// plane
	plane.init( 100000 );
	plane.setPosition( 0, 2000, 0 );
	plane.setOrientation( Vector::UNIT_X, 15 );
	plane.setSelfIlluminationColor( 120, 120, 0 );

	// Enable physics plate as static
	plane.enablePhysics( true );

	// Init lights and set the ambient light
	ambientLight( 50, 50, 50 );
	light.init( 100, 100, 100, 0, 0, 200 );	

	// Allow mouse control
	useDefault3DCameraControl( true );
	useMouseCameraControl( true );
	useKeyboardCameraControl( true );
}

void draw()
{

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
	// Apply force to all objects when space bar is pressed
	if ( key == ' ' )
	{
		float forceScale = 100;
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			box[i].applyCentralImpulse( Vector::NEGATIVE_UNIT_Y * forceScale );
			sphere[i].applyCentralImpulse( Vector::NEGATIVE_UNIT_Y * forceScale );
		}
	}
}