/* -------------------------------------------------------
Copyright (c) 2009 Alberto G. Salguero (agsh@ugr.es)

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

#ifndef _FreeCameraMode_H_
#define _FreeCameraMode_H_

#include "CCSPrerequisites.h"

#include "CCSCameraControlSystem.h"

namespace CCS
{
	/**
	 * In this mode the camera is controlled by the user. 
     * The camera orientation is fixed by a yaw axis.
	 */
    class DllExport FreeCameraMode : public CameraControlSystem::CameraMode, public CameraControlSystem::CollidableCamera
	{
	public:

        FreeCameraMode(CameraControlSystem* cam) 
            : CameraControlSystem::CameraMode(cam)
            , CameraControlSystem::CollidableCamera(cam)
			, mFixedAxis(Ogre::Vector3::UNIT_Y)
            , mMoveFactor(1)
            , mRotationFactor(0.13f)
            , mMargin(0) 
        { };

        virtual ~FreeCameraMode(){};

        virtual bool init();

        virtual void update(const Ogre::Real &timeSinceLastFrame);

        virtual void instantUpdate(){};

        //// Overriden methods

        //inline virtual void setCollisionsEnabled(bool value);
        //inline virtual bool getCollisionsEnabled() { return mCollisionsEnabled; }

        // Specific methods

		/**
		 * @brief Set the moving speed factor
		 * 
		 * @param unitsPerSecond the units the camera will be translated per second
		 */
        inline virtual void setMoveFactor(Ogre::Real unitsPerSecond){ mMoveFactor = unitsPerSecond; }

		/**
		 * @brief Set the rotating speed factor
		 * 
		 * @param radiansPerSecond the radians the camera will be rotated per second
		 */
        inline virtual void setRotationFactor(Ogre::Real radiansPerSecond){ mRotationFactor = radiansPerSecond; }

		/**
		 * @brief Tell the camera to go forward
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goForward(Ogre::Real percentage = 1){ mLongitudinalDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go backward
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goBackward(Ogre::Real percentage = 1){ goForward(-percentage); }

		/**
		 * @brief Tell the camera to go right (laterally)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goRight(Ogre::Real percentage = 1){ mLateralDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go left (laterally)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goLeft(Ogre::Real percentage = 1){ goRight(-percentage); }

		/**
		 * @brief Tell the camera to go up
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goUp(Ogre::Real percentage = 1){ mVerticalDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go down
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goDown(Ogre::Real percentage = 1){ goUp(-percentage); }

		/**
		 * @brief Tell the camera to look right
		 * 
		 * @param val the amount of rotation (use negative values to look left)
		 */
        inline virtual void yaw(Ogre::Real val){ mRotX += Ogre::Degree(-val * mRotationFactor); }

        /**
		 * @brief Tell the camera to look down
		 * 
		 * @param val the amount of rotation (use negative values to look up)
		 */
        inline virtual void pitch(Ogre::Real val){ mRotY += Ogre::Degree(-val * mRotationFactor); }

        /**
		 * @brief Margin value for the collision delegates
		 * 
		 * @param margin the distance 
		 */
        void setMargin(Ogre::Real margin){ mMargin = margin; }

        Ogre::Real getMargin(){ return mMargin; }

        /**
		 * @brief Collision delegate that mantains the camera always above the ground. A margin can be stablished using the 'setMargin' method.
		 */
        Ogre::Vector3 AboveGroundCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition);

        /**
		 * @brief Collision delegate that mantains a constant distance to the ground. The distance can be stablished using the 'setMargin' method.
		 */
        Ogre::Vector3 ConstantHeightCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition);

    protected:
        
        Ogre::Vector3 getFirstRealHit(Ogre::Vector3 origin, Ogre::Vector3 direction);
		
    protected:

        Ogre::Vector3 mFixedAxis;
        Ogre::Real mMoveFactor;
        Ogre::Real mRotationFactor;
        Ogre::Real mLongitudinalDisplacement;
        Ogre::Real mLateralDisplacement;
        Ogre::Real mVerticalDisplacement;
        Ogre::Vector3 mStartingPosition;
        Ogre::Radian mRotX;
	    Ogre::Radian mRotY;
        Ogre::Quaternion mStartingOrientation;
        Ogre::Real mMargin;
	};

}

#endif
