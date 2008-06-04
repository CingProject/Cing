// Vision test
#include "Vision.h"

CREATE_APPLICATION( "Vision Test" );

PVCamera        camera;               // Camera input capture
CompVision      cv;                   // Computer Vision system
Box							box;		
bool            debugComputerVision;  // if true computer vision debug will be drawn

PointLight      pointLight;           // point light
Pos3D           lightPos, targetPos;  // current and target light pos

void setup()
{
  // Init camera capture
  camera.init( 640, 480, 25, false );

  // Init compute vision system
  cv.init( 640, 480 );
  debugComputerVision = false;

  // Create a box
	box.init( 5 );
	//box2.init( 3 );
	box.setPosition( width / 2, height / 2 );
	//box2.setPosition( width / 2, height / 2, -200 );
	box.setDiffuseColor( 255, 0, 0 );
	//box2.setColor( 0, 0, 255 );

	// physics
	box.activatePhysics();

  // setup light
  lightPos.x = (width  / 2) + 50;
  lightPos.y = height / 2;
  lightPos.z = 0;
  targetPos = lightPos;
  //pointLight.init( 1, 1, 1, lightPos.x, lightPos.y, lightPos.z );
}

void draw()
{
  // Update camera capture
  camera.update();

  // Update computer vision process
  cv.update( camera.getImage() );

  // Draw compute vision debug images?
  if ( debugComputerVision )
  {
    camera.draw();
    cv.drawComputerVisionProcess();
  }

  // Get N Blobs
  int nBlos = cv.getNumBlobs();

  // If there is blob -> get the size of the biggest
  float lightIntensity = 0;
  if ( nBlos > 0 )
  {
    const Blob& blob = cv.getBlobN( 0 );
    lightIntensity = map( blob.area, 2000, 6000, 0, 1 );
  }

  // Set light intensity
  //pointLight.setDiffuseColor( lightIntensity, lightIntensity, lightIntensity );

  // If we are on target
  if ( dist( targetPos, lightPos ) < 10 )
  {
    // Calculate random target pos
    targetPos.x += random( -100, 100 );
    targetPos.y += random( -100, 100 );
  }
  
  // Interpolation values
  float speed = 0.1;

  // Modify the position 
  lightPos = (targetPos*speed) + (lightPos * (1-speed) );

  // Move the light
  //pointLight.setPosition( lightPos );
}

void end()
{

}

void mousePressed()
{
  // Store the background
  cv.storeBackground( camera.getImage() );
}

void keyPressed()
{
  if ( key == ' ' )
    debugComputerVision = !debugComputerVision;
}