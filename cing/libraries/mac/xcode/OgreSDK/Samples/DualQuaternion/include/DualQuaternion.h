#ifndef __DualQuaternion_Sample_H__
#define __DualQuaternion_Sample_H__

#include "SdkSample.h"

#if defined(USE_RTSHADER_SYSTEM) && defined(RTSHADER_SYSTEM_BUILD_EXT_SHADERS)
#include "OgreShaderExHardwareSkinning.h"
#endif

using namespace Ogre;
using namespace OgreBites;

class _OgreSampleClassExport Sample_DualQuaternion : public SdkSample
{
public:
	Sample_DualQuaternion() : ent(0), entDQ(0), totalTime(0)
#if defined(USE_RTSHADER_SYSTEM) && defined(RTSHADER_SYSTEM_BUILD_EXT_SHADERS)
		, mSrsHardwareSkinning(0)
#endif
	{
		mInfo["Title"] = "Dual Quaternion Skinning";
		mInfo["Description"] = "A demo of the dual quaternion skinning feature in conjunction with the linear skinning feature.";
		mInfo["Thumbnail"] = "thumb_dualquaternionskinning.png";
		mInfo["Category"] = "Animation";
	}

	bool frameRenderingQueued(const FrameEvent& evt)
	{
		const Real start = 30;
		const Real range = 145;
		const Real speed = 1;
		const Vector3 vec = Vector3(1,0.3,0).normalisedCopy();
		totalTime += evt.timeSinceLastFrame;
		Quaternion orient = Quaternion(Degree(start + Ogre::Math::Sin(totalTime * speed) * range), vec);
		ent->getSkeleton()->getBone("Bone02")->setOrientation(orient);
		entDQ->getSkeleton()->getBone("Bone02")->setOrientation(orient);
				
		return SdkSample::frameRenderingQueued(evt);
	}


protected:

