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

#ifndef _Cing_Object3D_H_
#define _Cing_Object3D_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"
#include "SceneGraphElement.h"
#include "Color.h"

// Common
#include "common/CommonTypes.h"

// Ogre
#include "OgreMaterial.h"

namespace Cing
{

/**
 * @internal
 * Represents a 3d object
 */
class Object3D: public SceneGraphElement
{
public:

	// Types of 3d Objects
	enum Object3DType
	{
		BOX,						///< Box primitive object, this is, a cube
		SPHERE,					///< Sphere primitive object
		PLANE,					///< Plane primitive object
		CONE,						///< Cone primitive object
		TRIANGLE_MESH		///< Non primitive objects, this is, any 3d triangle mesh
	};

	// Constructor / Destructor
	Object3D();
	virtual ~Object3D();

	// Init / Release / Update
	virtual void      init        			( const std::string& meshName = "", const std::string& materialName = "", Ogre::SceneNode* parent = NULL );
	virtual void      end         			();

	// Query methods
	bool				isValid     		() const { return m_bIsValid; }
	Ogre::Entity*		getEntity			() { return m_entity;	}
	const std::string	getName				() { return m_objectName;}
	Object3DType		getType				() { return m_type;		}
	const Vector&		getPosition			();
	const Quaternion&	getOrientation		() const;
	const Vector&		getScale			() const;

	// Set methods
	void				setType				( Object3DType type ) { m_type = type; }

	void              	setPosition 		( float x, float y, float z );
	void              	setPosition 		( float x, float y );
	void              	setPosition 		( const Vector& pos );

	void				translate			( float x, float y );
	void				translate			( float x, float y, float z );

	void				setOrientation		( const Quaternion& orientation );
	void				setOrientation		( const Vector& axis, float angleDegrees );

	void				rotate				( float angleRad );
	void				rotateX				( float angleRad );
	void				rotateY				( float angleRad );
	void				rotateZ				( float angleRad );
	void				rotate				( const Vector& axis, float angleRad );
	void				rotate				( const Quaternion& quat );

	void              	setScale    		( float xScale, float yScale, float zScale );
	void              	setScale    		( float xScale, float yScale );
	void              	setScale    		( float scale );
	void              	setScale    		( const Vector& scale );

	void				lookAt 				( Object3D& objectToTrack );

	void				setAmbientColor		( const Color& color );
	void				setAmbientColor		( const Color& color, float alpha );
	void				setAmbientColor		( float gray );
	void				setAmbientColor		( float gray, float alpha );
	void				setAmbientColor		( float value1, float value2, float value3 );
	void				setAmbientColor		( float value1, float value2, float value3, float alpha );

	void				setDiffuseColor		( const Color& color );
	void				setDiffuseColor		( const Color& color, float alpha );
	void				setDiffuseColor		( float gray );
	void				setDiffuseColor		( float gray, float alpha );
	void				setDiffuseColor		( float value1, float value2, float value3 );
	void				setDiffuseColor		( float value1, float value2, float value3, float alpha );

	void				setSpecularColor	( const Color& color );
	void				setSpecularColor	( const Color& color, float alpha );
	void				setSpecularColor	( float gray );
	void				setSpecularColor	( float gray, float alpha );
	void				setSpecularColor	( float value1, float value2, float value3 );
	void				setSpecularColor	( float value1, float value2, float value3, float alpha );

	void				setSelfIlluminationColor	( const Color& color );
	void				setSelfIlluminationColor	( const Color& color, float alpha );
	void				setSelfIlluminationColor	( float gray );
	void				setSelfIlluminationColor	( float gray, float alpha );
	void				setSelfIlluminationColor	( float value1, float value2, float value3 );
	void				setSelfIlluminationColor	( float value1, float value2, float value3, float alpha );

	void				setTexture					( const std::string& textureFileName );
	void				setMaterial					( const std::string& materialName );

	void				setVisible					( bool visible );
	bool				isVisible					() const;

	void				setRenderQueueGroup			( int queue );

	// Shadows
	void				castShadows				( bool cast );

	// Materials
	Ogre::MaterialPtr	getMaterial				();

	// Debug methods
	void				showBoundingBox			( bool show );

	// Public Const static attributes
	static const float OGRE_SCALE_CORRECTION; ///< Scale applied to all primitive objects in order to correct ogre scale bug with lighting
																						///< So, for example, in order to have a 1m diameter sphere, we have to create a 100m diameter sphere in maya
																						///< and then divide by 100 the scale in ogre (to have lighting working properly).
private:

	// Private methods
	void duplicateMaterial();

	// Constant / static attributes
	static long				m_objectCounter;///< Used to generate unique names for the 3d objects

	// Attributes
	Ogre::Entity*    		m_entity;				///< Ogre entity that represents the model
	Ogre::MaterialPtr		m_materialCopy;	///< Entity's material copy (to allow this object to have a specific color)
	std::string       		m_objectName;		///< Unique object name
	Object3DType			m_type;					///< Type of 3d object
	bool              		m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _Object3D_H_
