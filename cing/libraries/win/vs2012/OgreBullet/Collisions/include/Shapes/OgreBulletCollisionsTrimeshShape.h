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

#ifndef _OGREBULLETCOLLISIONS_TrimeshShape_H
#define _OGREBULLETCOLLISIONS_TrimeshShape_H

#include "OgreBulletCollisionsPreRequisites.h"

#include "OgreBulletCollisionsShape.h"

namespace OgreBulletCollisions
{
    // -------------------------------------------------------------------------
    // basic TrimeshShape
    class TriangleMeshCollisionShape : public CollisionShape
    {
	public:
		TriangleMeshCollisionShape(Ogre::Vector3 *_vertices, unsigned int _vertex_count, unsigned int *_indices, unsigned int_index_count, bool use32bitsIndices = true);
	    virtual ~TriangleMeshCollisionShape();

		bool drawWireFrame(DebugLines *wire, 
			const Ogre::Vector3 &pos = Ogre::Vector3::ZERO, 
			const Ogre::Quaternion &quat= Ogre::Quaternion::IDENTITY) const;

    private:
        btTriangleMesh*         mTriMesh;
    };
}
#endif //_OGREBULLETCOLLISIONS_TrimeshShape_H

