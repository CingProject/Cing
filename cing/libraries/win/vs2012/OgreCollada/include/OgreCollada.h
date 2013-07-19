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

#if !defined(__OGRECOLLADA_H__)
#define __OGRECOLLADA_H__

// Comment out the following if you do not wish to have Cg as a dependency. 
// Note that COLLADA FX programmable effects depend on either Cg or GLSL (no
// support in COLLADA is defined for HLSL), so without either of these, you
// will be limited to fixed-function profile_COMMON effects only.
// Note also that currently there is no support for GLSL in OgreCollada. In
// the future there may also be support for translating Cg programs to HLSL
// since the syntaxes are virtually identical.
#define USE_CG

// Ogre forward declarations
namespace Ogre
{
	class Root;
	class SceneManager;
	class Camera;
	class Light;
	class Image;
	class SceneNode;
	class Entity;
	class Resource;
}

class FCDEntity;
class FCDLight;
class FCDCamera;
class FCDSceneNode;

namespace OgreCollada
{
#if defined(UNICODE)
	typedef unsigned short char_t;
#else
	typedef char char_t;
#endif

	// other basic types
	typedef unsigned int		UInt32;
	typedef unsigned short		UInt16;
	typedef unsigned char		UInt8;
	typedef int					Int32;
	typedef short				Int16;
	typedef char				Int8;

	// interested code can register to find out when new resources are created (for example, command-line
	// COLLADA converters that want to serialize new objects such as Mesh and Skeleton to disk files)
	class IResourceNotification
	{
	public:
		IResourceNotification() {}
		virtual ~IResourceNotification() {}

		virtual void cameraCreated(Ogre::Camera* pCam, FCDCamera* pFColladaCamera){}
		virtual void lightCreated(Ogre::Light* pLight, FCDLight* pFColladaLight){}
		virtual void sceneNodeCreated(Ogre::SceneNode* pNode, FCDSceneNode* pFColladaSceneNode){}
		virtual void entityCreated(Ogre::Entity* pEntity, FCDSceneNode* pFColladaParentNode){}
		virtual void resourceCreated(Ogre::Resource* pResource, FCDEntity* pFColladaObject){}
		virtual void getDefaultTexture(Ogre::Image& rImage){}
	};

	class BindingManager;
	class DataStorage;

	class ImpExp
	{
	public:

		virtual ~ImpExp(){}

		typedef enum
		{
			NONE				= 0x00000000,		// import nothing from the Collada document
			ALL					= 0xFFFFFFFF,		// (DEFAULT) import everything from the Collada document (all objects and the visual scene)
			GEOMETRY			= 0x00000001,		// import geometry (creates Ogre Mesh/SubMesh)

			EFFECT				= 0x00000002,		// import effect(s) only (creates Ogre GpuProgram(s), if any)
			MATERIAL			= 0x00000004,		// import materials -- includes Effect(s) (creates FF and programmable Ogre Material(s))

			SKELETON			= 0x00000010,		// import skeleton(s) (creates Ogre Skeleton(s))
			ANIMATION			= 0x00000020		// import animations -- includes Effect(s) (creates Ogre Animation/Tracks/etc)
		} ImportFlags;

		// these two methods are used for deferred initialization/SM swapping
		virtual void setRoot(Ogre::Root* pRoot) = 0;
		virtual void setSceneManager(Ogre::SceneManager* pSceneMgr) = 0;

		// If you plan to write out your COLLADA doc with additional custom data within the 'extra'
		// nodes, you need to set this profile name. If the profile name is left to its default of 
		// blank, no additional profile techniques will be added to the document.
		virtual void setCustomProfileName(const std::string& name) = 0;

		// tell OgreCollada what resource group name to use for subsequent imports
		virtual void setResourceGroupName(const std::string& name) = 0;

		// Import the COLLADA document into the Ogre scene
		virtual bool importCollada(const std::string daeName, const std::string prefix = "", ImportFlags flags=ALL) = 0;

