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

#ifndef _CameraControlSystem_H_
#define _CameraControlSystem_H_

#include "CCSPrerequisites.h"

#include "CCSFastDelegate.h"

namespace CCS
{
    class DllExport CameraControlSystem 
	{
	public:

		/**
		 * @brief Abstract common base class of all camera modes
		 *
		 * Common base class of all camera modes. All derived camera modes
		 * should implement, at least, the "update" and "instantUpdate" methods
		 */
        class CameraMode
        {
        public:

			/**
			 * @brief Constructor
			 * @param cam a reference to a valid CCS
			 */
            CameraMode(CameraControlSystem* cam) : mCameraCS(cam)
                , mCameraPosition(Ogre::Vector3::ZERO)
                , mCameraOrientation(Ogre::Quaternion::IDENTITY) {};

			/**
			 * @brief Destructor
			 */
			virtual ~CameraMode() {};

			/**
			 * @brief Called when the camera mode is activated
			 *
			 * This method is called each time the camera mode is activated.
			 * All the initializations the camera mode needs in order to work
			 * should be performed here.
			 *
			 * @return true if no errors found
			 */
            virtual bool init(){ return true; };

			/**
			 * @brief Called when the camera mode is deactivated
			 *
			 * This method is called each time the camera mode is deactivated.
			 * All the actions performed in the init method which affect to other 
			 * camera modes should be resersed here.
			 */
            virtual void stop(){ };

			/**
			 * @brief Update camera position
			 *
			 * This method is called once per frame. The camera position and
			 * orientation variable members should be modified in this method.
			 */
            virtual void update(const Ogre::Real &timeSinceLastFrame) = 0;

			/**
			 * @brief Update camera position instantaneously
			 *
			 * This method is usually called just after the init method. The camera 
			 * position and orientation variable members should be modified in this
			 * method.
			 */
            virtual void instantUpdate() = 0;

            Ogre::Vector3 getCameraPosition() { return mCameraPosition; }
            Ogre::Quaternion getCameraOrientation() { return mCameraOrientation; }

        protected:
            CameraControlSystem* mCameraCS;
            Ogre::Vector3 mCameraPosition;
            Ogre::Quaternion mCameraOrientation;
        };

		/**
		 * @brief Abstract base camera mode with tightness
		 */
        class CameraModeWithTightness : public CameraMode
        {
        public:

			/**
			 * @brief Constructor
			 * @param cam a reference to a valid CCS
			 */
            CameraModeWithTightness(CameraControlSystem* cam) 
				: CameraMode(cam)
				, mTightness(1)
			{};

			/**
			 * @brief Destructor
			 */
            virtual ~CameraModeWithTightness() {};

            inline void setCameraTightness(Ogre::Real value) { mTightness = value; }
            inline Ogre::Real getCameraTightness() { return mTightness; }

        protected:
            Ogre::Real mTightness;
        };

		/**
		 * @brief Base class for collidable camera modes
		 */
        class CollidableCamera
        {
        public:

			/**
			 * @brief Constructor
			 * @param cam a reference to a valid CCS
			 */
            CollidableCamera(CameraControlSystem* cam) 				
				: mCameraCS2(cam)
				, mCollisionsEnabled(false)
			{};

			/**
			 * @brief Destructor
			 */
            virtual ~CollidableCamera() {};

            inline virtual void setCollisionsEnabled(bool value) { mCollisionsEnabled = value; }
            inline virtual bool getCollisionsEnabled() { return mCollisionsEnabled; }

			// A CollisionDelegate takes as params the position of the target and the camera, and returns the 
			// the new camera position.
			typedef fastdelegate::FastDelegate2<Ogre::Vector3, Ogre::Vector3, Ogre::Vector3> CollisionDelegate;
			
			CollisionDelegate collisionDelegate;

			template <class X, class Y>
			static CollisionDelegate newCollisionDelegate(Y* x, Ogre::Vector3 (X::*func)(Ogre::Vector3 targetPosition, Ogre::Vector3 cameraPosition)) { 
				return fastdelegate::MakeDelegate(x, func);
			}

