// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <list>
#include <string>

#include "OgreCollada.h"

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <limits.h>

// TODO: reference additional headers your program requires here
#include "FCollada.h"
#include "FMath/FMath.h"
#include "FCDocument/FCDocument.h"
#include "FCDocument/FCDLibrary.h"
#include "FCDocument/FCDGeometry.h"
#include "FCDocument/FCDGeometryMesh.h"
#include "FCDocument/FCDCamera.h"
#include "FCDocument/FCDEffect.h"
#include "FCDocument/FCDImage.h"
#include "FCDocument/FCDLight.h"
#include "FCDocument/FCDSceneNode.h"
#include "FCDocument/FCDEntityInstance.h"
#include "FCDocument/FCDGeometryInstance.h"
#include "FCDocument/FCDMaterialInstance.h"
#include "FCDocument/FCDControllerInstance.h"
#include "FCDocument/FCDController.h"
#include "FCDocument/FCDSkinController.h"
#include "FCDocument/FCDMorphController.h"
#include "FCDocument/FCDEntity.h"
#include "FCDocument/FCDMaterial.h"
#include "FCDocument/FCDExtra.h"
#include "FCDocument/FCDEffectPass.h"
#include "FCDocument/FCDEffectPassShader.h"
#include "FCDocument/FCDocumentTools.h"

#include "Ogre.h"
#include "OgreLogManager.h"
#include "OgreSceneManager.h"
#include "OgreMeshManager.h"
#include "OgreHardwareBufferManager.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreResourceGroupManager.h"
#include "OgreMaterial.h"
#include "OgreGpuProgram.h"
#include "OgreGpuProgramUsage.h"
#include "OgreTextureManager.h"
#include "OgreTexture.h"
#include "OgrePlugin.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeleton.h"
#include "OgreBone.h"

#include "Collada.h"
#include "ColladaUtils.h"

