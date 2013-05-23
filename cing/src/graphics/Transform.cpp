/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

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
*/

// Precompiled headers
#include "Cing-Precompiled.h"

#include "Transform.h"
#include "GraphicsManager.h"

#include "OgreSceneManager.h"

#include "common/MathUtils.h"
#include "common/CommonUserApi.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Transform::Transform() :	m_bIsValid( false )
{
	init();
};


/**
* @internal
* @brief Initializes the class so it becomes valid.
*
* @return true if the initialization was ok | false otherwise
*/
bool Transform::init()
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init values
	m_4x4 = Ogre::Matrix4::IDENTITY;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Transform::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @brief 
 *
 * @param mode
 */
void Transform::translate(  float x, float y, float z )
{
 	// Apply transform
	m_4x4 = m_4x4.concatenate( Ogre::Matrix4::getTrans(x,y,z ) );
}

/**
 * @brief 
 *
 * @param mode
 */
void Transform::rotate(  float x, float y, float z )
{
	// Make temporal transform
	Ogre::Matrix3 m3Temp;
	m3Temp.FromEulerAnglesXYZ( Ogre::Radian(x), Ogre::Radian(y), Ogre::Radian(z) );
	Ogre::Matrix4 tMat = Ogre::Matrix4( m3Temp );

	// Apply transform
	m_4x4 = m_4x4.concatenate(tMat);
}

//// Rotates around the X axis. Angle in Radians
void Transform::rotateX( float angleRad )
{
	Matrix3 rotation(Matrix3::IDENTITY);
	rotation.FromAxisAngle(Vector(1,0,0), Ogre::Radian(angleRad) );
	
	// Apply transform
	//m_4x4 = m_4x4 * rotation;
	m_4x4 = m_4x4 * Matrix4(rotation);
}

//// Rotates around the Y axis. Angle in Radians
void Transform::rotateY( float angleRad )
{
	Matrix3 rotation(Matrix3::IDENTITY);
	rotation.FromAxisAngle(Vector(0,1,0), Ogre::Radian(angleRad) );
	
	// Apply transform
	//m_4x4 = m_4x4 * rotation;
	m_4x4 = m_4x4 * Matrix4(rotation);
}

//// Rotates around the Z axis. Angle in Radians
void Transform::rotateZ( float angleRad )
{
	Matrix3 rotation(Matrix3::IDENTITY);
	rotation.FromAxisAngle(Vector(0,0,-1), Ogre::Radian(angleRad) );
	
	// Apply transform
	//m_4x4 = m_4x4 * rotation;
	m_4x4 = m_4x4 * Matrix4(rotation);
}


/**
 * @brief 
 *
 * @param mode
 */
void Transform::scale(  float x, float y, float z )
{
	// Apply transform
	// TODO: Check this. When we apply the scale, the result of the rotation
	// value in the matrix changes... Check this with Processing (documented in issue list)
	//Quaternion qbefore = m_4x4.extractQuaternion();
	m_4x4 = m_4x4.concatenate(Ogre::Matrix4::getScale( x, y, z ));
	//Quaternion qafter = m_4x4.extractQuaternion();
}

/**
 * @brief Returns the translation Vector
 *
 * @param mode
 */
Vector Transform::getPosition	() 
{
	return Vector( m_4x4[0][3], m_4x4[1][3], m_4x4[2][3] );
}
/**
 * @brief Returns the rotation Vector
 *
 * @param mode
 */
Vector Transform::getRotation	()
{
	Ogre::Quaternion rot = m_4x4.extractQuaternion();
	return Vector(	rot.getYaw().valueRadians(), rot.getPitch().valueRadians(),	rot.getRoll().valueRadians() );
}

/**
 * @brief Returns the rotation Quaternion
 *
 * @param mode
 */
Quaternion Transform::getRotQuaternion()
{
	return m_4x4.extractQuaternion();
}

/**
 * @brief Returns the scale Vector
 *
 * @param mode
 */
Vector Transform::getScale()
{
	Vector  tempV;
	float xS, yS, zS = 0.0f;
	tempV = Vector( m_4x4[0][0], m_4x4[0][1], m_4x4[0][2] );	 xS = tempV.length();
	tempV = Vector( m_4x4[1][0], m_4x4[1][1], m_4x4[1][2] );	 yS = tempV.length();
	tempV = Vector( m_4x4[2][0], m_4x4[2][1], m_4x4[2][2] );	 zS = tempV.length();
	return Vector(xS, yS, zS);
}
/**
 * @brief Apply this transform to an input Vector and returns the transformed Vector
 *				TODO: Optimize.
 *
 * @param mode
 */
Vector Transform::applyTransform( const Vector& input )
{
	return m_4x4*input;
};
/**
 * @brief 
 *
 * @param mode
 */
void Transform::identity()
{
	m_4x4 = Ogre::Matrix4::IDENTITY;
};
/**
 * @brief 
 *
 * @param mode
 */
void Transform::printMatrix()
{
	for ( int i = 0; i<4; i++ )
	{
		for ( int j = 0; j<4; j++ )
		{
			print("%f ", m_4x4[i][j]);
		}
		println("");
	}
};

/**
 * @brief 
 *
 * @param mode
 */
void Transform::applyMatrix( float m00, float m01, float m02, float m03,
							 float m10, float m11, float m12, float m13,
							 float m20, float m21, float m22, float m23,
							 float m30, float m31, float m32, float m33 )
{
	Ogre::Matrix4 tMat = Ogre::Matrix4( m00, m01, m02, m03,
										m10, m11, m12, m13,
										m20, m21, m22, m23,
										m30, m31, m32, m33);

	m_4x4 = m_4x4 * tMat;
};
} // namespace Cing
