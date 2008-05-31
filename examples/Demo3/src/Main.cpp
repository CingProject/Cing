// Vision test
#include "Vision.h"

CREATE_APPLICATION( "Vision Test" );

PVCamera        camera;               // Camera input capture
CompVision      cv;                   // Computer Vision system
Object3D        head;                 // 3D head
Object3D        headTarget;           // Object with no mesh, that the head will be looking at
bool            debugComputerVision;  // if true computer vision debug will be drawn

PointLight      pointLight;           // point light
Pos3D           lightPos;             // current light pos

void setup()
{
  // Init camera capture
  camera.init( 320, 240, 25, false );

  // Init compute vision system
  cv.init( 320, 240 );
  debugComputerVision = false;

  // Load the 3d head model
  head.init( "theviewer.mesh", "Examples/BumpMapping/MultiLight" );
  head.setPosition( width / 2, height / 2 );

  // Load head target
  headTarget.init();
  headTarget.setPosition( width / 2, height / 2, 800 );

  // Make head look constantly to the target
  head.trackObject( headTarget );

  // setup light
  lightPos.x = (width  / 2) + 50;
  lightPos.y = height / 2;
  lightPos.z = 100;
  pointLight.init( 1, 1, 1, lightPos.x, lightPos.y, lightPos.z );
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
  if ( nBlos > 0 )
  {
    const Blob& blob = cv.getBlobN( 0 );
    
    // Get blob coordinates
    float blobX     = camera.getWidth() - (blob.bbox.x + (blob.bbox.width / 2));
    float blobTopY  = camera.getHeight() - blob.bbox.y;     

    // Get current light position
    Pos3D pos = pointLight.getPosition();

    // Map new values
    pos.x = map( blobX, 0, camera.getWidth(), 0, width );
    pos.y = map( blobTopY, 0, camera.getHeight(), 0, height );

    // Set new light pos
    pointLight.setPosition( pos );

    // Set the target pos (same than light but further from the head)
    pos.z = 800;
    headTarget.setPosition( pos );
  }
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