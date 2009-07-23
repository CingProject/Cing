// An advanced example to show some short of communication between OGRE and Cing
// It loads a Collada scene and show how to play 3D animations.
#include "Cing.h"

// OGRE and Collada includes
#include "Ogre3d/include/OgreAnimation.h"  
#include "Ogre3d/include/OgreRoot.h"  
#include "Ogre3d/include/OgreEntity.h"  

// Collada includes
#include "OgreCollada/include/OgreCollada.h"

CREATE_APPLICATION( "Cing" );

// Animation time control
float	currentTime				= 0.0f;

// This object allow to register events for node,entity, camera and light creation 
class ColladaListener: public OgreCollada::IResourceNotification
{
public:
	ColladaListener(){};
	~ColladaListener(){};
	// Callback functions:
	void sceneNodeCreated(Ogre::SceneNode* pNode, FCDSceneNode* pFColladaSceneNode) {
		println("------------------------"); 
		println(pNode->getName().c_str());
		println(" created");
		Ogre::String name     = pNode->getName();
		Ogre::String theOther = "Collada.door";

		// Check if this SceneNode is the parent of the hierarchy
		int ret = strcmp( name.c_str(), theOther.c_str() );
		if (ret == 0)	{
			pNode->setPosition(width/2,height/2,0);
			// Set scale and position
			pNode->setScale(50,-50,50);
		}
	};
	void entityCreated(Ogre::Entity* entity, FCDSceneNode* colladaParentNode) {};
    void cameraCreated(Ogre::Camera* pCam, FCDCamera* pFColladaCamera){};
	void lightCreated(Ogre::Light* pLight, FCDLight* pFColladaLight){};
};
ColladaListener* colladaListener = NULL;
// Loads a Collada file
bool loadCollada( const String& fileName )
{
	println("Loading collada scene: " );	println( fileName.c_str() );

	// Create importer
	OgreCollada::ImpExp *pImporterExporter = OgreCollada::CreateImpExp(Ogre::Root::getSingletonPtr(), ogreSceneManager);

	// Create the listener
	colladaListener = new ColladaListener();

	//Register the IResourceNotification instance with the OgreCollada importer
	pImporterExporter->setResourceNotificationListener( (OgreCollada::IResourceNotification*)colladaListener );

	// Import collada file and check result
	bool success = pImporterExporter->importCollada( dataFolder + fileName, "Collada." );
	if ( !success )
		println( "Error loading %s. It should be in the data folder", fileName.c_str() );

	OgreCollada::DestroyImpExp(pImporterExporter);

	return success;
};

// Update node animations
void updateAnimations()
{
	// Update animations contained in the sceneManager of the m_activeScene
	Ogre::SceneManager::AnimationIterator animationIt = ogreSceneManager->getAnimationIterator();	
	while(animationIt.hasMoreElements())
	{
		// Iterate through all animations
		Ogre::Animation* animation = animationIt.getNext();
		const Ogre::Animation::NodeTrackList& trackList = animation->_getNodeTrackList();

		Ogre::Animation::NodeTrackList::const_iterator it = trackList.begin();
		Ogre::Animation::NodeTrackList::const_iterator iend = trackList.end();

		// and all animation tracks
		for(; it != iend; ++it) {
			const Ogre::NodeAnimationTrack* track = it->second;
			track->getAssociatedNode()->resetToInitialState();
		}
		// finally, play animation
		currentTime += elapsedSec/4.0;
		animation->apply( currentTime );
	}
}

PointLight	light;	

void setup()
{
	size( 800, 600, OPENGL );

	// Enables camera control
	useKeyboardCameraControl(true);
	showDebugOutput(true);

	// Init lights and set the ambient light
	ambientLight( 255, 255, 255 );
	light.init( 255, 255, 255, width / 2, height / 2, -200 );	

	// Loads collada a file
	bool success = loadCollada( "DoorAnimation.dae" );
	if ( success ) println(" Loading success!" );
}

void draw()
{
	stroke( 255,0,0 );
	line( 0,0,0, 100,0,0 );
	// Update node animations
	updateAnimations();
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