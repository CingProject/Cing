/*
This source file is part of the Cing project
For the latest info, see http://www.cing.cc

Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "Object3D.h"

// Graphics
#include "graphics/GraphicsManager.h"

// Framework
#include "framework/UserAppGlobals.h"
#include "framework/Application.h"


// Common
#include "common/Exception.h"
#include "common/Release.h"
#include "common/MathUtils.h"
#include "common/LogManager.h"

// Ogre
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreSubEntity.h"
#include "OgreMeshManager.h"
#include "OgreMaterialManager.h"
#include "OgreStringConverter.h"


namespace Cing
{

	// Static member init
	long			Object3D::m_objectCounter		= 0;
	const float		Object3D::OGRE_SCALE_CORRECTION = 1;

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	Object3D::Object3D():
		m_entity	( NULL  ),
		m_bIsValid	( false )
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
	* @param[in] parent		   Parent scene node. If not passed a new node will be created
	* @return true if the initialization was ok | false otherwise
	*/
	void Object3D::init( const std::string& meshName /*= ""*/, const std::string& materialName /*= ""*/, Ogre::SceneNode* parent /*= NULL*/ )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return;

		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();

		// Create a node in the scene for the object
		Ogre::SceneManager& sceneManager = GraphicsManager::getSingleton().getSceneManager();
		if ( parent )
			m_sceneNode = parent->createChildSceneNode();
		else
			m_sceneNode = sceneManager.getRootSceneNode()->createChildSceneNode();

		// By default we disable scale inheritance, as it is only useful when you create
		// a 3d object composed by many subobjects, but to create hierarchies with Scene Nodes, you usually don't want
		// the scale from the parent node to affect the child node
		m_sceneNode->setInheritScale(false);
		m_sceneNode->setInheritOrientation(false);
			

		// TODO: definir materiales básicos, y checkear hw capabilities según el que sea

		// Create the mesh
		if ( meshName != "" )
		{
			// Load the mesh (in case it has not been loaded before)
			Ogre::MeshPtr pMesh = Ogre::MeshManager::getSingleton().load( meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
			

			// Check if stencil shadows are enabled to build the mesh edges
			bool shadowsEnabled = GraphicsManager::getSingleton().shadowsEnabled();
			if ( shadowsEnabled && 
				 ((GraphicsManager::getSingleton().getCurrentShadowTechnique() == STENCIL_ADDITIVE) || 
				 (GraphicsManager::getSingleton().getCurrentShadowTechnique() == STENCIL_MODULATIVE)  ) )
			{		
				pMesh->buildEdgeList();
			}

			// TODO remove this from here
			// Build tangent vectors (for normal mapping), all our meshes use only 1 texture coordset 
			//unsigned short src, dest;
			//if ( !pMesh->suggestTangentVectorBuildParams( Ogre::VES_TANGENT, src, dest ) )
			//{
			//  pMesh->buildTangentVectors( Ogre::VES_TANGENT, src, dest );
			//}

			// Create a unique name for the entity
			m_objectName = meshName + Ogre::StringConverter::toString( ++m_objectCounter );

			// Create the entity and attach it to the scene node, and assign material
			m_entity = sceneManager.createEntity( m_objectName, meshName );
			m_sceneNode->attachObject( m_entity );

			// Set the object's material
			setMaterial( materialName );
		}


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

		// Destroy Ogre resources
		Ogre::SceneManager* sceneManager = GraphicsManager::getSingleton().getSceneManagerPtr();
		if ( sceneManager )
		{
			sceneManager->destroyEntity( m_entity );
			m_entity = NULL;
		}

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
	* @brief Returns the current scale of the object
	*
	* @return The current scale of the object in the scene
	*/
	const Vector& Object3D::getScale() const
	{
		// Check if the class is already released
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to get the scale of a 3d Object that has been already released (method end() has been called)" );

		// Return the position
		return m_sceneNode->getScale();	
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
	* @brief Translates the object's position
	* @param[in] x x translation(horizontal axis) of the object in the scene
	* @param[in] y y translation (vertical axis) of the object in the scene
	*/
	void Object3D::translate( float x, float y )
	{
		translate( x, y, 0 );
	}

	/**
	* @brief Translates the object's position
	* @param[in] x x translation(horizontal axis) of the object in the scene
	* @param[in] y y translation (vertical axis) of the object in the scene
	* @param[in] z z translation (depth) of the object in the scene
	*/
	void Object3D::translate( float x, float y, float z )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set position in Object3D not correctly initialized" );

		// Set the absolute object position
		m_sceneNode->translate( x, y, z );
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
	void Object3D::setOrientation( const Vector& axis, float angleDegrees )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to set orientation in Object3D not correctly initialized" );

		Quaternion q( Ogre::Radian( Ogre::Degree( angleDegrees ) ), axis );
		m_sceneNode->setOrientation( q );
	}


	/**
	* @brief Rotates the object around the Z axis
	*
	* @param angleRad Angle of rotation in radians around the specified axis
	*/
	void Object3D::rotate( float angleRad )
	{
		rotateZ( angleRad );
	}


	/**
	* @brief Rotates the object around the X axis
	*
	* @param angleRad Angle of rotation in radians around the specified axis
	*/
	void Object3D::rotateX( float angleRad )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to rotate a Object3D not correctly initialized" );

		Quaternion q( Ogre::Radian( angleRad ), Vector::UNIT_X );
		m_sceneNode->rotate( q );
	}

	/**
	* @brief Rotates the object around the Y axis
	*
	* @param angleRad Angle of rotation in radians around the specified axis
	*/
	void Object3D::rotateY( float angleRad )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to rotate a Object3D not correctly initialized" );

		Quaternion q( Ogre::Radian( angleRad ), Vector::UNIT_Y );
		m_sceneNode->rotate( q );
	}

	/**
	* @brief Rotates the object around the Z axis
	*
	* @param angleRad Angle of rotation in radians around the specified axis
	*/
	void Object3D::rotateZ( float angleRad )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to rotate a Object3D not correctly initialized" );

		Quaternion q( Ogre::Radian( angleRad ), Vector::UNIT_Z );
		m_sceneNode->rotate( q );
	}

	/**
	* @brief Rotates the object by a certain angle around an axis
	*
	* @param axis	Axis of rotation
	* @param Rad Angle of rotation in radians around the specified axis
	*/
	void Object3D::rotate( const Vector& axis, float angleRad )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to rotate a Object3D not correctly initialized" );

		Quaternion q( Ogre::Radian( angleRad ), axis );
		m_sceneNode->rotate( q );
	}

	/**
	* @internal 
	* @brief Rotates the object by a certain quaternion
	*
	* @param quat	Quaternion to rotate the object
	*/
	void Object3D::rotate( const Quaternion& quat )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Error. Trying to rotate a Object3D not correctly initialized" );

		m_sceneNode->rotate( quat );
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
	void Object3D::setAmbientColor( const Color& color, float alpha )
	{
		setAmbientColor( Color ( color.r, color.g, color.b, alpha ) );
	}

	/*
	* @brief Sets the ambient color of the object in gray scale
	*
	* @param gray New gray color of the object.
	*/
	void Object3D::setAmbientColor( float gray )
	{
		setAmbientColor( Color( gray, gray, gray ) );
	}

	/*
	* @brief Sets the ambient color of the object.
	*
	* @param gray	New gray color of the object.
	* @param alpha New alpha value.
	*/
	void Object3D::setAmbientColor( float gray, float alpha )
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
	void Object3D::setAmbientColor( float value1, float value2, float value3 )
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
	void Object3D::setAmbientColor( float value1, float value2, float value3, float alpha )
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
	void Object3D::setDiffuseColor( const Color& color, float alpha )
	{
		setDiffuseColor( Color ( color.r, color.g, color.b, alpha ) );
	}

	/*
	* @brief Sets the diffuse color of the object in gray scale
	*
	* @param gray New gray color of the object.
	*/
	void Object3D::setDiffuseColor( float gray )
	{
		setDiffuseColor( Color( gray, gray, gray ) );
	}

	/*
	* @brief Sets the diffuse color of the object.
	*
	* @param gray	New gray color of the object.
	* @param alpha New alpha value.
	*/
	void Object3D::setDiffuseColor( float gray, float alpha )
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
	void Object3D::setDiffuseColor( float value1, float value2, float value3 )
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
	void Object3D::setDiffuseColor( float value1, float value2, float value3, float alpha )
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
	void Object3D::setSpecularColor( const Color& color, float alpha )
	{
		setSpecularColor( Color ( color.r, color.g, color.b, alpha ) );
	}

	/*
	* @brief Sets the specular color of the object in gray scale
	*
	* @param gray New gray color of the object.
	*/
	void Object3D::setSpecularColor( float gray )
	{
		setSpecularColor( Color( gray, gray, gray ) );
	}

	/*
	* @brief Sets the specular color of the object.
	*
	* @param gray	New gray color of the object.
	* @param alpha New alpha value.
	*/
	void Object3D::setSpecularColor( float gray, float alpha )
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
	void Object3D::setSpecularColor( float value1, float value2, float value3 )
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
	void Object3D::setSpecularColor( float value1, float value2, float value3, float alpha )
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
	void Object3D::setSelfIlluminationColor( const Color& color, float alpha )
	{
		setSelfIlluminationColor( Color ( color.r, color.g, color.b, alpha ) );
	}

	/*
	* @brief Sets the self illumination color of the object in gray scale
	*
	* @param gray New gray color of the object.
	*/
	void Object3D::setSelfIlluminationColor( float gray )
	{
		setSelfIlluminationColor( Color( gray, gray, gray ) );
	}

	/*
	* @brief Sets the self illumination color of the object.
	*
	* @param gray	New gray color of the object.
	* @param alpha New alpha value.
	*/
	void Object3D::setSelfIlluminationColor( float gray, float alpha )
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
	void Object3D::setSelfIlluminationColor( float value1, float value2, float value3 )
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
	void Object3D::setSelfIlluminationColor( float value1, float value2, float value3, float alpha )
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
			LOG_ERROR( "Trying to set a material (%s) that does not exist", materialName.c_str() );
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
	 * @brief Returns whether the object is visible or not
	 *
	 * @return true if it is visible, false otherwise
	 */
	bool Object3D::isVisible() const
	{
		if ( isValid() )
			return m_entity->isVisible();
		return false;
	}

	/**
	 * @brief Sets the render queue in which this object will be rendered. This allows to control
	 * which objects are rendered first ant which last.ç
	 * @note Render queue index values range from 0 to 105, being 0 de background objects and 105 the las objects to be rendered (such as GUI)
	 *
	 * @param queue Number of the queue
	 */
	void Object3D::setRenderQueueGroup( int queue )
	{
		if ( isValid() )
			m_entity->setRenderQueueGroup( queue );
	}


	/**
	 * Configures the object to cast or not shadows
	 * @param[in] cast if true, light will cast shadows
	 */
	void Object3D::castShadows( bool cast )
	{
		if ( isValid() )
			m_entity->setCastShadows(cast);
	}

	/**
	 * Returns a reference to the material of the object
	 * @return material of the object
	 */	
	Ogre::MaterialPtr Object3D::getMaterial()
	{	
		if ( !isValid() )
		{
			LOG_ERROR( "Call to getMaterial when the object has not been initialized yet. Returning empty material" );
			return m_materialCopy;
		}

		// Check if the material is duplicated
		if ( m_materialCopy.isNull() )
			return m_entity->getSubEntity(0)->getMaterial();
		else
			return m_materialCopy;
	}

	/**
	* @internal 
	* @brief Shows the object's bounding box (or hides it)
	*
	* @param show If true, the object's bounding is rendered, if false it is hidden
	*/
	void Object3D::showBoundingBox( bool show )
	{
		if ( m_sceneNode )
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

} // namespace Cing
