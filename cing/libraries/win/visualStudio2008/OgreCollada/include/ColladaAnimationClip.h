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
 * \file ColladaAnimationClip.h
 * \author Van Aarde Krynauw
 * \date 2008-06-13
 * \brief An animation clip class.
 */

#ifndef COLLADA_ANIMATION_CLIP_H
#define COLLADA_ANIMATION_CLIP_H

#include <iostream>
#include <string>
#include <Ogre.h>
#include "ColladaAnimationTrack.h"

namespace OgreCollada
{	
	/** AnimationClip class. */
	class AnimationClip
	{
		public:
			///Constant string used to reference the unnamed, or playback, animation clip.
			static const Ogre::String UNNAMED_ANIMATION;
			
			/** Default Constructor 
				@param name The name of this animation clip. The animation clip name cannot be changed after creation.
			*/
			AnimationClip(std::string name);
			
			/** Default Destructor */
			~AnimationClip();
			
//			/** Set the animation clip name.
//			 @param name The name of the animation clip.
//			 @remarks This name will be given to the Ogre animation clip.
//			*/
// 			void setName(std::string name);
			
			//NOTE: Functions to handle the animation clip length will be added at a later stage:
			// e.g., getStart / setStart, getEnd/setEnd, generateTimes (used for unnamed clips)
			// until such time, the animation clip length can be retrieved from the FCDAnimationClip pointer.
			
			/** Get the duration of this animation clip by inspecting the animation tracks.
			 * @remarks The length of unnamed animation clips can't be retrieved from the FCDAnimationClip
			 *          simply because there is none. Iterate over the animation tracks and collect the 
			 *          length from the keyframe times.
			 */
			float getDurationFromTracks();
			
			/** Set corresponding FCollada animation clip.
				@param pCLip A pointer to the FCollada animation clip.
				@remarks The FCDAnimationClip pointer is simply used to retrieve the length of the animation clip.
				         TODO: Generate the length from the keyframes for unnamed clips.
			 */
			void setColladaAnimationClip(FCDAnimationClip* pClip);
			
			/** Get corresponding FCollada animation clip.
				@return A pointer to the FCollada animation clip.
			 */
			FCDAnimationClip* getColladaAnimationClip();
			
			/** Set corresponding Ogre animation.
				@param pCLip A pointer to the Ogre animation.
				@remarks This function is simply provided for convenience.
			*/
			void setOgreAnimation(Ogre::Animation* pAnim);
			
			/** Get corresponding Ogre animation.
				@remarks This function is simply provided for convenience.
				@return A pointer to the Ogre animation.
			 */
			Ogre::Animation* getOgreAnimation();
			
			/** Get the animation clip name.
				@return A string containing the animation clip name.
			*/
			std::string getName();
			
			/** Get the number of tracks */
			unsigned int getTrackCount();
			
			/** Get track */
			AnimationTrack* getTrack(unsigned int trackIndex);
			
			/** Get track from the track's Id */
			AnimationTrack* getTrackById(unsigned int id);
			
			/** Create a new animation track
				@param type The type of track to create. Note that this is only used to set a flag.
				@remarks Use this function to create a new animation track and then populate it.
				@return Returns a TransformKeyframeMap structure which is basically just a keyframe sampler. Each
				        TransformKeyframeMap corresponds to one OgreAnimationTrack.
			*/
			AnimationTrack& createTrack(AnimationTrack::TrackType type);
			
			/** Returns whether this animation clip has tracks for scene node animation.
				@return A boolean value that specifies whether this animation clip contains scene node animation tracks.
			 */
			bool hasSceneNodeTracks();
			
			/** Sets whether this animation clip has tracks for scene node animation. [INTERNAL]
				@remarks Do not call this function directly (unless you have a very good reason to do so). This will be called automatically
				         by the an animation track which is a child of this animation clip.
				@param hasTracks A boolean value that specifies whether this animation clip contains scene node animation tracks.
			 */
			
			void _setSceneNodeTracks(bool hasTracks);
			
			/** Returns whether this animation clip has tracks for skeletal animation.
				@return A boolean value that specifies whether this animation clip contains skeletal animation tracks.
			 */
			bool hasSkeletonTracks();
			
			/** Sets whether this animation clip has tracks for skeletal animation. [INTERNAL]
				@remarks Do not call this function directly (unless you have a very good reason to do so). This will be called automatically
				         by the an animation track which is a child of this animation clip.
				@param hasTracks A boolean value that specifies whether this animation clip contains skeletal animation tracks.
			 */
			void _setSkeletonTracks(bool hasTracks);
			
			void setSkeleton(Ogre::Skeleton* pSkeleton) { mSkeleton = pSkeleton; }
			
			Ogre::Skeleton* getSkeleton() { return mSkeleton; }
			
			/** Overload for ostream output. */
			friend std::ostream& operator <<(std::ostream& output, AnimationClip& clip);
			
		private:
			/** Name of this animation clip */
			std::string mClipName;
			/** Store a pointer the Ogre animation clip locally for easy access. */
			Ogre::Animation* mOgreAnimation;
			/** The FCollada animation clip which corresponds to this intermediate animation clip. */
			FCDAnimationClip* mFCAnimationClip;
			/** Whether this animation clip contains scene node tracks */
			bool mHasSceneNodeTracks;
			/** Whether this animation clip contains skeletal node tracks */
			bool mHasSkeletonTracks;
			/** Tracks containing the keyframe samplers. Each AnimationTrack corresponds to an Ogre animation track. */
			std::vector< AnimationTrack* >  mTracks;
			/** If this animation clip belongs to a skeletal, it will be this one */
			Ogre::Skeleton* mSkeleton;
	};
}



#endif
