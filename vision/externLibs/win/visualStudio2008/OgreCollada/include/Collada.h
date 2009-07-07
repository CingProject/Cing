/*
-----------------------------------------------------------------------------
This source file is part of NESE
    (Networked Extensible Simulation Environment)

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgements in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

-----------------------------------------------------------------------------
*/

class FCDEffect;
class FCDMaterial;
class FCDGeometry;
class FCDImage;
class FCDocument;
class FCDGeometryInstance;
class FCDControllerInstance;
class FCDMaterialInstance;
class FCDEntityInstance;
class FCDEffectProfile;
class FCDEffectStandard;
class FCDTexture;
class FCDAnimationClip;
class FCDAnimationInstance;

#include "ColladaDataStorage.h"
#include "ColladaTransformKeyframe.h"
#include "ColladaBonePose.h"

/** \brief OgreCollada library and supporting utilities.
 * @author Van Aarde Krynauw
 */
namespace OgreCollada
{
	// utility funcs/macros
	const char_t* convertString(const wchar_t* pUnicode);
	const wchar_t* convertString(const char_t* pAnsi);

#if defined(UNICODE)
#	define CONVERT_STRING(wc) (convertString(wc))
#else
#	define CONVERT_STRING(wc) (wc)
#endif

	/** Importer / Exporter class for OgreCollada */
	class ImpExpImpl : public ImpExp
	{
	public:

		/** Constructor */
		ImpExpImpl(Ogre::Root* pRoot, Ogre::SceneManager* pSceneMgr);
		/** Destructor */
		virtual ~ImpExpImpl();

		/** these two methods are used for deferred initialization/SM swapping */
		void setRoot(Ogre::Root* pRoot);					
		void setSceneManager(Ogre::SceneManager* pSceneMgr);

		// If you plan to write out your COLLADA doc with additional custom data within the 'extra'
		// nodes, you need to set this profile name. If the profile name is left to its default of 
		// blank, no additional profile techniques will be added to the document.
		void setCustomProfileName(const std::string& name);

		void setResourceGroupName(const std::string& name);

		// Import the COLLADA document into the Ogre scene
		bool importCollada(const std::string daeName, const std::string prefix = "", ImportFlags flags=ALL);

		// Exports the COLLADA document -- note that this does NOT serialize the Ogre scene manager;
		// if you intend for changes in the Ogre scene to be reflected in the COLLADA document, you need
		// to alter, on your own, the FCollada FCDEntity* that gets attached to every Ogre::MovableObject
		// during document import. If you do not supply a document name, the original imported document is
		// overwritten.
		bool exportCollada(const char_t* pDaeName=0);

		void setResourceNotificationListener(IResourceNotification* pCallback=0);

	protected:
		//////////////////////////////////////////////////////////
		// Import methods
		//////////////////////////////////////////////////////////
		void createScene();
		void createObjects();
		void createPhysics();

		void createScene(FCDocument* pDoc);
		void processVisualScene(FCDSceneNode* pSceneRoot);

		void processGeometryInstance(FCDGeometryInstance* pGeomInst);
		void processControllerInstance(FCDControllerInstance* pControllerInst);
		void processMaterialInstance(FCDMaterialInstance* pMaterialInst);
		void processAnimationInstance(FCDAnimationInstance* pAnimInstance);
		Ogre::Light* processLightInstance(FCDEntityInstance* pLightInstance);
		Ogre::Camera* processCameraInstance(FCDEntityInstance* pCameraInstance);

