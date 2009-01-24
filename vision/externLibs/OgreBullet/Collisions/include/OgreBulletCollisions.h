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

#ifndef _OGREBULLETCOLLISIONS_H
#define _OGREBULLETCOLLISIONS_H

#include "OgreBulletCollisionsPreRequisites.h"


#ifdef _PRECOMP


    #include "Utils/OgreBulletConverter.h"

	//Debug Helpers
	#include "Debug/OgreBulletCollisionsDebugLines.h"
	#include "Debug/OgreBulletCollisionsDebugShape.h"
	#include "Debug/OgreBulletCollisionsDebugDrawer.h"
	#include "Debug/OgreBulletCollisionsDebugContact.h"

    //Shapes
    #include "OgreBulletCollisionsShape.h"

    #include "Shapes/OgreBulletCollisionsBoxShape.h"
    #include "Shapes/OgreBulletCollisionsConeShape.h"
    #include "Shapes/OgreBulletCollisionsSphereShape.h"
    #include "Shapes/OgreBulletCollisionsCylinderShape.h"
	#include "Shapes/OgreBulletCollisionsTriangleShape.h"
	#include "Shapes/OgreBulletCollisionsTerrainShape.h"
	#include "Shapes/OgreBulletCollisionsCylinderShape.h"
	#include "Shapes/OgreBulletCollisionsCapsuleShape.h"
    #include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"

    #include "Shapes/OgreBulletCollisionsCompoundShape.h"

    #include "Shapes/OgreBulletCollisionsMultiSphereShape.h"

    #include "Shapes/OgreBulletCollisionsConvexHullShape.h"
    #include "Shapes/OgreBulletCollisionsMinkowskiSumShape.h"

    #include "Shapes/OgreBulletCollisionsTrimeshShape.h"

    //Utils
    #include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

    // Ray
    //#include "OgreBulletCollisionsConvexCast.h"

    // Core Collisions
    #include "OgreBulletCollisionsObject.h"
    #include "OgreBulletCollisionsObjectState.h"

    #include "OgreBulletCollisionsWorld.h"

    #include "OgreBulletCollisionsRay.h"





    #include "OgreString.h"
    #include "OgreFontManager.h"

    #include "BulletCollision/CollisionShapes/btMinkowskiSumShape.h"
    #include "BulletCollision/CollisionShapes/btTriangleShape.h"


#endif //_PRECOMP

#endif //_OGREBULLETCOLLISIONS_H

