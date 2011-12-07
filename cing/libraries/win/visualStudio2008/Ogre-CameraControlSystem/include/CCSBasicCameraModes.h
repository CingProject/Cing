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

#ifndef _BasicCameraModes_H_
#define _BasicCameraModes_H_

#include "CCSPrerequisites.h"

#include "CCSCameraControlSystem.h"

namespace CCS
{
	/**
	 * In this mode the camera position and orientation never change. 
	 */
    class DllExport FixedCameraMode : public CameraControlSystem::CameraMode 
	{
	public:

        FixedCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : CameraControlSystem::CameraMode(cam)
			, mFixedAxis(fixedAxis)
            , mLastPosition(Ogre::Vector3::ZERO)
            , mLastOrientation(Ogre::Quaternion::IDENTITY)
            
        { };

        virtual ~FixedCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(true,mFixedAxis);
            mCameraCS->setAutoTrackingTarget(false);

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame){};
        virtual void instantUpdate()
        {
            mCameraPosition = mLastPosition;
            mCameraOrientation = mLastOrientation;
        };

        // Specific methods

        inline virtual void setCameraPosition(Ogre::Vector3 pos)
        {
            mLastPosition = pos;
            mCameraPosition = pos;
        }

        inline virtual void setCameraOrientation(Ogre::Quaternion orient)
        {
            mLastOrientation = orient;
            mCameraOrientation = mLastOrientation;
        }

        inline virtual void setCameraOrientation(const Ogre::Radian roll, 
            const Ogre::Radian yaw, const Ogre::Radian pitch)
        {
            mLastOrientation = Ogre::Quaternion(roll,Ogre::Vector3::UNIT_Z) 
                            * Ogre::Quaternion(yaw,Ogre::Vector3::UNIT_Y)
                            * Ogre::Quaternion(pitch,Ogre::Vector3::UNIT_X);

            mCameraOrientation = mLastOrientation;
        }
		
    protected:
        Ogre::Vector3 mFixedAxis;
        Ogre::Vector3 mLastPosition;
        Ogre::Quaternion mLastOrientation;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera position is fixed and the camera always 
	 * points to the target. 
	 */
    class DllExport FixedTrackingCameraMode : public FixedCameraMode 
	{
	public:

        FixedTrackingCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : FixedCameraMode(cam,fixedAxis)
        { };

        virtual ~FixedTrackingCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(true,mFixedAxis);
            mCameraCS->setAutoTrackingTarget(true);

            instantUpdate();

            return true;
        }
		    
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera follows the target. The second parameter is the relative position
     * to the target. The orientation of the camera is fixed by a yaw axis (UNIT_Y by default). 
	 */
    class DllExport ChaseCameraMode : public CameraControlSystem::CameraModeWithTightness, public CameraControlSystem::CollidableCamera 
	{
	public:

        ChaseCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : CameraControlSystem::CameraModeWithTightness(cam)	
			, CameraControlSystem::CollidableCamera(cam)
            , mFixedAxis(fixedAxis)
			, mRelativePositionToCameraTarget(relativePositionToCameraTarget)
            
        {
            mTightness = 0.01f;
        };

        virtual ~ChaseCameraMode(){};

