/* -------------------------------------------------------
Copyright (c) 2012 Alberto G. Salguero (alberto.salguero at uca.es)

Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions of
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------- */

#ifndef _SphericCameraMode_H_
#define _SphericCameraMode_H_

#include "CCSPrerequisites.h"

#include "CCSCameraControlSystem.h"

namespace CCS
{
	/**
	 * The camera position is constrained to the extent of a sphere centered in the player (Zelda-style). 
	 */
    class DllExport SphericCameraMode : public CameraControlSystem::CameraMode//, public CameraControlSystem::CollidableCamera
	{
	public:

		/**
		 * @param relativePositionToCameraTarget Default camera position with respecto to camera target
		 * @param autoResetTime If greater than zero the camera will be resetted to its initial position after the amount of seconds specified.
		 * @param resetDistance true for correcting the camera distance to the target when resetting. If false, the camera will maintaing the current distance to target.
		 * @param resetRotationFactor Speed factor for correcting the camera rotation when resetting.
		 * @param offset Offset applied to the center of the sphere.  
		 */
        SphericCameraMode(CameraControlSystem* cam
			, const Ogre::Vector3 &relativePositionToCameraTarget
			, Ogre::Real outerSphereRadious, Ogre::Real innerSphereRadious = 0.0f
			, Ogre::Real autoResetTime = 3.0f
			, bool resetDistance = true
			, Ogre::Real resetRotationFactor = 1.0f
			, const Ogre::Vector3 &offset = Ogre::Vector3::ZERO
			, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : CameraControlSystem::CameraMode(cam)
            //, CameraControlSystem::CollidableCamera(cam, margin)
			, mRelativePositionToCameraTarget(relativePositionToCameraTarget)
			, mOuterSphereRadious(outerSphereRadious), mInnerSphereRadious(innerSphereRadious)
			, mAutoResetTime(autoResetTime)
			, mResetDistance(resetDistance)
			, mResetRotationFactor(resetRotationFactor)
			, mOffset(offset)
			, mFixedAxis(fixedAxis)
			, mResseting(false)
			, mLastRessetingDiff(Ogre::Radian(2.0f * Ogre::Math::PI))
        {
			/*this->collisionDelegate = newCollisionDelegate(this
				, &CollidableCamera::DefaultCollisionDetectionFunction);*/
			if( innerSphereRadious > outerSphereRadious){ throw new Ogre::Exception(1001,"Inner radious greater than outer radious","CCSSphericCameraMode.h"); }
			if( innerSphereRadious > relativePositionToCameraTarget.length() 
				|| outerSphereRadious < relativePositionToCameraTarget.length())
			{ 
				throw new Ogre::Exception(1002,"relativePositionToCameraTarget param value not valid.","CCSSphericCameraMode.h"); 
			}
		}

        virtual ~SphericCameraMode(){};

        virtual bool init();

        virtual void update(const Ogre::Real &timeSinceLastFrame);

        virtual void instantUpdate();

        // Specific methods

		/**
		 * @brief Reset the camera position to its initial position.
		 *
		 * @param instant true for doing it instantaniously; false for doing it smoothly.
		 */
		void reset(bool instant = true);

		/**
		 * @param outerSphereRadious cannot be smaller than current inner radious nor the length of relativePositionToCameraTarget.
		 */
		void setOuterSphereRadious(Ogre::Real outerSphereRadious);
		Ogre::Real getOuterSphereRadious(){ return mOuterSphereRadious; }

		/**
		 * @param innerSphereRadious cannot be greater than current outer radious nor the length of relativePositionToCameraTarget.
		 */
		void setInnerSphereRadious(Ogre::Real innerSphereRadious);
		Ogre::Real getInnerSphereRadious(){ return mInnerSphereRadious; }

		/**
		 * @brief Displacement of the center of the sphere with respect to the fixed axis
		 */
		void setHeightOffset(Ogre::Real value){ mOffset = mFixedAxis.normalisedCopy() * value; }
		void setOffset(Ogre::Vector3 offset){ mOffset = offset; }
		Ogre::Vector3 getOffset(){ return mOffset; }

		/**
		 * @param seconds zero for disabling.
		 */
		void setAutoResetTime(Ogre::Real seconds){ mAutoResetTime = seconds; }
		Ogre::Real getAutoResetTime(){ return mAutoResetTime; }
		
    protected:

        Ogre::Vector3 mFixedAxis;
		Ogre::Vector3 mOffset;
		Ogre::Real mOuterSphereRadious;
		Ogre::Real mInnerSphereRadious;
        Ogre::Real mResetDistance;
        Ogre::Real mResetRotationFactor;
		Ogre::Vector3 mRelativePositionToCameraTarget;
		Ogre::Real mAutoResetTime;
		Ogre::Real mTimeSinceLastChange;
		bool mResseting;
		Ogre::Radian mLastRessetingDiff;
		Ogre::Vector3 mLastTargetPosition;
		Ogre::Quaternion mLastTargetOrientation;
	};

}

#endif
