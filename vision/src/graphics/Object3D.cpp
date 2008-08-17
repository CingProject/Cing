/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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
#include "common/MathUtils.h"
#include "common/LogManager.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreSceneManager.h"
#include "externLibs/Ogre3d/include/OgreEntity.h"
#include "externLibs/Ogre3d/include/OgreSubEntity.h"
#include "externLibs/Ogre3d/include/OgreMeshManager.h"
#include "externLibs/Ogre3d/include/OgreMaterialManager.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"


namespace Graphics
{

// Static member init
long					Object3D::m_objectCounter				= 0;
const float		Object3D::OGRE_SCALE_CORRECTION = 1;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Object3D::Object3D():
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
void Object3D::init( const std::string& meshName /*= ""*/, const std::string& materialName /*= ""*/ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return;

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
		LOG_ERROR( "Trying to set a material (%s) that does not exist", materialName );


	// This is a triangle mesh type
	m_type = TRIANGLE_MESH;

	// The class is now initialized
	m_bIsValid = true;
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

	// The class is not valid anymore
	m_bIsValid = false;
}


/**
 * @brief Returns the current position of the object
 *
 * @return The current position of the object in the scene
 */
const Vector& Object3D::getPosition()
{
	// Check if the class is already released
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to get the position of a 3d Object that has been already released (method end() has been called)" );

	// Return the position
	return m_sceneNode->getPosition();
}

/** 
 * @brief Returns the current orientation of the object
 *
 * @return The current position of the object in the scene
 */
const Quaternion& Object3D::getOrientation() const
{
	// Check if the class is already released
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to get the orientation of a 3d Object that has been already released (method end() has been called)" );

	// Return the position
	return m_sceneNode->getOrientation();	
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
void Object3D::setPosition( const Vector& pos  )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in Object3D not correctly initialized" );

  setPosition( pos.x, pos.y, pos.z ); 
}


/**
 * @internal 
 * @brief Sets the new orientation of this object.
 *
 * @param orientation new orientation of this object. It is represented in a quaternion, this is, 
 * a rotation around an axis
 */
void Object3D::setOrientation( const Quaternion& orientation )
{
	if ( !isValid() )
		THROW_EXCEPTION( "Error. Trying to set orientation in Object3D not correctly initialized" );

	m_sceneNode->setOrientation( orientation );
}

/**
 * @internal 
 * @brief Sets the new orientation of this object.
 *
 * @param axis	Axis of rotation
 * @param angle Angle of rotation in degrees around the specified axis
 */
void Object3D::setOrientation( const Vector& axis, float angle )
{
	if ( !isValid() )
		THROW_EXCEPTION( "Error. Trying to set orientation in Object3D not correctly initialized" );

	Quaternion q( Ogre::Radian( Ogre::Degree( angle ) ), axis );
	m_sceneNode->setOrientation( q );
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

	// Set the normal normalization by hw, because if not, the scaling can cause
	// the object not to be lighted properly
	// -> do this, just when the scale is not 1
	if ( !equal( xScale, 1.0f ) || !equal( yScale, 1.0f ) || !equal( zScale, 1.0f ) )
		m_entity->setNormaliseNormals( true );
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
void Object3D::setScale( const Vector& scale )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set scale in Object3D not correctly initialized" );

  setScale( scale.x, scale.y, scale.z );
}

/**
 * @internal
 * @brief Makes this object to be constantly looking at the target object provided
 * @param[in] objectToTrack object that will be looked
 */
void Object3D::lookAt( Object3D& objectToTrack )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying track object in an Object3D not correctly initialized" );
  
  // TODO hacer offset configurable...
  m_sceneNode->setAutoTracking( true, objectToTrack.getSceneNode(), Vector::UNIT_Z );
}

//--------------------------------------------------------------------------------------------------
// COLOR MODIFICATION
//--------------------------------------------------------------------------------------------------

/*
 * @brief Sets the ambient color of the object.
 *
 * @param color New color of the object.
 */
void Object3D::setAmbientColor( const Color& color )
{
	// Duplicate material (if is not duplicated) to modify just this instance's material
	duplicateMaterial();

	// Change the ambient color of the copy material
	m_materialCopy->setAmbient( color.normalized() );
}

