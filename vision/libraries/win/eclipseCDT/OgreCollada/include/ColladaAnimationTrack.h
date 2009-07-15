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
 * \file ColladaAnimationTrack.h
 * \author Van Aarde Krynauw
 * \date 2008-06-19
 * \brief An animation track class.
 */

#ifndef COLLADA_ANIMATION_TRACK_H
#define COLLADA_ANIMATION_TRACK_H

#include <ostream>
#include <string>
#include <Ogre.h>
#include "ColladaTransformKeyframe.h"

namespace OgreCollada
{
	//We need a forward declaration for the back reference to the animation clip that owns this animation track.
	class AnimationClip;
	
	//This maps keyframe times to transformation keyframes.
	typedef std::map<float, TransformKeyframe*> KeyframeMap;
	
	/** AnimationTrack class */
	class AnimationTrack
	{
		public:
			/** Types of animation tracks. A track can have only one type. */
			enum TrackType
			{
				/** Unknown animation track. Your track should never have this type. */
				TT_UNKNOWN,
				/** Scene node animation track. */
				TT_SCENENODE,
				/** Skeletal animation track. */
				TT_SKELETON
			};
			
			/** Default Constructor */
			AnimationTrack(AnimationClip* pAnimClip, TrackType type);
			
			/** Default Destructor */
			~AnimationTrack();
			
			/** Set an ID for this animation track */
			void setId(unsigned int id);
			
			/** Retrieve the ID for this animation track */
			unsigned int getId();
			
			/** Set scene node */
			void setSceneNode(Ogre::SceneNode* pNode);
			
			/** Get scene node */
			Ogre::SceneNode* getSceneNode();
			
			/** Set skeleton */
			void setBone(Ogre::Bone* pBone);
			
			/** Get skeleton */
			Ogre::Bone* getBone();
			
			/** Set the animation track type */
			void setType(TrackType type);
			
			/** Get the animation track type */
			TrackType getType();
			
			/** Calculate track duration from keyframes. */
			float getDuration();
			
			/** Get the start time of the track. */
			float getStart();
			
			/** Get the end time of the track. */
			float getEnd();
			
			/** Determine whether this track has an animated translation
				@remarks Sometimes a skeletal animation in the COLLADA doc doesn't have any translation keyframes
				         so we need to flag the track so that the inverse bind pose won't be applied to the keyframes.
			*/
			bool hasAnimatedTranslation();
			void setHasAnimatedTranslation(bool hasTranslation);
			
			/** Retrieve a keyframe. If that keyframe doesn't exist a new one is created. */
			TransformKeyframe* getKeyframe(float time, bool interpTranslation, bool interpRotation, bool interpScaling);
			
			/** Retrieve the map with the keyframes for iteration purposes */
			KeyframeMap& getKeyframeMap();
			
			/** Retrieve the animation clip to which this animation track belongs */
			AnimationClip* getParentClip() { return mAnimationClip; }
			
			/** Apply a translation to the base translation for this animation track */
			void applyTranslation(const Ogre::Vector3& translation);
			
			/** Apply a scale to the base scale for this animation track */
			void applyScale(const Ogre::Vector3& scale);
			
			/** Apply an orientation to the base orientation for this animation track */
// 			void applyOrientation(const Ogre::Quaternion& orientation);
			void applyOrientation(const Ogre::Matrix3& matrix);
			
			/** Overload for ostream output. */
// 			friend std::ostream& operator <<(std::ostream& output, AnimationTrack& clip);
			
		private:
			/** Animation clip owning this track */
			AnimationClip* mAnimationClip;
			/** An integer to identify this animation track under an animation clip */
			unsigned int mId;
			/** Whether this keyframe contains any translation data whatsoever */
			bool mHasAnimatedTranslation;
			/** A map is used to store the keyframes so that the keyframes can be easily 
			    looked up using their keyframe time as the lookup value. */
			KeyframeMap mKeyframes;
			/** Type of animation for which this track is used. */
			TrackType mTrackType;
			/** The scene to which this animation track is applicable */
			Ogre::SceneNode* mSceneNode;
			/** The skeleton to which this animation track is applicable */
			Ogre::Bone* mBone;
			/** Base transformation to be applied to new keyframes */
			Ogre::Vector3 mBaseTranslate;
			Ogre::Vector3 mBaseScale;
// 			Ogre::Quaternion mBaseOrientation;
			Ogre::Matrix3 mBaseRotation;
	};
}



#endif