			/**
			 * @brief Collisions detection based on the basic Ogre raycasting 
			 * 
			 * @param cameraTargetPosition the position of the camera target
			 * @param cameraPosition the position of the camera
			 *
			 * @return the first hit between the camera target position and the camera position.
			 */
			Ogre::Vector3 DefaultCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition);

		protected:
			CameraControlSystem* mCameraCS2;
			bool mCollisionsEnabled;
		};

		/** 
		 * @brief Contructor 
		 *
		 * @param sm Ogre Scene manager pointer
		 * @param name Name of the instance
         * @param camera Ogre camera. If null it will be created by the CCS
		 * @param recalculateOnTargetMoving If true the camera position will be recalculated at the end of the frame if the target scene node is changed within the current frame.
		 */
		CameraControlSystem(Ogre::SceneManager* sm, Ogre::String name, Ogre::Camera *camera = NULL, bool recalculateOnTargetMoving = true);

		/** 
		 * @brief Contructor
		 *
		 * @param sm Ogre Scene manager pointer
		 * @param name Name of the instance
         * @param customCameraSceneNode External scenenode holding the Ogre camera. 
		 * @param recalculateOnTargetMoving If true the customCameraSceneNode position will be recalculated at the end of the frame if the target scene node is changed within the current frame.
		 */
		CameraControlSystem(Ogre::SceneManager* sm, Ogre::String name, Ogre::SceneNode* customCameraSceneNode, bool recalculateOnTargetMoving = true);

		/**
		 * @brief Destructor
		 */
		~CameraControlSystem();

		/**
		 * @brief Add a new camera mode
		 *
		 * Add a new named camera mode instance to the CCS. Multiple instances of the
		 * same camera mode, with the same or different parameters, can be registered 
		 * in the same CCS if their instance names are differents. 
		 *
		 * @param name the name to assign to the camera mode
		 * @param cameraMode cameraMode the camera mode to be registered
		 */
        inline void registerCameraMode(Ogre::String name, CameraMode* cameraMode)
        {
            mCameraModes[name] = cameraMode;
        }

		/**
		 * @brief Remove an instance of a camera mode in a CCS. NOT IMPLEMENTED YET.
		 *
		 * @param cameraMode camera mode instance to be removed
		 */
        inline void removeCameraMode(CameraMode* cameraMode){ throw new Ogre::Exception(1000,"Not implemented.","CCSCameraControlSystem.h"); };

		/**
		 * @brief Remove all camera modes associated to the CCS. 
		 */
		void deleteAllCameraModes();

		/**
		 * @brief Get the camera mode instance by name
		 *
		 * @param name the name of the camera mode to search for
		 *
		 * @return the cameraMode or null if not found
		 */
        CameraMode* getCameraMode(Ogre::String name);

		/**
		 * @brief Get the next camera mode instance (alphabetic order)
		 *
		 * @return the next cameraMode instance or the first if the current 
		 * is the latest one
		 */
        CameraMode* getNextCameraMode();

		/**
		 * @brief Get the name of a camera mode instance
		 *
		 * @param camMode the camera mode instance
		 *
		 * @return the name of the camera mode instance or an empty string
		 * if the camera instance has not been found in the CCS
		 */
        Ogre::String getCameraModeName(CameraMode* camMode);

		/**
		 * @brief Get the current camera mode instance
		 *
		 * @return the current camera mode instance
		 */
        inline CameraMode* getCurrentCameraMode(){ return mCurrentCameraMode; };

		/**
		 * @brief Set the current camera mode instance
		 *
		 * @param cameraMode the camera mode instance to be set
		 */
        void setCurrentCameraMode(CameraMode* cameraMode);

		/**
		 * @brief Update the CCS
		 *
		 * This method should be called once per frame
		 *
		 * @param timeSinceLastFrame time elapsed since last frame
		 */
        void update(const Ogre::Real &timeSinceLastFrame);
		
		/**
		 * @brief Get the ogre::camera associated to the CCS
		 *
		 * @return the ogre::camera
		 */
		inline Ogre::Camera* getOgreCamera()
		{
            return mCamera;
		}

		/**
		 * @brief Set the ogre::camera associated to the CCS
		 */
		inline void setOgreCamera(Ogre::Camera* camera)
		{
            mCamera = camera;
		}