/*
 * @brief Sets the ambient color of the object.
 *
 * @param color New color of the object.
 * @param alpha Alpha or transparency level of the object. 0 means transparent, 255 means opaque.
 */
void Object3D::setAmbientColor( const Color& color, int alpha )
{
	setAmbientColor( Color ( color.r, color.g, color.b, alpha ) );
}

/*
 * @brief Sets the ambient color of the object in gray scale
 *
 * @param gray New gray color of the object.
 */
void Object3D::setAmbientColor( int gray )
{
	setAmbientColor( Color( gray, gray, gray ) );
}

/*
 * @brief Sets the ambient color of the object.
 *
 * @param gray	New gray color of the object.
 * @param alpha New alpha value.
 */
void Object3D::setAmbientColor( int gray, int alpha )
{
	setAmbientColor( Color ( gray, gray, gray, alpha ) );
}

/*
 * @brief Sets the ambient color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setAmbientColor( int value1, int value2, int value3 )
{
	setAmbientColor( Color ( value1, value2, value3 ) );
}

/*
 * @brief Sets the ambient color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setAmbientColor( int value1, int value2, int value3, int alpha )
{

	setAmbientColor( Color ( value1, value2, value3, alpha ) );
}

/*
 * @brief Sets the diffuse color of the object.
 *
 * @param color New color of the object.
 */
void Object3D::setDiffuseColor( const Color& color )
{
	// Duplicate material (if is not duplicated) to modify just this instance's material
	duplicateMaterial();

	// Change the ambient color of the copy material
	m_materialCopy->setDiffuse( color.normalized() );
}

/*
 * @brief Sets the diffuse color of the object.
 *
 * @param color New color of the object.
 * @param alpha Alpha or transparency level of the object. 0 means transparent, 255 means opaque.
 */
void Object3D::setDiffuseColor( const Color& color, int alpha )
{
	setDiffuseColor( Color ( color.r, color.g, color.b, alpha ) );
}

/*
 * @brief Sets the diffuse color of the object in gray scale
 *
 * @param gray New gray color of the object.
 */
void Object3D::setDiffuseColor( int gray )
{
	setDiffuseColor( Color( gray, gray, gray ) );
}

/*
 * @brief Sets the diffuse color of the object.
 *
 * @param gray	New gray color of the object.
 * @param alpha New alpha value.
 */
void Object3D::setDiffuseColor( int gray, int alpha )
{
	setDiffuseColor( Color ( gray, gray, gray, alpha ) );
}

/*
 * @brief Sets the diffuse color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setDiffuseColor( int value1, int value2, int value3 )
{
	setDiffuseColor( Color ( value1, value2, value3 ) );
}

/*
 * @brief Sets the diffuse color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setDiffuseColor( int value1, int value2, int value3, int alpha )
{

	setDiffuseColor( Color ( value1, value2, value3, alpha ) );
}

/*
 * @brief Sets the specular color of the object.
 *
 * @param color New color of the object.
 */
void Object3D::setSpecularColor( const Color& color )
{
	// Duplicate material (if is not duplicated) to modify just this instance's material
	duplicateMaterial();

	// Change the ambient color of the copy material
	m_materialCopy->setSpecular( color.normalized() );
}

/*
 * @brief Sets the specular color of the object.
 *
 * @param color New color of the object.
 * @param alpha Alpha or transparency level of the object. 0 means transparent, 255 means opaque.
 */
void Object3D::setSpecularColor( const Color& color, int alpha )
{
	setSpecularColor( Color ( color.r, color.g, color.b, alpha ) );
}

/*
 * @brief Sets the specular color of the object in gray scale
 *
 * @param gray New gray color of the object.
 */
void Object3D::setSpecularColor( int gray )
{
	setSpecularColor( Color( gray, gray, gray ) );
}

/*
 * @brief Sets the specular color of the object.
 *
 * @param gray	New gray color of the object.
 * @param alpha New alpha value.
 */
void Object3D::setSpecularColor( int gray, int alpha )
{
	setSpecularColor( Color ( gray, gray, gray, alpha ) );
}

/*
 * @brief Sets the specular color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setSpecularColor( int value1, int value2, int value3 )
{
	setSpecularColor( Color ( value1, value2, value3 ) );
}

/*
 * @brief Sets the specular color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setSpecularColor( int value1, int value2, int value3, int alpha )
{

	setSpecularColor( Color ( value1, value2, value3, alpha ) );
}

/*
 * @brief Sets the specular color of the object.
 *
 * @param color New color of the object.
 */
