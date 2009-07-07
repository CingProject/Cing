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

#ifndef _OGREBULLETCOLLISIONS_PREREQUISITES_H
#define _OGREBULLETCOLLISIONS_PREREQUISITES_H

#include "btBulletCollisionCommon.h"

#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "Ogre.h"

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif

namespace OgreBulletCollisions
{
    class BtOgreConverter;
    class OgreBtConverter;

    class CollisionsWorld;

    class CollisionRayResultCallback;
    class CollisionClosestRayResultCallback;

    class DebugLines;

    class DebugDrawer;

    class DebugContact;
    class DebugNormal;
    class DebugContactText;
    
    class DebugCollisionShape;
    class BoxDebugShape;
    class SphereDebugShape;
    class ConeDebugShape;
    class TriangleMeshDebugShape;
    class RayDebugShape;

	class GImpactConcaveShape;
	class HeightmapCollisionShape;
    class CollisionShape;
    class BoxCollisionShape;
    class SphereCollisionShape;
    class CylinderCollisionShape;
	class CapsuleCollisionShape;
    class ConeCollisionShape;

    class CompoundCollisionShape;

    class MultiSphereCollisionShape;
    class MinkowskiSumCollisionShape;

    class ConvexHullCollisionShape;
    class TriangleMeshCollisionShape;

    class Object;

    class ObjectState;

	class CollisionShapebject;

    typedef std::vector<Ogre::Vector3> Vector3Array;
    typedef std::map<unsigned short, Vector3Array* > BoneIndex;
    typedef std::pair<unsigned short, Vector3Array* > BoneKeyIndex;

}

#endif //_OGREBULLETCOLLISIONS_PREREQUISITES_H

