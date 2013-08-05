/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ParticleGS.cpp
Description: Demonstrates the use of the geometry shader and render to vertex
	buffer to create a particle system that is entirely calculated on the GPU.
	Partial implementation of ParticlesGS example from Microsoft's DirectX 10
	SDK : http://msdn.microsoft.com/en-us/library/ee416421.aspx
-----------------------------------------------------------------------------
*/

#include "ProceduralManualObject.h"
#include "OgreRenderToVertexBuffer.h"
#include "RandomTools.h"
#include "SamplePlugin.h"
#include "SdkSample.h"

using namespace Ogre;
using namespace OgreBites;

//#define LOG_GENERATED_BUFFER
const Vector3 GRAVITY_VECTOR = Vector3(0, -9.8, 0);

#ifdef LOG_GENERATED_BUFFER
struct FireworkParticle 
{
	float pos[3];
	float timer;
	float type;
	float vel[3];
};
#endif

class _OgreSampleClassExport Sample_ParticleGS : public SdkSample
{
public:
    Sample_ParticleGS() 
	{
		mInfo["Title"] = "Geometry Shader Particle System";
		mInfo["Description"] = "A demo of particle systems using geometry shaders and render to vertex buffers.";
		mInfo["Thumbnail"] = "thumb_particlegs.png";
		mInfo["Category"] = "Effects";
    }

protected:

	void createProceduralParticleSystem()
	{
		mParticleSystem = static_cast<ProceduralManualObject*>
			(mSceneMgr->createMovableObject("ParticleGSEntity", ProceduralManualObjectFactory::FACTORY_TYPE_NAME));
		mParticleSystem->setMaterial("Ogre/ParticleGS/Display");

		//Generate the geometry that will seed the particle system
		ManualObject* particleSystemSeed = mSceneMgr->createManualObject("ParticleSeed");
		//This needs to be the initial launcher particle
		particleSystemSeed->begin("Ogre/ParticleGS/Display", RenderOperation::OT_POINT_LIST);
		particleSystemSeed->position(0,0,0); //Position
		particleSystemSeed->textureCoord(1); //Timer
		particleSystemSeed->textureCoord(0); //Type
		particleSystemSeed->textureCoord(0,0,0); //Velocity
		particleSystemSeed->end();

		//Generate the RenderToBufferObject
		RenderToVertexBufferSharedPtr r2vbObject = 
			HardwareBufferManager::getSingleton().createRenderToVertexBuffer();
		r2vbObject->setRenderToBufferMaterialName("Ogre/ParticleGS/Generate");
		
		//Apply the random texture
		TexturePtr randomTexture = RandomTools::generateRandomVelocityTexture();
		r2vbObject->getRenderToBufferMaterial()->getBestTechnique()->getPass(0)->
			getTextureUnitState("RandomTexture")->setTextureName(
			randomTexture->getName(), randomTexture->getTextureType());

		r2vbObject->setOperationType(RenderOperation::OT_POINT_LIST);
		r2vbObject->setMaxVertexCount(16000);
		r2vbObject->setResetsEveryUpdate(false);
		VertexDeclaration* vertexDecl = r2vbObject->getVertexDeclaration();
		size_t offset = 0;
		offset += vertexDecl->addElement(0, offset, VET_FLOAT3, VES_POSITION).getSize(); //Position
		offset += vertexDecl->addElement(0, offset, VET_FLOAT1, VES_TEXTURE_COORDINATES, 0).getSize(); //Timer
		offset += vertexDecl->addElement(0, offset, VET_FLOAT1, VES_TEXTURE_COORDINATES, 1).getSize(); //Type
		vertexDecl->addElement(0, offset, VET_FLOAT3, VES_TEXTURE_COORDINATES, 2).getSize(); //Velocity
		
		//Bind the two together
		mParticleSystem->setRenderToVertexBuffer(r2vbObject);
		mParticleSystem->setManualObject(particleSystemSeed);

		//Set bounds
		AxisAlignedBox aabb;
		aabb.setMinimum(-100,-100,-100);
		aabb.setMaximum(100,100,100);
		mParticleSystem->setBoundingBox(aabb);
	}

