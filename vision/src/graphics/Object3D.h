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
	bool              isValid     			() const { return m_bIsValid; }
	Ogre::SceneNode*  getSceneNode			() { return m_sceneNode;			}
	Ogre::Entity*			getEntity					() { return m_entity;					}
	const std::string getName						() { return m_objectName;			}
	Object3DType			getType						() { return m_type;						}
	const Vector&			getPosition				();
	const Quaternion&	getOrientation		() const;

	// Set methods
	void							setType						( Object3DType type ) { m_type = type; }

	void              setPosition 			( float x, float y, float z );
	void              setPosition 			( float x, float y );  
	void              setPosition 			( const Vector& pos );
	
	void							setOrientation		( const Quaternion& orientation );
	void							setOrientation		( const Vector& axis, float angle );

	void							rotate						( const Vector& axis, float angle );
	void							rotate						( const Quaternion& quat );

	void              setScale    			( float xScale, float yScale, float zScale );
	void              setScale    			( float xScale, float yScale );
	void              setScale    			( float scale );
	void              setScale    			( const Vector& scale );

	void              lookAt 						( Object3D& objectToTrack );

	void							setAmbientColor		( const Color& color );
	void							setAmbientColor		( const Color& color, int alpha );
	void							setAmbientColor		( int gray );
	void							setAmbientColor		( int gray, int alpha );
	void							setAmbientColor		( int value1, int value2, int value3 );
	void							setAmbientColor		( int value1, int value2, int value3, int alpha );

	void							setDiffuseColor		( const Color& color );
	void							setDiffuseColor		( const Color& color, int alpha );
	void							setDiffuseColor		( int gray );
	void							setDiffuseColor		( int gray, int alpha );
	void							setDiffuseColor		( int value1, int value2, int value3 );
	void							setDiffuseColor		( int value1, int value2, int value3, int alpha );

	void							setSpecularColor	( const Color& color );
	void							setSpecularColor	( const Color& color, int alpha );
	void							setSpecularColor	( int gray );
	void							setSpecularColor	( int gray, int alpha );
	void							setSpecularColor	( int value1, int value2, int value3 );
	void							setSpecularColor	( int value1, int value2, int value3, int alpha );

	void							setSelfIlluminationColor	( const Color& color );
	void							setSelfIlluminationColor	( const Color& color, int alpha );
	void							setSelfIlluminationColor	( int gray );
	void							setSelfIlluminationColor	( int gray, int alpha );
	void							setSelfIlluminationColor	( int value1, int value2, int value3 );
	void							setSelfIlluminationColor	( int value1, int value2, int value3, int alpha );

	void							setTexture								( const std::string& textureFileName );
	void							setMaterial								( const std::string& materialName );

	void							setVisible								( bool visible );

	// Debug methods
	void							showBoundingBox						( bool show );


	// Public Const static attributes
	static const float OGRE_SCALE_CORRECTION; ///< Scale applied to all primitive objects in order to correct ogre scale bug with lighting
																						///< So, for example, in order to have a 1m diameter sphere, we have to create a 100m diameter sphere in maya
																						///< and then divide by 100 the scale in ogre (to have lighting working properly).
private:

	// Private methods
	void duplicateMaterial();

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