	void setupContent()
	{
#if defined(USE_RTSHADER_SYSTEM) && defined(RTSHADER_SYSTEM_BUILD_EXT_SHADERS)
        //Add the hardware skinning to the shader generator default render state
        mSrsHardwareSkinning = mShaderGenerator->createSubRenderState(Ogre::RTShader::HardwareSkinning::Type);
        Ogre::RTShader::RenderState* renderState = mShaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
        renderState->addTemplateSubRenderState(mSrsHardwareSkinning);

        Ogre::MaterialPtr pCast1 = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_dq_skinning_1weight_twophase");
        Ogre::MaterialPtr pCast2 = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_dq_skinning_2weight_twophase");
        Ogre::MaterialPtr pCast3 = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_dq_skinning_3weight_twophase");
        Ogre::MaterialPtr pCast4 = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_dq_skinning_4weight_twophase");

        Ogre::RTShader::HardwareSkinningFactory::getSingleton().setCustomShadowCasterMaterials(RTShader::ST_DUAL_QUATERNION, pCast1, pCast2, pCast3, pCast4);

        Ogre::MaterialPtr pCast1l = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_skinning_1weight");
        Ogre::MaterialPtr pCast2l = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_skinning_2weight");
        Ogre::MaterialPtr pCast3l = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_skinning_3weight");
        Ogre::MaterialPtr pCast4l = Ogre::MaterialManager::getSingleton().getByName("Ogre/RTShader/shadow_caster_skinning_4weight");

        Ogre::RTShader::HardwareSkinningFactory::getSingleton().setCustomShadowCasterMaterials(RTShader::ST_LINEAR, pCast1l, pCast2l, pCast3l, pCast4l);
#endif
		// set shadow properties
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
		mSceneMgr->setShadowTextureSize(2048);
		mSceneMgr->setShadowColour(ColourValue(0.6, 0.6, 0.6));
		mSceneMgr->setShadowTextureCount(1);

		// add a little ambient lighting
		mSceneMgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

		SceneNode* lightsBbsNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		BillboardSet* bbs;

		// Create billboard set for lights .
		bbs = mSceneMgr->createBillboardSet();
		bbs->setMaterialName("Examples/Flare");
		lightsBbsNode->attachObject(bbs);

 		Light* l = mSceneMgr->createLight();
		Vector3 dir;
 		l->setType(Light::LT_POINT);
 		l->setPosition(30, 70, 40);
 		dir = -l->getPosition();
 		dir.normalise();
 		l->setDirection(dir);
 		l->setDiffuseColour(1, 1, 1);
 		bbs->createBillboard(l->getPosition())->setColour(l->getDiffuseColour());

		// create a floor mesh resource
		MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_Y, -1), 250, 250, 25, 25, true, 1, 15, 15, Vector3::UNIT_Z);

		// add a floor to our scene using the floor mesh we created
		Entity* floor = mSceneMgr->createEntity("Floor", "floor");
		floor->setMaterialName("Examples/Rockwall");
		floor->setCastShadows(false);
		mSceneMgr->getRootSceneNode()->attachObject(floor);

		// set camera initial transform and speed
		mCamera->setPosition(100, 20, 0);
		mCamera->lookAt(0, 10, 0);
		mCameraMan->setTopSpeed(50);

		setupModels();
	}

	void setupModels()
	{
		SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->translate(0, 0, 20, Node::TS_LOCAL);

		//Create and attach a spine entity with standard skinning
		ent = mSceneMgr->createEntity("Spine", "spine.mesh");
		ent->setMaterialName("spine");
		ent->getSkeleton()->getBone("Bone02")->setManuallyControlled(true);
		sn->attachObject(ent);
		sn->scale(Vector3(0.2,0.2,0.2));

		sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->translate(0, 0, -20, Node::TS_LOCAL);

		//Create and attach a spine entity with dual quaternion skinning
		entDQ = mSceneMgr->createEntity("SpineDQ", "spine.mesh");
		entDQ->setMaterialName("spineDualQuat");
		entDQ->getSkeleton()->getBone("Bone02")->setManuallyControlled(true);
		sn->attachObject(entDQ);
		sn->scale(Vector3(0.2,0.2,0.2));
		
#if defined(USE_RTSHADER_SYSTEM) && defined(RTSHADER_SYSTEM_BUILD_EXT_SHADERS)
        //In case the system uses the RTSS, the following line will ensure
        //that the entity is using hardware animation in RTSS as well.
        RTShader::HardwareSkinningFactory::getSingleton().prepareEntityForSkinning(ent);
        RTShader::HardwareSkinningFactory::getSingleton().prepareEntityForSkinning(entDQ, RTShader::ST_DUAL_QUATERNION, false, true);

        //The following line is needed only because the spine models' materials have shaders and
        //as such is not automatically reflected in the RTSS system		
        RTShader::ShaderGenerator::getSingleton().createShaderBasedTechnique(
            ent->getSubEntity(0)->getMaterialName(),
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            true);
        
        RTShader::ShaderGenerator::getSingleton().createShaderBasedTechnique(
            entDQ->getSubEntity(0)->getMaterialName(),
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            true);
#endif

		// create name and value for skinning mode
		StringVector names;
		names.push_back("Skinning");
		String value = "Software";

		// change the value if hardware skinning is enabled
		MaterialPtr dqMat = ent->getSubEntity(0)->getMaterial();
		if(!dqMat.isNull())
		{
			Technique* bestTechnique = dqMat->getBestTechnique();
			if(bestTechnique)
			{
				Pass* pass = bestTechnique->getPass(0);
				if (pass && pass->hasVertexProgram() && pass->getVertexProgram()->isSkeletalAnimationIncluded())
				{
					value = "Hardware";
				}
			}
		}

		// create a params panel to display the skinning mode
		mTrayMgr->createParamsPanel(TL_TOPLEFT, "Skinning", 170, names)->setParamValue(0, value);
	}

	void cleanupContent()
	{
		MeshManager::getSingleton().remove("floor");

#if defined(USE_RTSHADER_SYSTEM) && defined(RTSHADER_SYSTEM_BUILD_EXT_SHADERS)
        Ogre::RTShader::RenderState* renderState = mShaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
        renderState->removeTemplateSubRenderState(mSrsHardwareSkinning);
#endif
	}

	Entity* ent;
	Entity* entDQ;

	Real totalTime;

#if defined(USE_RTSHADER_SYSTEM) && defined(RTSHADER_SYSTEM_BUILD_EXT_SHADERS)
	RTShader::SubRenderState* mSrsHardwareSkinning;
#endif
};

#endif