        virtual bool init()
        {
            setFixedYawAxis(true, mFixedAxis);
            mCameraCS->setAutoTrackingTarget(true);

			this->collisionDelegate = newCollisionDelegate(this
				, &CollidableCamera::DefaultCollisionDetectionFunction);

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {           
			if(mCameraCS->getTargetSceneNode())
			{
				Ogre::Vector3 cameraCurrentPosition = mCameraCS->getCameraPosition();
				Ogre::Vector3 cameraFinalPositionIfNoTightness = mCameraCS->getCameraTargetPosition() 
					+ mCameraCS->getCameraTargetOrientation() * mRelativePositionToCameraTarget;

				Ogre::Vector3 diff = (cameraFinalPositionIfNoTightness - cameraCurrentPosition) * mTightness;
				mCameraPosition += diff;

				if(mCollisionsEnabled)
				{
					mCameraPosition = collisionDelegate(mCameraCS->getCameraTargetPosition(), mCameraPosition);
				}
			}
        }

        virtual void instantUpdate()
        {
			if(mCameraCS->getTargetSceneNode())
			{
				mCameraPosition = mCameraCS->getCameraTargetPosition() 
					+ mCameraCS->getCameraTargetOrientation() * mRelativePositionToCameraTarget;

				if(mCollisionsEnabled)
				{
					mCameraPosition = collisionDelegate(mCameraCS->getCameraTargetPosition(), mCameraPosition);
				}
			}
        }

        // Specific methods

        inline virtual void setCameraRelativePosition(Ogre::Vector3 posRelativeToCameraTarget)
        {
            mRelativePositionToCameraTarget = posRelativeToCameraTarget;
            instantUpdate();
        }

        inline virtual void setFixedYawAxis(bool useFixedAxis, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::ZERO)
        {
            mFixedAxis = mFixedAxis;
            mCameraCS->setFixedYawAxis(true,mFixedAxis);
        }
		
    protected:
        Ogre::Vector3 mFixedAxis;
        Ogre::Vector3 mRelativePositionToCameraTarget;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * This mode is similar to "Chase" camera mode but the camera orientation is not fixed by
     * a yaw axis. The camera orientation will be the same as the target.
	 */
    class DllExport ChaseFreeYawAxisCameraMode : public ChaseCameraMode 
	{
	public:

        ChaseFreeYawAxisCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
            , Ogre::Quaternion rotation) 
            : ChaseCameraMode(cam, relativePositionToCameraTarget)
        {
            mRotationOffset = rotation;
        }

        ChaseFreeYawAxisCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch) 
            : ChaseCameraMode(cam, relativePositionToCameraTarget)
        {
            mRotationOffset = Ogre::Quaternion(roll,Ogre::Vector3::UNIT_Z) 
                            * Ogre::Quaternion(yaw,Ogre::Vector3::UNIT_Y)
                            * Ogre::Quaternion(pitch,Ogre::Vector3::UNIT_X);
        }

        virtual ~ChaseFreeYawAxisCameraMode(){};

        virtual bool init()
        {
            ChaseCameraMode::setFixedYawAxis(false);
            mCameraCS->setAutoTrackingTarget(false);

			this->collisionDelegate = newCollisionDelegate(this
				, &CollidableCamera::DefaultCollisionDetectionFunction);

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {            
            // Update camera position
            ChaseCameraMode::update(timeSinceLastFrame);

            // Update camera orientation
            mCameraOrientation = mCameraCS->getCameraTargetOrientation() * mRotationOffset;
        }

        virtual void instantUpdate()
        {
			if(mCameraCS->getTargetSceneNode())
			{
				// Update camera position
				ChaseCameraMode::instantUpdate();

				// Update camera orientation
				mCameraOrientation = mCameraCS->getCameraTargetOrientation() * mRotationOffset;
			}
        }

        // Specific methods

        inline virtual void setCameraRelativePosition(Ogre::Vector3 posRelativeToCameraTarget
            , Ogre::Quaternion rotation)
        {
            mRelativePositionToCameraTarget = posRelativeToCameraTarget;

            mRotationOffset = rotation;

            instantUpdate();
        }

        inline virtual void setCameraRelativePosition(Ogre::Vector3 posRelativeToCameraTarget
            , const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch)
        {
            mRelativePositionToCameraTarget = posRelativeToCameraTarget;

            mRotationOffset = Ogre::Quaternion(roll,Ogre::Vector3::UNIT_Z) 
                            * Ogre::Quaternion(yaw,Ogre::Vector3::UNIT_Y)
                            * Ogre::Quaternion(pitch,Ogre::Vector3::UNIT_X);

            instantUpdate();
        }
		
    protected:
        Ogre::Quaternion mRotationOffset;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera node is attached to the target node as a child.
	 */
    class DllExport AttachedCameraMode : public CameraControlSystem::CameraMode
	{
	public:

        AttachedCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
            , Ogre::Quaternion rotation) 
            : CameraMode(cam)
            , mRotation(rotation)
            , mRelativePositionToCameraTarget(relativePositionToCameraTarget)
        { }

        AttachedCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget, 
            const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch) 
            : CameraMode(cam)
            , mRelativePositionToCameraTarget(relativePositionToCameraTarget)
        {
            mRotation = Ogre::Quaternion(roll,Ogre::Vector3::UNIT_Z) 
                * Ogre::Quaternion(yaw,Ogre::Vector3::UNIT_Y)
                * Ogre::Quaternion(pitch,Ogre::Vector3::UNIT_X);
        }

