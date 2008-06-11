#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

// 3d primitives
Sphere			sphere;					// Sphere 3d primitive
Box					box;						// Box (cube) 3d primitive
Plane				plane;					// Plane 3d primitive
PointLight	light, light2;	// Lights in the scene



void setup()
{
	// sphere
	sphere.init( 100 );
	sphere.setPosition( 0, -100, 0 );
	sphere.setDiffuseColor( 100, 100, 100 );
	sphere.setTexture( "BeachStones.jpg" );

	// box
	box.init( 100 );
	box.setPosition( 100, 0, 0 );
	box.setTexture( "BeachStones.jpg" );
	box.setDiffuseColor( 255, 255, 255 );

	// plane
	plane.init( 100000 );
	plane.setPosition( 0, -500, 0 );
	plane.setTexture( "BeachStones.jpg" );
	plane.setSelfIlluminationColor( 60, 0, 30 );
	plane.setOrientation( Vector( 1, 0, 0 ), 10 );

	// Init lights and set the ambient light
	ambientLight( 50, 50, 50 );
	light.init( 255, 0, 0, 0, 0, 200 );	
	light2.init( 255, 100, 160, 100, 0, 200 );
}

void draw()
{
	static float pTime;
	pTime += 0.01f;
	float v = cos(pTime)*150;
	light.setPosition( Vector( v, 0, 200 ) );
}

void end()
{
}

void mousePressed()
{
	if ( mouseButton == RIGHT )
	{
		drawPhysics( true );
		
		// Activate object's physics
		sphere.activatePhysics();
		box.activatePhysics();
		plane.activatePhysicsStatic();
	}
	else
	{
		// Deactivate object's physics
		sphere.deActivatePhysics();
		box.deActivatePhysics();
		plane.deActivatePhysics();
	}
}

void keyPressed()
{
	// Drawing modes
	if ( key == '1' )
		setRenderMode( DRAW_WIREFRAME );
	else if ( key == '2' )
		setRenderMode( DRAW_SOLID );
	else if ( key == '3' )
		setRenderMode( DRAW_POINTS );

	// debug physics
	else if ( key == 'h' )
		drawPhysics( true );
	// debug lights
	else if ( key == 'l' )
	{
		light.drawDebug( true );
		light2.drawDebug( true );
	}
}