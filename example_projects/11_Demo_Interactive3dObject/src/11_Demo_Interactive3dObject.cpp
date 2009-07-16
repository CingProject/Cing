#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows how to load a 3d model of a head and make it look at the user (using blob detection)
 * It creates a 3d object as "lookAt" target for the head. The position of the object is driven by the blobs
 * detects in the scene. Also a light is placed depending on blob's positions.
 *
 * This example also show that the use of ogre mesh and material system. The material script, shader and textures
 * used in the example can be found in the data folder of the example
 *
 * Keys 
 *	- b: shows camera input and background subtraction result 
 *	- space bar: store current camera image as background
*/

// computer vision stuff
BackgroundSubtraction bgSubtraction;				// Object to perform Background subtraction
BlobFinder						bfinder;							// Object to perform blob finding
Capture								camera;								// Camera to capture video input
Image									img;									// Image to draw the result of the background subtraction
bool									drawCV;								// Variable to control if the computer vision process will be drawn

// 3d stuff
Object3D        			head;                 // 3D head
Object3D							user;									// Object to represent the position of the user
PointLight      			pointLight;           // point light
Vector          			lightPos, targetPos;  // current and target light pos


void setup()
{
	// Init camera and image
	camera.init( 0, 320, 240, 25, RGB );
	img.init( 320, 240, RGB );
	drawCV = false;

	// Load 3d object of a head
	head.init( "head.mesh", "Examples/BumpMapping/MultiLight" );
	head.setPosition( width/2, height/2, -200 );
	//head.setOrientation( Vector::UNIT_Z, 180 );

	// Init object that will be used to represent the position of the user -> no mesh and no material
	user.init();
	user.setPosition( width/2, height/2, 1500 );
	head.lookAt( user );

	// setup lighting
	lightPos.z = 100;
	targetPos = lightPos;
	pointLight.init( 255, 255, 255, width/2, height/2, -300 );	
	ambientLight( 10, 10, 10 );

	// no camera movement
	useKeyboardCameraControl( false );
}

void draw()
{return;
	// Update the camera
	camera.update();

	// background subtraction
	bgSubtraction.update( camera.getImage(), img);

	// Find blobs
	bfinder.update( img );

	// Get number of found blobs in the scene
	int nBlos = bfinder.getNumBlobs();

	// If there there are blobs -> get the position of the biggest
	if ( nBlos > 0 )
	{
		Blob& blob = bfinder.getBlobN( 0 );

		// Get blob coordinates
		float blobX     = camera.getWidth() - (blob.bbox.x + (blob.bbox.width / 2));
		float blobTopY  = camera.getHeight() - blob.bbox.y;     

		// Get current light position
		Vector pos = pointLight.getPosition();

		// Map new values
		pos.x = map( blobX, 0, camera.getWidth(), 0, width );
		pos.y = map( blobTopY, 0, camera.getHeight(), 0, height );

		// Set new light pos
		pointLight.setPosition( pos );
		pos.z = -1500;
		user.setPosition( pos );
	}


	// Draw images computer vision process
	if ( drawCV )
	{
		camera.getImage().draw2d( 0, 0 );
		img.draw2d( 320, 0 );
	}
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
	// Draw computer vision process
	if ( key == 'd' )
		drawCV = !drawCV;

	// Store background when space bar is pressed
	else if ( key == ' ' )
		bgSubtraction.storeBackground( camera.getImage() );
}