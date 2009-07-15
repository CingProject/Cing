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
 * \file ColladaTransformKeyframe.h
 * \author Van Aarde Krynauw
 * \date 2008-06-13
 * \brief A Transform keyframe class.
 */

#ifndef COLLADA_TRANSFORM_KEYFRAME_H
#define COLLADA_TRANSFORM_KEYFRAME_H

namespace OgreCollada
{
	class TransformKeyframe
	{
		public:
			/** Default Constructor */
			TransformKeyframe();
			/** Default Destructor */
			~TransformKeyframe();
		
			float time;
			Ogre::Vector3 translate;
			Ogre::Vector3 scale;
// 			Ogre::Quaternion orientation;
			Ogre::Matrix3 rotation;
			
			//Rotations are stored as Euler Angles (deprecated)
			//Scene node animation still use this, though. But scene node animation must be replaced with 
			//quaternion rotations.
// 			Ogre::Vector3 axis;
			
			std::string toString();
		
			bool operator==(TransformKeyframe& other);
			
			bool operator!=(TransformKeyframe& other);
			
			bool operator<(TransformKeyframe& other);
	};
	
/****************************************************************************/
	
	//This maps keyframe times to transformation keyframes.
	typedef std::map<float, TransformKeyframe> TransformKeyframeMap;
}



#endif
