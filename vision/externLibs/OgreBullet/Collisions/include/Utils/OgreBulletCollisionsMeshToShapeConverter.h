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
#ifndef _OgreBulletCollisionsMeshToShapeConverter_H_
#define _OgreBulletCollisionsMeshToShapeConverter_H_

#include "OgreBulletCollisionsPreRequisites.h"

namespace OgreBulletCollisions 
{
	class VertexIndexToShape
	{
	public:
		VertexIndexToShape(const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		~VertexIndexToShape();

		Ogre::Real      getRadius();
		Ogre::Vector3   getSize();


		SphereCollisionShape*           createSphere();
		BoxCollisionShape*              createBox();
		TriangleMeshCollisionShape*     createTrimesh();
		CylinderCollisionShape*         createCylinder();
		ConvexHullCollisionShape*       createConvex();


		const Ogre::Vector3*    getVertices();
		unsigned int            getVertexCount();
		const unsigned int*     getIndices();
		unsigned int            getIndexCount();

	protected:

		void addStaticVertexData(const Ogre::VertexData *vertex_data);

		void addAnimatedVertexData(const Ogre::VertexData *vertex_data, 
							const Ogre::VertexData *blended_data, 
							const Ogre::Mesh::IndexMap *indexMap);

		void addIndexData(Ogre::IndexData *data, const unsigned int offset = 0);

	
	protected:
		Ogre::Vector3*	    mVertexBuffer;
		unsigned int*       mIndexBuffer;
		unsigned int        mVertexCount;
		unsigned int        mIndexCount;

		Ogre::Matrix4		mTransform;

		Ogre::Real		    mBoundRadius;
		Ogre::Vector3		mBounds;

		BoneIndex           *mBoneIndex;

	};

	class StaticMeshToShapeConverter : public VertexIndexToShape
	{
	public:

		StaticMeshToShapeConverter(Ogre::Renderable *rend, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		StaticMeshToShapeConverter(Ogre::Entity *entity,   const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		StaticMeshToShapeConverter();

		~StaticMeshToShapeConverter();

		void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		
		void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);


	protected:

		Ogre::Entity*		mEntity;
		Ogre::SceneNode*	mNode;

	};

	class AnimatedMeshToShapeConverter : public VertexIndexToShape
	{
	public:

		AnimatedMeshToShapeConverter(Ogre::Entity *entity, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		AnimatedMeshToShapeConverter();
		~AnimatedMeshToShapeConverter();

		void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform);

		BoxCollisionShape* createAlignedBox(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation);

		BoxCollisionShape* createOrientedBox(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation);

		CapsuleCollisionShape* createOrientedCapsuleCollisionShape(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation);

	protected:

		bool getBoneVertices(unsigned char bone, 
			unsigned int &vertex_count, 
			Ogre::Vector3* &vertices,
			const Ogre::Vector3 &bonePosition);

		bool getOrientedBox(unsigned char bone, 
			const Ogre::Vector3 &bonePosition,
			const Ogre::Quaternion &boneOrientation,
			Ogre::Vector3 &extents,
			Ogre::Vector3 *axis,
			Ogre::Vector3 &center);

		
		Ogre::Entity*		mEntity;
		Ogre::SceneNode*	mNode;

		Ogre::Vector3       *mTransformedVerticesTemp;
		size_t               mTransformedVerticesTempSize;

	};

}

#endif




