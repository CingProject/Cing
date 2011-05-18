/*
-----------------------------------------------------------------------------
This source file is part of OgreCollada (http://ogrecollada.sf.net)

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

/**
 * \file ColladaBonePose.h
 * \author Van Aarde Krynauw
 * \date 2008-08-03
 * \brief A class used to store the bind pose and bind pose stack for bones in a skeleton.
 */

#ifndef COLLADA_BONE_POSE_H
#define COLLADA_BONE_POSE_H

#include <Ogre.h>

namespace OgreCollada
{

	/**
	 * A class used to store the bind pose and bind pose stack for a single bone in a skeleton.
	 */
	class BonePose
	{
		public:
			/** Constructor
			 * \param pBone The Ogre::Bone which corresponds to this bind pose and bind pose stack.
			 */
			BonePose(Ogre::Bone* pBone);
			/** Destructor */
			~BonePose();
			
			/** 
			 * Retrieve the Ogre::Bone that corresponds to this BonePose object.
			 */
			Ogre::Bone* getBone();
			
			/**
			 * Set the object space bind pose. This class will take ownership of the object and delete it upon destruction.
			 */
			void setBindPoseOS(Ogre::Matrix4* pBindPoseOS);
			
			/**
			 * Set the object space bind pose. This class allocate a new matrix and initialise it with the contents of the given matrix reference.
			 */
			void setBindPoseOS(Ogre::Matrix4& bindPoseOS);
			
			/**
			 * Get the object space bind pose matrix pointer. Note that it may be NULL.
			 */
			Ogre::Matrix4* getBindPoseOS();
			
			/**
			 * Set the local space bind pose. This class will take ownership of the object and delete it upon destruction.
			 */
			void setBindPoseLS(Ogre::Matrix4* pBindPoseLS);
			
			/**
			 * Set the local space bind pose. This class allocate a new matrix and initialise it with the contents of the given matrix reference.
			 */
			void setBindPoseLS(Ogre::Matrix4& bindPoseLS);
			
			/**
			 * Get the local space bind pose matrix pointer. Note that it may be NULL.
			 */
			Ogre::Matrix4* getBindPoseLS();
			
			/**
			 * Set the object space bind pose stack. This class will take ownership of the object and delete it upon destruction.
			 */
			void setBindPoseStack(Ogre::Matrix4* pBindPoseStack);
			
			/**
			 * Set the object space bind pose stack. This class allocate a new matrix and initialise it with the contents of the given matrix reference.
			 */
			void setBindPoseStack(Ogre::Matrix4& bindPoseStack);
			
			void applyToBindPoseStack(Ogre::Matrix4* transform);
			
			void applyToBindPoseStack(Ogre::Matrix4& transform);
			
			/**
			 * Get the bind pose stack (all the concatenated bind poses of the bones above this one in the hierarchy). This is used to 
			 * calculate the local space transformation of the bone by multiplying the bind pose in object space to this matrix.
			 */
			Ogre::Matrix4* getBindPoseStack();
			
		private:
			/**
			 * The bind pose for this bone in object space. This pose is retrieved directly from FCollada and stored here without any
			 * processing. If a bone does not have a bind pose, i.e. NULL here, then the Ogre::Bone transformation is used to put on the
			 * bind pose stack.
			 */
			Ogre::Matrix4* m_pBindPoseOS;
			/** 
			 * Local space bind pose
			 */
			Ogre::Matrix4* m_pBindPoseLS;
			
			/**
			 * The bind pose stack at this node. The bind pose stack that is stored here is used to transform this bone from its bind pose
			 * (given by the COLLADA skin controller) to local space.
			 */
			Ogre::Matrix4* m_pBindPoseStack;
			
			/**
			 * The Ogre::Bone that corresponds to this node in the skeleton hierarchy.
			 */
			Ogre::Bone* pBone;
	};
	
}

#endif
