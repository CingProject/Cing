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

#include "Object3D.h"

// Graphics
#include "graphics/GraphicsManager.h"

// Framework
#include "framework/UserAppGlobals.h"

// Common
#include "common/Exception.h"
#include "common/Release.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreSceneManager.h"
#include "externLibs/Ogre3d/include/OgreEntity.h"
#include "externLibs/Ogre3d/include/OgreSubEntity.h"
#include "externLibs/Ogre3d/include/OgreMeshManager.h"
#include "externLibs/Ogre3d/include/OgreMaterialManager.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"

// Physics
#include "physics/PhysicsObject.h"

namespace Graphics
{

// Static member init
long Object3D::m_objectCounter = 0;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Object3D::Object3D():
	m_physicsObject	( NULL  ),
  m_sceneNode			( NULL  ),
  m_entity				( NULL  ),
  m_bIsValid			( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Object3D::~Object3D()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] meshName      Name of the mesh  (.mesh file) to be used by the object. If empty the object will have no mesh.
 * @param[in] materialName  Name of the material (defines the visual aspect) of the object. If empty the object will have no material.
 * @return true if the initialization was ok | false otherwise
 */
bool Object3D::init( const std::string& meshName /*= ""*/, const std::string& materialName /*= ""*/ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Create a node in the scene for the object
  Ogre::SceneManager& sceneManager = Graphics::GraphicsManager::getSingleton().getSceneManager();
  m_sceneNode = sceneManager.getRootSceneNode()->createChildSceneNode();

  // TODO: definir materiales básicos, y checkear hw capabilities según el que sea

  // Create the mesh
  if ( meshName != "" )
  {
    Ogre::MeshPtr pMesh = Ogre::MeshManager::getSingleton().load( meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

		// TODO remove this from here
    // Build tangent vectors (for normal mapping), all our meshes use only 1 texture coordset 
    unsigned short src, dest;
    if ( !pMesh->suggestTangentVectorBuildParams( Ogre::VES_TANGENT, src, dest ) )
    {
      pMesh->buildTangentVectors( Ogre::VES_TANGENT, src, dest );
    }

		// Create a unique name for the entity
		m_objectName = meshName + Ogre::StringConverter::toString( ++m_objectCounter );

    // Create the entity and attach it to the scene node, and assign material
    m_entity = sceneManager.createEntity( m_objectName, meshName );
    m_sceneNode->attachObject( m_entity );
  }

	// Check if the material exist
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName( materialName );
	if ( !material.isNull() )
	{
		// Assign material
		if ( materialName != "" )
			m_entity->setMaterialName( materialName );  
	}
	// TODO: Log -> material does not exist
	else
		THROW_EXCEPTION( "Trying to set a material that does not exist" );


	// This is a triangle mesh type
	m_type = TRIANGLE_MESH;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Object3D::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// Release the physics (just in case are activated)
	Common::Release( m_physicsObject );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Sets the new position of the object.
 * @param[in] x new x coordinate (horizontal axis) of the object in the scene
 * @param[in] y new y coordinate (vertical axis) of the object in the scene
 * @param[in] z new z coordinate (depth) of the object in the scene
 */
void Object3D::setPosition( float x, float y, float z )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in Object3D not correctly initialized" );

  // Set the absolute object position
  m_sceneNode->setPosition( x, y, z );
}

/**
 * @internal
 * @brief Sets the new position of the object. z coordinate (depth) is assumed to be 0.
 * @param[in] x new x coordinate of the object in the scene
 * @param[in] y new y coordinate of the object in the scene
 */
void Object3D::setPosition( float x, float y )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in Object3D not correctly initialized" );

  setPosition( x, y, 0.0f ); 
}

/**
 * @internal
 * @brief Sets the new position of the object.
 * @param[in] pos new absolute position of the object in the scene
 */
void Object3D::setPosition( const Vector3& pos  )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in Object3D not correctly initialized" );

  setPosition( pos.x, pos.y, pos.z ); 
}

/**
 * @internal
 * @brief Sets the new scale of the object.
 * @param[in] x new x scale (horizontal axis) of the object in the scene
 * @param[in] y new y scale (vertical axis) of the object in the scene
 * @param[in] z new z scale (depth) of the object in the scene
 */