        virtual ~AttachedCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(false);
            mCameraCS->setAutoTrackingTarget(false);

            mCameraPreviousParentSceneNode = mCameraCS->getCameraSceneNode()->getParent();
            mCameraPreviousParentSceneNode->removeChild(mCameraCS->getCameraSceneNode());
            mCameraCS->getTargetSceneNode()->addChild(mCameraCS->getCameraSceneNode());
            
            mCameraPosition = mRelativePositionToCameraTarget;
            
            mCameraOrientation = mRotation;

            instantUpdate();

            return true;
        }

        virtual void stop()
        {
            mCameraCS->getCameraSceneNode()->getParent()->removeChild(mCameraCS->getCameraSceneNode());
            mCameraPreviousParentSceneNode->addChild(mCameraCS->getCameraSceneNode());
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame) { }
        virtual void instantUpdate() { }

    protected:
        Ogre::Quaternion mRotation;
        Ogre::Vector3 mRelativePositionToCameraTarget;
        Ogre::Node* mCameraPreviousParentSceneNode; 

	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This is basically an attached camera mode with the posibility of hide 
	 * or show the focus of the scene
	 */
    class DllExport FirstPersonCameraMode : public AttachedCameraMode
	{
	public:

        FirstPersonCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
            , Ogre::Quaternion rotation)
			: AttachedCameraMode(cam, relativePositionToCameraTarget,rotation)
            , mCharacterVisible(true)
        { }

        FirstPersonCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
            , const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch) 
			: AttachedCameraMode(cam, relativePositionToCameraTarget,roll,yaw,pitch)
            , mCharacterVisible(true)
        { }

        virtual ~FirstPersonCameraMode(){};

		virtual bool init()
        {
			AttachedCameraMode::init();

            mCameraCS->getTargetSceneNode()->setVisible(mCharacterVisible);
            
            return true;
        }

		virtual void stop()
        {
			AttachedCameraMode::stop();

            mCameraCS->getTargetSceneNode()->setVisible(true);
        }

		// Specific methods

        void setCharacterVisible(bool value)
        { 
            mCharacterVisible = value;
            if(mCameraCS->getTargetSceneNode())
            {
                mCameraCS->getTargetSceneNode()->setVisible(mCharacterVisible);
            }
        }

        bool getCharacterVisible(){ return mCharacterVisible; }

	protected:
		bool mCharacterVisible;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera is constrained to the limits of a plane. 
	 * The camera always points to the target, perpendicularly to the plane.
	 */
    class DllExport PlaneBindedCameraMode : public CameraControlSystem::CameraModeWithTightness 
	{
	public:

        PlaneBindedCameraMode(CameraControlSystem* cam, Ogre::Plane &plane, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : CameraControlSystem::CameraModeWithTightness(cam)
			, mFixedAxis(fixedAxis)
			, mPlane(plane)
        {
            mTightness = 1;
        };

        virtual ~PlaneBindedCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(true,mFixedAxis);
            mCameraCS->setAutoTrackingTarget(true);

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {
            Ogre::Real distance = mPlane.getDistance(mCameraCS->getCameraTargetPosition());

            Ogre::Vector3 cameraCurrentPosition = mCameraCS->getCameraPosition();
            Ogre::Vector3 cameraFinalPositionIfNoTightness = mCameraCS->getCameraTargetPosition() -
                mPlane.normal.normalisedCopy() * distance;

            Ogre::Vector3 diff = (cameraFinalPositionIfNoTightness - cameraCurrentPosition) * mTightness;
            mCameraPosition += diff;
        };

        virtual void instantUpdate()
        {
            Ogre::Real distance = mPlane.getDistance(mCameraCS->getCameraTargetPosition());
            mCameraPosition = mCameraCS->getCameraTargetPosition() - 
                mPlane.normal.normalisedCopy() * distance;
        };
		
    protected:
        Ogre::Vector3 mFixedAxis;
        Ogre::Plane mPlane;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera points to a given position (the "focus") 
	 * throuh the target. The camera orientation is fixed by a yaw axis.
	 */
    class DllExport ThroughTargetCameraMode : public CameraControlSystem::CameraMode 
	{
	public:

        ThroughTargetCameraMode(CameraControlSystem* cam, Ogre::Real margin, bool inverse = false
            , Ogre::Vector3 focusPos = Ogre::Vector3::ZERO, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : CameraControlSystem::CameraMode(cam)
			, mFixedAxis(fixedAxis)
			, mMargin(margin)
            , mInverse(inverse)
        { };

        virtual ~ThroughTargetCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(true,mFixedAxis);
            mCameraCS->setAutoTrackingTarget(true);

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {
            instantUpdate();
        };

        virtual void instantUpdate()
        {
            if(mCameraCS->hasCameraTarget())
            {
                if(!mInverse)
                {
                    Ogre::Vector3 focusToTarget = mCameraCS->getCameraTargetPosition() - mFocusPos;
                    mCameraPosition = mCameraCS->getCameraTargetPosition() + 
                        focusToTarget.normalisedCopy() * mMargin;
                }
                else
                {
                    Ogre::Vector3 focusToTarget = mCameraCS->getCameraTargetPosition() - mFocusPos;
                    mCameraPosition = mFocusPos - 
                        focusToTarget.normalisedCopy() * mMargin;
                }
            }
        };

        // Specific methods

        inline virtual void setCameraFocusPosition(Ogre::Vector3 pos)
        { 
            mFocusPos = pos; 
            init();
        };

        void setInverse(bool value){ mInverse = value; }
        bool getInverse(){ return mInverse; }
		
    protected:
        Ogre::Vector3 mFixedAxis;
        Ogre::Vector3 mFocusPos;
        Ogre::Real mMargin;
        bool mInverse;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this camera mode the position of the camera is chosen to be 
	 * the closest to the target of a given list. The camera orientation 
	 * is fixed by a yaw axis.
	 */
    class DllExport ClosestToTargetCameraMode : public FixedTrackingCameraMode 
	{
	public:

        ClosestToTargetCameraMode(CameraControlSystem* cam, Ogre::Real timeInterval = 1
            , const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : FixedTrackingCameraMode(cam, fixedAxis)
			, mTimeInterval(timeInterval)
            , mTime(timeInterval)
        { };

        virtual ~ClosestToTargetCameraMode(){};

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {
            instantUpdate();
            return;

            mTime -= timeSinceLastFrame;

            if(mTime < 0)
            {
                instantUpdate();
                mTime = mTimeInterval;
            }
        };

        virtual void instantUpdate()
        {
            if(mCameraCS->hasCameraTarget())
            {
                Ogre::Real minDistance = std::numeric_limits<float>::max();
                Ogre::Vector3 targetPosition = mCameraCS->getCameraTargetPosition();

                std::vector<Ogre::Vector3>::const_iterator it;
                std::vector<Ogre::Vector3>::const_iterator closest;
                for(it = mPositionsList.begin(); it != mPositionsList.end(); ++it)
                {
                    Ogre::Vector3 vector_i = ((Ogre::Vector3)(*it));
                    Ogre::Real distance = (vector_i - targetPosition).length();
                    if( distance < minDistance)
                    {
                        closest = it;
                        minDistance = distance;
                    }
                }

                mCameraPosition = (Ogre::Vector3)(*closest);
            }
        };

        // Specific methods

        inline virtual void addCameraPosition(Ogre::Vector3 &pos)
        { 
            mPositionsList.push_back(pos); 
            init();
        };
		
    protected:
        std::vector<Ogre::Vector3> mPositionsList;
        Ogre::Real mTimeInterval;
        Ogre::Real mTime;
        bool mInverse;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera is controlled by the user. In this mode the 
	 * target is always seen from the same point of view.
	 */
    class DllExport FixedDirectionCameraMode : public CameraControlSystem::CameraModeWithTightness 
	{
	public:

        FixedDirectionCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &direction, const Ogre::Real distance
			, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y) 
            : CameraControlSystem::CameraModeWithTightness(cam)
			, mFixedAxis(fixedAxis)
            , mDistance(distance)
			, mDirection(direction.normalisedCopy())            
        {
            mTightness = 1;
        };

        virtual ~FixedDirectionCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(true, mFixedAxis);
            mCameraCS->setAutoTrackingTarget(true);

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {            
            Ogre::Vector3 cameraCurrentPosition = mCameraCS->getCameraPosition();
            Ogre::Vector3 cameraFinalPositionIfNoTightness = mCameraCS->getCameraTargetPosition() 
                - mDirection * mDistance;

            Ogre::Vector3 diff = (cameraFinalPositionIfNoTightness - cameraCurrentPosition) * mTightness;
            mCameraPosition += diff;
        }

        virtual void instantUpdate()
        {
            mCameraPosition = mCameraCS->getCameraTargetPosition() 
                - mDirection * mDistance;
        }

        // Specific methods

        inline virtual void setDirection(Ogre::Vector3 direction)
        {
            mDirection = direction.normalisedCopy();
            instantUpdate();
        }

        inline virtual void setDistance(Ogre::Real distance)
        {
            mDistance = distance;
            instantUpdate();
        }
		
    protected:
        Ogre::Vector3 mFixedAxis;
        Ogre::Real mDistance;
        Ogre::Vector3 mDirection;
	};

    //--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera is constrained to the limits of a plane. 
	 * This is the typical camera you can found in any Real Time Strategy game.
	 */
    class DllExport RTSCameraMode : public CameraControlSystem::CameraMode 
	{
	public:

        RTSCameraMode(CameraControlSystem* cam
            , const Ogre::Vector3 &initialPosition = Ogre::Vector3::ZERO
            , const Ogre::Vector3 &upAxis = Ogre::Vector3::NEGATIVE_UNIT_Z
            , const Ogre::Vector3 &leftAxis = Ogre::Vector3::NEGATIVE_UNIT_X
            , const Ogre::Radian cameraPitch = Ogre::Radian(Ogre::Degree(90))
            , Ogre::Real minZoom = 0, Ogre::Real maxZoom = 0) 
            : CameraControlSystem::CameraMode(cam)
            , mUpAxis(upAxis.normalisedCopy())
            , mLeftAxis(leftAxis.normalisedCopy())
            , mMoveFactor(1)
            , mCameraPitch(cameraPitch)
            , mMinZoom(minZoom)
            , mMaxZoom(maxZoom)
            , mZoom(0)
        {
            mRotation = Ogre::Quaternion(cameraPitch,leftAxis);
            mHeightAxis = -upAxis.crossProduct(leftAxis).normalisedCopy();
            mCameraPosition = initialPosition;

            mHeightDisplacement = 0;
            mLateralDisplacement = 0;
            mVerticalDisplacement = 0;
        };

        virtual ~RTSCameraMode(){};

        virtual bool init()
        {
            mCameraCS->setFixedYawAxis(false);
            mCameraCS->setAutoTrackingTarget(false);
            
            mCameraOrientation = mRotation;

            instantUpdate();

            return true;
        }

        virtual void update(const Ogre::Real &timeSinceLastFrame)
        {
            Ogre::Vector3 displacement = ((mUpAxis * mVerticalDisplacement)
                + (mHeightAxis * mHeightDisplacement)
                + (mLeftAxis * mLateralDisplacement)) * timeSinceLastFrame * mMoveFactor;

            mCameraPosition += displacement;

            mHeightDisplacement = 0;
            mLateralDisplacement = 0;
            mVerticalDisplacement = 0; 

            mCameraOrientation = mRotation;
        }

        virtual void instantUpdate()
        {
            Ogre::Vector3 displacement = ((mUpAxis * mVerticalDisplacement)
                + (mHeightAxis * mHeightDisplacement)
                + (mLeftAxis * mLateralDisplacement));

            mCameraPosition += displacement;

            mHeightDisplacement = 0;
            mLateralDisplacement = 0;
            mVerticalDisplacement = 0; 

            mCameraOrientation = mRotation;
        }

        // Specific methods

        void setCameraPitch(Ogre::Radian cameraPitch)
        {
            mCameraPitch = cameraPitch;
            mRotation = Ogre::Quaternion(mCameraPitch, mLeftAxis);
        }

        Ogre::Radian getCameraPitch(){ return mCameraPitch; };

        /**
		 * @brief Set the moving speed factor (increase the height)
		 * 
		 * @param unitsPerSecond the units the camera will be translated per second
		 */
        inline virtual void setMoveFactor(Ogre::Real unitsPerSecond){ mMoveFactor = unitsPerSecond; }

        void setZoom(Ogre::Real zoom)
        {
            Ogre::Real desiredDisplacement = zoom - mZoom;

            //Ogre::Real desiredZoom = mZoom + desiredDisplacement;

            if(zoom <= mMaxZoom && zoom >= mMinZoom)
            {
                mHeightDisplacement = desiredDisplacement;
                mZoom = zoom;
            }
            else if(zoom > mMaxZoom)
            {
                mHeightDisplacement = mMaxZoom - mZoom;
                mZoom = mMaxZoom;
            }
            else if(zoom < mMinZoom)
            {
                mHeightDisplacement = mMinZoom - mZoom;
                mZoom = mMinZoom;
            }

            instantUpdate();
        }

        Ogre::Real getZoom(){ return mZoom; };

		/**
		 * @brief Tell the camera to zoom in (reduce the height)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void zoomIn(Ogre::Real percentage = 1)
        { 
            Ogre::Real desiredDisplacement = mMoveFactor * percentage;
            Ogre::Real desiredZoom = mZoom + desiredDisplacement;

            setZoom(desiredZoom);

            /*if(desiredZoom <= mMaxZoom && desiredZoom >= mMinZoom)
            {
                mHeightDisplacement = desiredDisplacement;
                mZoom = desiredZoom;
            }
            else if(desiredZoom > mMaxZoom)
            {
                mHeightDisplacement = mMaxZoom - mZoom;
                mZoom = mMaxZoom;
            }
            else if(desiredZoom < mMinZoom)
            {
                mHeightDisplacement = mMinZoom - mZoom;
                mZoom = mMinZoom;
            }*/
        }

		/**
		 * @brief Tell the camera to zoom out
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void zoomOut(Ogre::Real percentage = 1){ zoomIn(-percentage); }

		/**
		 * @brief Tell the camera to go right (along the leftAxis)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goRight(Ogre::Real percentage = 1){ mLateralDisplacement -= mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go left (along the leftAxis)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goLeft(Ogre::Real percentage = 1){ goRight(-percentage); }

		/**
		 * @brief Tell the camera to go up (along the upAxis)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goUp(Ogre::Real percentage = 1){ mVerticalDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go down (along the upAxis)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goDown(Ogre::Real percentage = 1){ goUp(-percentage); }
		
    protected:

        Ogre::Quaternion mRotation;
        Ogre::Radian mCameraPitch;
        Ogre::Vector3 mUpAxis;
        Ogre::Vector3 mLeftAxis;
        Ogre::Vector3 mHeightAxis;
        Ogre::Real mMoveFactor;
        Ogre::Real mHeightDisplacement;
        Ogre::Real mLateralDisplacement;
        Ogre::Real mVerticalDisplacement;
        Ogre::Real mZoom;
        Ogre::Real mMinZoom;
        Ogre::Real mMaxZoom;
	};
}

#endif
