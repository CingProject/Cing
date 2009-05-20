#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

#include "externLibs/OgreCollada/include/OgreCollada.h"
#include "externLibs/Ogre3d/include/OgreRoot.h"

void setup()
{
  size( 800, 600 );

  // Enable camera control
  useKeyboardCameraControl(true);
  useMouseCameraControl(true);

  // Create importer
  OgreCollada::ImpExp *pImporterExporter = OgreCollada::CreateImpExp(Ogre::Root::getSingletonPtr(), ogreSceneManager);

  // Import collada file and check result
  String colladaFile = "DoorAnimation.dae";
  bool success = pImporterExporter->importCollada( dataFolder + colladaFile, "import1" );
  if ( !success )
    LOG_ERROR( "Error loading %s. It should be in the data folder", colladaFile.c_str() );

   
  OgreCollada::DestroyImpExp(pImporterExporter);
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
}