void Object3D::setScale( float xScale, float yScale, float zScale )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set scale in Object3D not correctly initialized" );

  // Set the scale of the object 
  m_sceneNode->setScale( xScale, yScale, zScale );
}

/**
 * @internal
 * @brief Sets the new scale of the object. Z scale is not modified
 * @param[in] x new x scale (horizontal axis) of the object in the scene
 * @param[in] y new y scale (vertical axis) of the object in the scene
 */
void Object3D::setScale( float xScale, float yScale )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set scale in Object3D not correctly initialized" );

  setScale( xScale, yScale, 1.0f );
}

/**
 * @internal
 * @brief Sets the new scale of the object. The three axis are scaled equally
 * @param[in] scale new scale of the object in the scene
 */
void Object3D::setScale( float scale )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set scale in Object3D not correctly initialized" );

  setScale( scale, scale, scale );
}

/**
 * @internal
 * @brief Sets the new scale of the object.
 * @param[in] scale vector containing the scale in the three axis of the object
 */
void Object3D::setScale( const Vector3& scale )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set scale in Object3D not correctly initialized" );

  setScale( scale.x, scale.y, scale.z );
}

/**
 * @internal
 * @brief Makes this object to be constantly looking at the target object provided
 * @param[in] objectToTrack object that will be tracke
 */
void Object3D::trackObject( Object3D& objectToTrack )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying track object in an Object3D not correctly initialized" );
  
  // TODO hacer offset configurable...
  m_sceneNode->setAutoTracking( true, objectToTrack.getSceneNode(), Vector3::UNIT_Z );
}

/*
 * @brief Sets the color of the object.
 *
 * @param color New color of the object.
 */
void Object3D::setColor( const Color& color )
{
	// If this instance is going to have a specific color, it is necessary to 
	// duplicate the material assigned (if we haven't done before)
	if ( m_materialCopy.isNull() )
	{
		// Clone the material with a unique name (original name + object name (unique))
		Ogre::MaterialPtr origMaterial = m_entity->getSubEntity(0)->getMaterial();	
		std::string newMaterialName = origMaterial->getName() + m_objectName ;
		m_materialCopy = origMaterial->clone( newMaterialName );

		// Assign the material
		m_entity->setMaterialName( newMaterialName );
	}

	// Change the ambient color of the copy material
	m_materialCopy->setAmbient( color );
}

/*
 * @brief Sets the color of the object.
 *
 * @param color New color of the object.
 * @param alpha Alpha or transparency level of the object. 0 means transparent, 255 means opaque.
 */
void Object3D::setColor( const Color& color, int alpha )
{
	setColor( Color ( color.r, color.g, color.b, alpha ) );
}

/*
 * @brief Sets the color of the object in gray scale
 *
 * @param gray New gray color of the object.
 */
void Object3D::setColor( int gray )
{
	setColor( Color( gray, gray, gray ) );
}

/*
 * @brief Sets the color of the object.
 *
 * @param gray	New gray color of the object.
 * @param alpha New alpha value.
 */
void Object3D::setColor( int gray, int alpha )
{
	setColor( Color ( gray, gray, gray, alpha ) );
}

/*
 * @brief Sets the color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setColor( int value1, int value2, int value3 )
{
	setColor( Color ( value1, value2, value3 ) );
}

/*
 * @brief Sets the color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setColor( int value1, int value2, int value3, int alpha )
{
	setColor( Color ( value1, value2, value3, alpha ) );
}


/*
 * @brief Activates the physics for this object. 
 * 
 * From now on, the object's position and rotation will be driven by the physics of the scene.
 */
void Object3D::activatePhysics()
{
	// Create and init the physics object
	m_physicsObject = new Physics::PhysicsObject();
	m_physicsObject->init( *this );
}

/*
 * @brief DeActivates the physics for this object. 
 * 
 * From now on, the object's position and rotation will be again controlled manually
 */
void Object3D::deActivatePhysics()
{
	// Release physics object
	Common::Release( m_physicsObject );
}

} // namespace Graphics