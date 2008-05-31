/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _Object3D_H_
#define _Object3D_H_

#include "GraphicsPrereqs.h"

// Common
#include "common/CommonTypes.h"

namespace Graphics
{

/**
 * @internal
 * Represents a 3d object
 */
class Object3D
{
public:

	// Types of 3d Objects
	enum Object3DType
	{
		BOX,						///< Box object, this is, a cube
		TRIANGLE_MESH		///< Non primitive objects, this is, any 3d triangle mesh
	};

	// Constructor / Destructor
	Object3D();
	virtual ~Object3D();

	// Init / Release / Update
	bool              init        			( const std::string& meshName = "", const std::string& materialName = "");
	void              end         			();

	// Query methods
	bool              isValid     			() const { return m_bIsValid; }
	Ogre::SceneNode*  getSceneNode			() { return m_sceneNode;			}
	Ogre::Entity*			getEntity					() { return m_entity;					}
	const std::string getName						() { return m_objectName;			}
	Object3DType			getType						() { return m_type;						}

	// Set methods
	void							setType						( Object3DType type ) { m_type = type; }
	void              setPosition 			( float x, float y, float z );
	void              setPosition 			( float x, float y );  
	void              setPosition 			( const Vector3& pos );
	void              setScale    			( float xScale, float yScale, float zScale );
	void              setScale    			( float xScale, float yScale );
	void              setScale    			( float scale );
	void              setScale    			( const Vector3& scale );
	void              trackObject 			( Object3D& objectToTrack );
	void							setColor					( const Color& color );
	void							setColor					( const Color& color, int alpha );
	void							setColor					( int gray );
	void							setColor					( int gray, int alpha );
	void							setColor					( int value1, int value2, int value3 );
	void							setColor					( int value1, int value2, int value3, int alpha );

	// Physics related methods
	virtual void			activatePhysics		();
	void							deActivatePhysics	();

protected:

	Physics::PhysicsObject*		m_physicsObject;///< Physics object that will control this 3d object in case the physics is activated for it

private:

	// Constant / static attributes
	static long       m_objectCounter;///< Used to generate unique names for the 3d objects

	// Attributes
	Ogre::SceneNode* 					m_sceneNode;		///< Node in the scene manager where the object is placed
	Ogre::Entity*    					m_entity;				///< Ogre entity that represents the model
	Ogre::MaterialPtr					m_materialCopy;	///< Entity's material copy (to allow this object to have a specific color) 
	std::string       				m_objectName;		///< Unique object name
	Object3DType							m_type;					///< Type of 3d object
	bool              				m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _Object3D_H_