		// Exports the COLLADA document -- note that this does NOT serialize the Ogre scene manager;
		// if you intend for changes in the Ogre scene to be reflected in the COLLADA document, you need
		// to alter, on your own, the FCollada FCDEntity* that gets attached to every Ogre::MovableObject
		// during document import. If you do not supply a document name, the original imported document is
		// overwritten.
		virtual bool exportCollada(const char_t* pDaeName=0) = 0;

		virtual void setResourceNotificationListener(IResourceNotification* pCallback=0) = 0;
	};

	ImpExp* CreateImpExp(Ogre::Root* pRoot=0, Ogre::SceneManager* pSm=0);
	void DestroyImpExp(ImpExp* pImpExp);

	// declare the managed wrapper for OgreCollada
	#if defined(_CLR)
	namespace Managed
	{
		using namespace System;
		using namespace Mogre;

		// called when OgreCollada creates a Mogre Camera object
		public delegate void OnCameraCreated(Camera ^ camera, IntPtr ^ pFColladaCamera);
		// called when OgreCollada creates a Mogre Light object
		public delegate void OnLightCreated(Light ^ light, IntPtr ^ pFColladaLight);
		// called when OgreCollada creates a Mogre SceneNode object
		public delegate void OnSceneNodeCreated(SceneNode ^ node, IntPtr ^ pFColladaSceneNode);
		// called when OgreCollada creates a Mogre Entity object
		public delegate void OnEntityCreated(Entity ^ entity, IntPtr ^ pFColladaParentNode);
		// called when OgreCollada creates a Mogre Resource object
		public delegate void OnResourceCreated(Resource ^ resource, IntPtr ^ pFColladaObject);
		// called when OgreCollada needs a default for a missing texture
		public delegate void GetDefaultTexture(Image ^ image);

		class ResourceListener;
		public ref class OgreCollada
		{
		public:
			event OnCameraCreated ^ CameraCreated;
			event OnLightCreated ^ LightCreated;
			event OnSceneNodeCreated ^ SceneNodeCreated;
			event OnEntityCreated ^ EntityCreated;
			event OnResourceCreated ^ ResourceCreated;
			event GetDefaultTexture ^ NeedsDefaultTexture;

			enum class ImportFlags
			{
				None				= 0x00000000,		// import nothing from the Collada document
				All					= 0xFFFFFFFF,		// (DEFAULT) import everything from the Collada document (all objects and the visual scene)
				Geometry			= 0x00000001,		// import geometry (creates Ogre Mesh/SubMesh)

				Effect				= 0x00000002,		// import effect(s) only (creates Ogre GpuProgram(s), if any)
				Material			= 0x00000004,		// import materials -- includes Effect(s) (creates FF and programmable Ogre Material(s))

				Skeleton			= 0x00000010,		// import skeleton(s) (creates Ogre Skeleton(s))
				Animation			= 0x00000020,		// import animations -- includes Effect(s) (creates Ogre Animation/Tracks/etc)
			};

			OgreCollada();		// default c'tor
			~OgreCollada();		// d'tor
			!OgreCollada();		// Dispose() handler

			// pointer to the Mogre Root object to be used by this instance of OgreCollada
			property Mogre::Root ^ Root;
			// pointer to the Mogre SceneManager object to be used by this instance of OgreCollada
			property Mogre::SceneManager ^ SceneManager;
			// used to provide a different profile name for export
			property System::String ^ CustomProfileName;

			// import a COLLADA file (all elements are imported)
			bool Import(System::String ^ daeFileName);
			// import a COLLADA file (only elements specified by the bitmask in flags, are imported)
			bool Import(System::String ^ daeFileName, ImportFlags flags);
			// export a COLLADA file using the name of the current COLLADA file (all elements exported)
			bool Export();
			// export a COLLADA file using the provided filename of the current COLLADA file (all elements exported)
			bool Export(System::String ^ daeFileName);
			// export a COLLADA file using the provided filename of the current COLLADA file (only elements specified by the bitmask in flags are exported)
			bool Export(System::String ^ daeFileName, ImportFlags flags);

		protected:
		private:
			OgreCollada::ImpExp* m_pImpExp;
			//ResourceListener* m_pListener;
		};
		
	}
	#endif
}


#endif //__OGRECOLLADA_H__