    void testCapabilities(const RenderSystemCapabilities* caps)
    {
        if (!caps->hasCapability(RSC_GEOMETRY_PROGRAM))
        {
			OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Your render system / hardware does not support geometry programs, "
                        "so you cannot run this sample. Sorry!", 
                        "Sample_ParticleGS::createScene");
        }
		if (!caps->hasCapability(RSC_HWRENDER_TO_VERTEX_BUFFER))
        {
			OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Your render system / hardware does not support render to vertex buffers, "
                        "so you cannot run this sample. Sorry!", 
                        "Sample_ParticleGS::createScene");
        }
    }
    
	void setupContent(void)
    {
        demoTime = 0;

        mProceduralManualObjectFactory = OGRE_NEW ProceduralManualObjectFactory();
        Root::getSingleton().addMovableObjectFactory(mProceduralManualObjectFactory);

		createProceduralParticleSystem();

		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mParticleSystem);
		//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mParticleSystem->getManualObject());
		mCamera->setPosition(0,35,-100);
		mCamera->lookAt(0,35,0);
		
		//Add an ogre head to the scene
		SceneNode* ogreHeadSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Entity *ogreHead = mSceneMgr->createEntity("head", "ogrehead.mesh");        
		ogreHeadSN->scale(0.1,0.1,0.1);
		ogreHeadSN->yaw(Degree(180));
		ogreHeadSN->attachObject(ogreHead);
		
		//Add a plane to the scene
		Plane plane;
		plane.normal = Vector3::UNIT_Y;
		plane.d = 100;
		MeshManager::getSingleton().createPlane("Myplane",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
			1500,1500,20,20,true,1,60,60,Vector3::UNIT_Z);
		Entity* pPlaneEnt = mSceneMgr->createEntity( "plane", "Myplane" );
		pPlaneEnt->setMaterialName("Examples/Rockwall");
		pPlaneEnt->setCastShadows(false);
		mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,95,0))->attachObject(pPlaneEnt);
    }

    void cleanupContent()
    {
        Root::getSingleton().removeMovableObjectFactory(mProceduralManualObjectFactory);
        OGRE_DELETE mProceduralManualObjectFactory;
        mProceduralManualObjectFactory = 0;

        MeshManager::getSingleton().remove("Myplane");
    }

	bool frameStarted(const FrameEvent& evt) 
	{ 
		//Set shader parameters
		GpuProgramParametersSharedPtr geomParams = mParticleSystem->
			getRenderToVertexBuffer()->getRenderToBufferMaterial()->
			getBestTechnique()->getPass(0)->getGeometryProgramParameters();
        if (geomParams->_findNamedConstantDefinition("elapsedTime"))
        {
            geomParams->setNamedConstant("elapsedTime", evt.timeSinceLastFrame);
		}		
        demoTime += evt.timeSinceLastFrame;
		if (geomParams->_findNamedConstantDefinition("globalTime"))
		{
			geomParams->setNamedConstant("globalTime", demoTime);
		}
		if (geomParams->_findNamedConstantDefinition("frameGravity"))
		{
			geomParams->setNamedConstant("frameGravity", GRAVITY_VECTOR * evt.timeSinceLastFrame);
		}		
		return SdkSample::frameStarted(evt); 
	}

#ifdef LOG_GENERATED_BUFFER
	bool frameEnded(const FrameEvent& evt) 
	{ 
		//This will only work if the vertex buffer usage is dynamic (see R2VB implementation)
		LogManager::getSingleton().getDefaultLog()->stream() << 
			"Particle system for frame " <<	Root::getSingleton().getNextFrameNumber();
		RenderOperation renderOp;
		mParticleSystem->getRenderToVertexBuffer()->getRenderOperation(renderOp);
		const HardwareVertexBufferSharedPtr& vertexBuffer = 
			renderOp.vertexData->vertexBufferBinding->getBuffer(0);
		
		assert(vertexBuffer->getVertexSize() == sizeof(FireworkParticle));
		FireworkParticle* particles = static_cast<FireworkParticle*>
			(vertexBuffer->lock(HardwareBuffer::HBL_READ_ONLY));
		
		for (size_t i=0; i<renderOp.vertexData->vertexCount; i++)
		{
			FireworkParticle& p = particles[i];
			LogManager::getSingleton().getDefaultLog()->stream() <<
				"FireworkParticle " << i+1 << " : " <<
				"Position : " << p.pos[0] << " " << p.pos[1] << " " << p.pos[2] << " , " <<
				"Timer : " << p.timer << " , " <<
				"Type : " << p.type << " , " <<
				"Velocity : " << p.vel[0] << " " << p.vel[1] << " " << p.vel[2];
		}
		
		vertexBuffer->unlock();
		return SdkSample::frameEnded(evt); 
	}
#endif
    
    Real demoTime;
    ProceduralManualObject* mParticleSystem;
    ProceduralManualObjectFactory *mProceduralManualObjectFactory;
};

#ifndef OGRE_STATIC_LIB

SamplePlugin* sp;
Sample* s;

extern "C" _OgreSampleExport void dllStartPlugin()
{
	s = new Sample_ParticleGS;
	sp = OGRE_NEW SamplePlugin(s->getInfo()["Title"] + " Sample");
	sp->addSample(s);
	Root::getSingleton().installPlugin(sp);
}

extern "C" _OgreSampleExport void dllStopPlugin()
{
	Root::getSingleton().uninstallPlugin(sp); 
	OGRE_DELETE sp;
	delete s;
}
#endif

