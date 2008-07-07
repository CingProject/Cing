#include "Vision.h"
CREATE_APPLICATION( "Vision Demo" );

/*
 * This example shows of create 3d primitives and basic lighting
 * It applies a texture (from an image file) to the 3d primitives
 * Keys 
 *	- 1, 2, 3 to control render mode
 *	- w,a,s,d,q,e,r and arrows: control the 3d camera
 * Mouse
 *	- move mouse to rotate the camera
 */

// 3d primitives
Sphere			sphere;					// Sphere 3d primitive
Box					box;						// Box (cube) 3d primitive
Plane				plane;					// Plane 3d primitive
PointLight	light, light2;	// Lights in the scene

float				timeVar = 0;

void setup()
{
	// sphere
	sphere.init( 100 );
	sphere.setPosition( (width / 2) + 100, height / 2, 0 );
	sphere.setDiffuseColor( 100, 100, 100 );
	sphere.setTexture( "BeachStones.jpg" );

	// box
	box.init( 100 );
	box.setPosition( (width / 2) - 100, height / 2, 0 );
	box.setTexture( "BeachStones.jpg" );
	box.setDiffuseColor( 255, 0, 0 );

	// plane
	plane.init( 10000 );
	plane.setPosition( 0, 0 , 0 );
	plane.setSelfIlluminationColor( 0, 0, 100 );
	plane.setOrientation( Vector( 1, 0, 0 ), 10 );

	// Init lights and set the ambient light
	ambientLight( 50, 50, 50 );
	light.init( 255, 255, 255, width / 2, height / 2, 200 );	
	light2.init( 255, 100, 160, width / 2, height / 2, 200 );

	// Allow mouse control
	useMouseCameraControl( true );

	// Show lights in the scene
	light.drawDebug( true );
	light2.drawDebug( true );
}

void draw()
{
	// move light
	timeVar += 0.01f;
	Vector pos = light.getPosition(); 
	pos.x += cos( timeVar ) * 10;
	light.setPosition( pos );
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
	// Drawing modes
	if ( key == '1' )
		setRenderMode( DRAW_WIREFRAME );
	else if ( key == '2' )
		setRenderMode( DRAW_SOLID );
	else if ( key == '3' )
		setRenderMode( DRAW_POINTS );
}