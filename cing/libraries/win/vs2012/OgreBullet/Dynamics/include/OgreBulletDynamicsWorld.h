/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



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

#ifndef _OGREBULLETDYNAMICS_DynamicWorld_H
#define _OGREBULLETDYNAMICS_DynamicWorld_H

#include "OgreBulletDynamicsPreRequisites.h"

#include "OgreBulletCollisionsWorld.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

namespace OgreBulletDynamics
{
    // -------------------------------------------------------------------------
    // basic rigid body class
    class DynamicsWorld : public OgreBulletCollisions::CollisionsWorld
    {
    public:
        DynamicsWorld(Ogre::SceneManager *mgr, 
            const Ogre::AxisAlignedBox &bounds,  
            const Ogre::Vector3 &gravity,
            bool init = true, bool set32BitAxisSweep = true, unsigned int maxHandles = 1500000);

	    ~DynamicsWorld();

        template <class BTDNYWORLDCLASS>
            void createBtDynamicsWorld(BTDNYWORLDCLASS *&createdWorld)
            {
				createdWorld = new BTDNYWORLDCLASS(mDispatcher, mBroadphase, mConstraintsolver, &mDefaultCollisionConfiguration);
                mWorld = createdWorld;
            }

        void stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1./60.);

        void addRigidBody (RigidBody *rb, short collisionGroup, short collisionMask);


        inline btDynamicsWorld * getBulletDynamicsWorld() const {return static_cast<btDynamicsWorld *> (mWorld);};

        void removeConstraint(TypedConstraint *constraint);
        void addConstraint(TypedConstraint *constraint);
        bool isConstraintRegistered(TypedConstraint *constraint) const;

        void addVehicle(RaycastVehicle *v);

    private:
        btConstraintSolver                                *mConstraintsolver;

		std::deque <TypedConstraint *>                    mConstraints;
		std::deque <ActionInterface *>                    mActionInterface;
    };
}
#endif //_OGREBULLETDYNAMICS_DynamicWorld_H