void Object3D::setSelfIlluminationColor( const Color& color )
{
	// Duplicate material (if is not duplicated) to modify just this instance's material
	duplicateMaterial();

	// Change the ambient color of the copy material
	m_materialCopy->setSelfIllumination( color.normalized() );
}

/*
 * @brief Sets the self illumination color of the object.
 *
 * @param color New color of the object.
 * @param alpha Alpha or transparency level of the object. 0 means transparent, 255 means opaque.
 */
void Object3D::setSelfIlluminationColor( const Color& color, int alpha )
{
	setSelfIlluminationColor( Color ( color.r, color.g, color.b, alpha ) );
}

/*
 * @brief Sets the self illumination color of the object in gray scale
 *
 * @param gray New gray color of the object.
 */
void Object3D::setSelfIlluminationColor( int gray )
{
	setSelfIlluminationColor( Color( gray, gray, gray ) );
}

/*
 * @brief Sets the self illumination color of the object.
 *
 * @param gray	New gray color of the object.
 * @param alpha New alpha value.
 */
void Object3D::setSelfIlluminationColor( int gray, int alpha )
{
	setSelfIlluminationColor( Color ( gray, gray, gray, alpha ) );
}

/*
 * @brief Sets the self illumination color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setSelfIlluminationColor( int value1, int value2, int value3 )
{
	setSelfIlluminationColor( Color ( value1, value2, value3 ) );
}

/*
 * @brief Sets the self illumination color of the object.
 *
 * @param value1	Firs value of the color (red or hue, depending on the color mode)
 * @param value2	Second value of the color (green or saturation, depending on the color mode)
 * @param value3	Third value of the color (blue or brightness, depending on the color mode)
 * @param alpha New alpha value.
 */
void Object3D::setSelfIlluminationColor( int value1, int value2, int value3, int alpha )
{

	setSelfIlluminationColor( Color ( value1, value2, value3, alpha ) );
}

/**
 * @brief Uses a file as color texture to render this object
 *
 * @param textureFileName Name of the file to use as color texture. It should be in the data folder
 */
void Object3D::setTexture( const std::string& textureFileName )
{
	// Duplicate material (if is not duplicated) to modify just this instance's material
	duplicateMaterial();

	// Remove texture units (just in case)
	m_materialCopy->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();

	// Change the ambient color of the copy material
	m_materialCopy->getTechnique(0)->getPass(0)->createTextureUnitState( textureFileName );
}

/**
 * @brief Sets the material for the object
 *
 * @param materialName Name of the material to assign. It must be located in the data folder.
 */
void Object3D::setMaterial( const std::string& materialName )
{
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
		LOG_ERROR( "Trying to set a material (%s) that does not exist", materialName );
}

/**
 * @brief Sets the object visible or invisible
 *
 * @param visible if true the object will be visible, if false it will be invisible
 */
void Object3D::setVisible( bool visible )
{
	if ( isValid() )
		m_sceneNode->setVisible( visible );
}

/**
 * @internal 
 * @brief Shows the object's bounding box (or hides it)
 *
 * @param show If true, the object's bounding is rendered, if false it is hidden
 */
void Object3D::showBoundingBox( bool show )
{
	m_sceneNode->showBoundingBox( show );
}


/**
 * @internal 
 * @brief Duplicates the object's material, and stores the copy in the attribute m_materialCopy.
 *
 * If the material has been duplicated before, this method does nothing. The material duplication
 * is done to allow modification of the material, but just for this instance, not affecting the rest
 * of objects that are using the same material
 */
void Object3D::duplicateMaterial()
{
	// Check if the material is already duplicated
	if ( m_materialCopy.isNull() )
	{
		// Clone the material with a unique name (original name + object name (unique))
		Ogre::MaterialPtr origMaterial = m_entity->getSubEntity(0)->getMaterial();	
		std::string newMaterialName = origMaterial->getName() + m_objectName ;
		m_materialCopy = origMaterial->clone( newMaterialName );

		// Assign the material
		m_entity->setMaterialName( newMaterialName );
	}
}

} // namespace Graphics
