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

#ifndef _OrbitalCameraMode_H_
#define _OrbitalCameraMode_H_

#include "CCSPrerequisites.h"

#include "CCSCameraControlSystem.h"
#include "CCSBasicCameraModes.h"

namespace CCS
{
	/**
	 * This is basically an attached camera mode where the user
	 * can mofify the camera position. If the scene focus is seen
	 * as the center of a sphere, the camera rotates arount it
	 */
    class DllExport OrbitalCameraMode : public ChaseFreeYawAxisCameraMode
	{
	public:

		OrbitalCameraMode(CameraControlSystem* cam, Ogre::Real initialZoom = 1
			, Ogre::Radian initialHorizontalRotation = Ogre::Radian(0), Ogre::Radian initialVerticalRotation = Ogre::Radian(0)
			, bool resetToInitialPosition = true, Ogre::Real collisionmargin = 0.1f) 
			: ChaseFreeYawAxisCameraMode(cam, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, collisionmargin)
			, mZoomFactor(1)
            , mRotationFactor(0.13)
			, mInitialRotHorizontal(initialHorizontalRotation)
			, mInitialRotVertical(initialVerticalRotation)
			, mInitialZoom(initialZoom)			
			, mZoom(initialZoom)
			, mRotHorizontal(0)
			, mRotVertical(0)
			, mZoomDisplacement(0)
			, mResetToInitialPosition(resetToInitialPosition)
        {
			this->setCameraTightness(1);
		};

        virtual ~OrbitalCameraMode(){};

        virtual bool init();

        virtual void update(const Ogre::Real &timeSinceLastFrame);

        // Specific methods

		/**
		 * @brief Set the zooming speed factor
		 * 
		 * @param unitsPerSecond the units the camera will be zoomed in/out per second
		 */
        inline virtual void setZoomFactor(Ogre::Real unitsPerSecond){ mZoomFactor = unitsPerSecond; }

		/**
		 * @brief Set the rotating speed factor
		 * 
		 * @param radiansPerSecond the radians the camera will be rotated per second
		 */
		inline virtual void setRotationFactor(Ogre::Real radiansPerSecond){ mRotationFactor = radiansPerSecond; }

        /**
		 * @brief Tell the camera to look right
		 * 
		 * @param val percentage of the speed factor [-1,1] (use negative values to look left)
		 */
        inline virtual void yaw(Ogre::Real val = 1){ mRotHorizontalDisplacement += Ogre::Degree(-val); }

        /**
		 * @brief Get the current yaw value
		 * 
		 * @return the amount of rotation
		 */
        inline virtual Ogre::Radian getYaw(){ return -mRotHorizontal; }

        /**
		 * @brief Set the yaw value
		 * 
		 * @param val the amount of rotation (use negative values to look left)
		 */
        inline virtual void setYaw(Ogre::Radian val){ mRotHorizontal = mInitialRotHorizontal - val; }

        /**
		 * @brief Tell the camera to look down
		 * 
		 * @param val percentage of the speed factor [-1,1] (use negative values to look up)
		 */
        inline virtual void pitch(Ogre::Real val = 1){ mRotVerticalDisplacement += Ogre::Degree(-val); }

        /**
		 * @brief Get the current pitch value
		 * 
		 * @return the amount of rotation
		 */
        inline virtual Ogre::Radian getPitch(){ return -mRotVertical; }

        /**
		 * @brief Set the pitch value
		 * 
		 * @param val the amount of rotation (use negative values to look up)
		 */
        inline virtual void setPitch(Ogre::Radian val){ mRotVertical = mInitialRotVertical - val; }

		/**
		 * @brief Tell the camera to zoom out
		 * 
		 * @param val percentage of the speed factor [-1,1] (use negative values to zoom in)
		 */
		inline virtual void zoom(Ogre::Real val = 1){ mZoomDisplacement += val; }

        /**
		 * @brief Get the current zoom value
		 * 
		 * @return the amount of zoom
		 */
        inline virtual Ogre::Real getZoom(){ return mZoom; }

        /**
		 * @brief Set the zoom value
		 * 
		 * @param val the amount of zoom (use negative values to zoom in)
		 */
        inline virtual void setZoom(Ogre::Real val){ mZoom = val; }

		virtual void reset();
		
    protected:
        
        Ogre::Real mZoomFactor;
        Ogre::Real mRotationFactor;
        Ogre::Radian mRotHorizontal;
	    Ogre::Radian mRotVertical;
		Ogre::Real mZoom;
		Ogre::Radian mInitialRotHorizontal;
	    Ogre::Radian mInitialRotVertical;
		Ogre::Real mInitialZoom;
		Ogre::Radian mRotHorizontalDisplacement;
	    Ogre::Radian mRotVerticalDisplacement;
		Ogre::Real mZoomDisplacement;
		bool mResetToInitialPosition;
	};

}

#endif