		/**
		 * @brief Set the focus of the scene
		 *
		 * This does not mean that the camera will always look to the focus. It
		 * depends on the camera mode
		 *
		 * @param targetNode the focus 
		 */
        void setCameraTarget(Ogre::SceneNode* targetNode);

		/**
		 * @brief Get wheter the CCS has a scene focus defined
		 *
		 * @return true if it has or false in other case
		 */
        inline bool hasCameraTarget(){ return (mTargetNode)?true:false; };

		/**
		 * @brief Get the scene node which holds the focus of the scene
		 *
		 * @return the scene node holding the scene focus 
		 */
        inline Ogre::SceneNode* getTargetSceneNode(){ return mTargetNode; }

		/**
		 * @brief Get the scene node the camera is attached to
		 *
		 * @return the scene node the camera is attached to
		 */
        inline Ogre::SceneNode* getCameraSceneNode(){ return mCameraNode; }

		/**
		 * @brief Get the derived focus position
		 *
		 * @return the position in global coordinate system
		 */
        inline Ogre::Vector3 getCameraTargetPosition(){ return mTargetNode->_getDerivedPosition(); }

		/**
		 * @brief Get the derived camera position
		 *
		 * @return the position in global coordinate system
		 */
        inline Ogre::Vector3 getCameraPosition(){ return mCameraNode->_getDerivedPosition(); }

		/**
		 * @brief Get the derived focus orientation
		 *
		 * @return the orientation in global coordinate system
		 */
        inline Ogre::Quaternion getCameraTargetOrientation(){ return mTargetNode->_getDerivedOrientation(); }

		/**
		 * @brief Get the derived camera orientation
		 *
		 * @return the orientation in global coordinate system
		 */
        inline Ogre::Quaternion getCameraOrientation(){ return mCameraNode->_getDerivedOrientation(); }

		/**
		 * @brief set wheter the camera will always points to the scene focus or not
		 *
		 * The method setFixedYawAxis should be used in combination with this one.
		 *
		 * @param autoTracking if true the camera will always automatically points
		 * to the scene focus
		 */
        void setAutoTrackingTarget(bool autoTracking);

		/**
		 * @brief Set the fixed yaw axis
		 *
		 * @param useFixedAxis if true, the next parameter is considered
		 * @param fixedAxis the axis which will be used to completely 
		 * define the rotation when using autotracking
		 */
        inline void setFixedYawAxis(bool useFixedAxis, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y )
        {
            mCameraNode->setFixedYawAxis(useFixedAxis,fixedAxis);
        }

		/**
		 * @brief Get the last timeSinceLastFrame value when the CCS was updated.
		 * 
		 * @return The timeSinceLastFrame value
		 */
		inline Ogre::Real getTimeSinceLastFrameLastUpdate(){ return mTimeSinceLastFrameLastUpdate; };

		inline Ogre::SceneManager* getSceneManager(){ return mSceneMgr; };

	protected:

		/**
		 * @brief This class is internally used to notify the CameraControlSystem class the target scene node has been moved within the current frame
		 */
		class NodeListener : public Ogre::Node::Listener
		{
		public:
			NodeListener(CameraControlSystem* cam) : mCameraCS(cam){};
			~NodeListener(){};
			void nodeUpdated(const Ogre::Node *nod){ mCameraCS->update( mCameraCS->getTimeSinceLastFrameLastUpdate() ); };
		protected:
			Ogre::Real mTimeSinceLastFrame;
			CameraControlSystem* mCameraCS;
		};

        Ogre::SceneManager *mSceneMgr;
		Ogre::String mName;

		Ogre::Camera* mCamera; // ogre camera
        Ogre::SceneNode* mCameraNode; // Where the camera is attached to
        bool mOwnCamera; // true if the ogre camera has been created by this class

        Ogre::SceneNode* mTargetNode; // The scene focus
		NodeListener* mTargetNodeListener; // To recalculate the camera position if the target scenenode has been changed within the current frame
		Ogre::Real mTimeSinceLastFrameLastUpdate; // Time value passed to the last call of the method "update"
		bool mRecalculateOnTargetMoving;

        CameraMode* mCurrentCameraMode;
        std::map<Ogre::String, CameraMode*> mCameraModes; // The list of camera mode instances
		
	};
}

#endif