		void createBasicShading(Ogre::Pass* pPass, FCDEffectStandard* pEffect);
		void addTextureStage(Ogre::Pass* pPass, FCDTexture* pTex);
		void addEffect(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile, const std::string& preferredTechnique);
		void addTexture(FCDImage* pImage);
		void createConstantTechnique(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile);
		void createBlinnTechnique(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile);
		void createLambertTechnique(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile);
		void createPhongTechnique(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile);
		void createCgTechnique(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile, const std::string& preferredTechnique);
		void createTechnique(Ogre::MaterialPtr pMtl, FCDEffectProfile* pProfile, const std::string& preferredTechnique);
		void createMaterial(Ogre::MaterialPtr pMtl, FCDMaterial* pMaterial);
		Ogre::Material* addMaterial(FCDMaterialInstance* pMaterialInst);
		Ogre::Mesh* createMesh(FCDGeometryInstance* pGeomInst, Ogre::Matrix4& bindShapeMatrix);
		void createSkinnedMesh(FCDControllerInstance* pControllerInst);
		void skinSubmesh(Ogre::SubMesh* pSubMesh, FCDSkinController* pSkinController, DataStorage::SingleToMultiMap& remappedIndices, Ogre::Matrix4& bindShapeMatrix);
		Ogre::Mesh* createMesh(FCDControllerInstance* pControllerInst);
		Ogre::Skeleton* createSkeleton(FCDSceneNode* pRootBone);
		void setNodeTransform(Ogre::Node* pOgreNode, FCDSceneNode* pNode);
		void setNodeTransform(Ogre::Node* pOgreNode, FMMatrix44& mat);
		void addTracksToMap(std::set<std::string>& animClipNames, std::map<std::string, unsigned int>& animTrackMap, Ogre::Node* pNode, AnimationTrack::TrackType trackType);
		void copyCurveData(std::string qualifier, FCDTransform* pTransform, std::map<std::string, unsigned int>& animTrackMap, Ogre::Skeleton* pSkeleton = 0);
		void applyNodeTransformToTracks(Ogre::Node* pNode, std::map<std::string, unsigned int>& animTrackMap);
		void getClipNamesFromNode(FCDSceneNode* pNode, std::set<std::string>& clipNames);
		void createAnimationTracks(Ogre::Node* pOgreNode, FCDSceneNode* pNode);
		void populateSkeleton(Ogre::SkeletonPtr pSkeleton, Ogre::Bone* pBone, FCDSceneNode* pFCBone);
		void processOgreSkeleton(Ogre::Skeleton* pSkeleton);
		void createOgreAnimations();
		void createSkeletonAnimationTracks(Ogre::SkeletonPtr pSkeleton, Ogre::Bone* pBone, FCDSceneNode* pFCNode);
		void createNodeAnimationTracks(Ogre::SceneNode* pNode, FCDSceneNode* pFCNode);
		void collectTranslationAnimation(FCDAnimated* pAnim, std::map<std::string, unsigned int>& animTrackMap);
		void collectRotationAnimation(FCDAnimated* pAnim, std::map<std::string, unsigned int>& animTrackMap);
		void collectScaleAnimation(FCDAnimated* pAnim, std::map<std::string, unsigned int>& animTrackMap);
		void addAnimationTrack(Ogre::Animation* pAnim, float startTime, Ogre::Node* pNode, FCDSceneNode* pFCNode, bool bRelative);
		void bindParameters(FCDEffectPassShader* pProgram, Ogre::Pass* pPass, Ogre::GpuProgramParametersSharedPtr pCgParams);
		void getNodeTransform(Ogre::Node* pNode, Ogre::Matrix4& rMat);
		void setNodeTransform(Ogre::Node* pNode, Ogre::Matrix4& rMat);
		
		//ColladaScene.cpp functions:
		Ogre::Entity* createEntity(Ogre::SceneManager* pSm, const FCDGeometryInstance* pInst);
		Ogre::Entity* createEntity(Ogre::SceneManager* pSm, FCDControllerInstance* pInst);

		//////////////////////////////////////////////////////////
		// TODO: Export methods (if any needed)
		//////////////////////////////////////////////////////////

		// mesh options
		bool m_bGenerateTangents;	// have Ogre generate vertex tangent data for mesh data (on import or export)
		bool m_bBuildEdgeLists;		// have Ogre generate edge lists from mesh data for shadow volumes (import or export)

		// skeleton options
		bool m_bOptimizeSkeleton;	// remove unused or redundant bones in skeletons (import, export)

		// animation options
		bool m_bOptimizeAnimations;	// remove redundant keyframe data from keyframed animations (import, export)
		float m_fOptimizationFactor;	// set the variation tolerance for keyframe optimization

		// 'profile' name to use when creating additional "extra" techniques
		std::string	m_customProfileName;	
		// Prefix given to all imported entities
		std::string m_prefix;
		// Ogre Root object instance
		Ogre::Root*	m_pRoot;
		// Ogre Scene Manager to use for import/export of scene/objects
		Ogre::SceneManager*		m_pSceneMgr;
		// pointer to the actual FCollada COLLADA document instance
		FCDocument* m_pDoc;
		// pointer to resource-creation notification callback
		IResourceNotification* m_pCallback;
		// monotonically-increasing counter to make unique names when needed
		UInt32 m_uniqueCounter;
		// track geometry and material input/parameter mappings
		BindingManager* m_pBindingManager;
		// storage of data during various COLLADA processing stages
		DataStorage* m_pDataStorage; 
		// track the material instance with which we currently are working 
		FCDMaterialInstance* m_pCurrentMtlInst;

		// LUT for found skeletons
		std::map<std::string, Ogre::Skeleton*> m_skeletons;
		// tracking bone indices during construction on a per-skeleton basis
		UInt32 m_nextBoneIndex;
		// mapping of bones to parent skeleton since Ogre doesn't provide
		std::map<Ogre::Bone*, Ogre::Skeleton*>	m_boneToSkeletonLUT;
		// mapping of bones to animation tracks for applying the inverse bind pose (ColladaController.cpp)
		std::map<Ogre::Bone*, std::list<AnimationTrack*> >	m_boneToAnimationTracksLUT;
		// mappings of bone node name (sid) to the Ogre bone in question
		std::map<std::string, Ogre::Bone*> m_nameToBoneLUT;
		// Mapping of bones to BonePose structures
		std::map<Ogre::Bone*, BonePose*> m_boneToPoseLUT;
		// A list of entities that will need their skeletons rebuilt
		std::list<Ogre::Entity*> m_entityList;
		
		//A counter that is added to scene node names to ensure that they are unique without 
		//having to check with Ogre everytime.
		unsigned int m_UniqueNameCounter;

		// name of Ogre resource group in which to stick imported resources
		std::string m_resGroupName;
	};
}

