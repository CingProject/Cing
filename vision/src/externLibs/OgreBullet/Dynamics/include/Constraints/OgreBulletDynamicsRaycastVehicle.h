/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This program is free software; you can redistribute it and/or modify it under
the terms of the GPL General Public License with runtime exception as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GPL General Public License with runtime exception for more details.

You should have received a copy of the GPL General Public License with runtime exception along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
-----------------------------------------------------------------------------
*/
#ifndef _OGREBULLETDYNAMICS_RaycastVehicle_H
#define _OGREBULLETDYNAMICS_RaycastVehicle_H

#include "OgreBulletDynamicsPreRequisites.h"

#include "OgreBulletDynamicsConstraint.h"


namespace OgreBulletDynamics
{

    // -------------------------------------------------------------------------
    // VehicleRayCaster  class
    class VehicleRayCaster
    {
    public:
        VehicleRayCaster(DynamicsWorld *world);
        virtual ~VehicleRayCaster();
        
        btVehicleRaycaster *getBulletVehicleRayCaster()
        {return static_cast <btVehicleRaycaster*> (mBulletVehicleRayCaster);};
         
    private:
        btDefaultVehicleRaycaster *mBulletVehicleRayCaster;        
    };
    // -------------------------------------------------------------------------
    // VehicleTuning  class
    class VehicleTuning
    {
    public:
        VehicleTuning(
            const Ogre::Real suspensionStiffness,
            const Ogre::Real suspensionCompression,
            const Ogre::Real suspensionDamping,
            const Ogre::Real maxSuspensionTravelCm,
            const Ogre::Real frictionSlip);
        virtual ~VehicleTuning();
        
        btRaycastVehicle::btVehicleTuning *getBulletTuning()
        {return mBulletTuning;};
        
    private:
        btRaycastVehicle::btVehicleTuning *mBulletTuning;        
    };
    // -------------------------------------------------------------------------
    // RaycastVehicle  class
    class WheelInfo
    {
    public:
        WheelInfo(btWheelInfo &w): 
          mWheel(&w)
          {};
        virtual ~WheelInfo(){};

        btWheelInfo *getBulletWheelInfo(){return static_cast<btWheelInfo *> (mWheel);}
       
    protected:
        btWheelInfo                   *mWheel;
    };
    // -------------------------------------------------------------------------
    // RaycastVehicle  class
    class RaycastVehicle : public TypedConstraint
    {
    public:
        RaycastVehicle(WheeledRigidBody *body,  
            VehicleTuning        *vt,
            VehicleRayCaster     *caster = 0);

        virtual ~RaycastVehicle();

        btRaycastVehicle *getBulletVehicle()
        {return static_cast<btRaycastVehicle *> (mConstraint);};
        void setCoordinateSystem(int rightIndex,int upIndex,int forwardIndex);

        void addWheel(
            Ogre::SceneNode *node,
            const Ogre::Vector3 &connectionPoint,
            const Ogre::Vector3 &wheelDirection,
            const Ogre::Vector3 &wheelAxle,
            const Ogre::Real suspensionRestLength,
            const Ogre::Real wheelRadius,
            const bool isFrontWheel,
            const Ogre::Real wheelFriction,
            const Ogre::Real rollInfluence);

        // when all wheels are attached, make vehicle aware of it
        void setWheelsAttached();
        // update wheels when needed.
        void setTransform();

        void applyEngineForce (float engineForce, int wheel);
        void setSteeringValue(float steering, int wheel);

     protected:
         VehicleTuning                   *mTuning;
         VehicleRayCaster                *mRayCaster;
    
         std::vector<btWheelInfo *>         mWheelsInfo;
         std::vector<Ogre::SceneNode *>      mWheelNodes;

         WheeledRigidBody* mChassisBody;
         Ogre::SceneNode *mNode;
         
    };
}
#endif //_OGREBULLETDYNAMICS_RaycastVehicle_H

