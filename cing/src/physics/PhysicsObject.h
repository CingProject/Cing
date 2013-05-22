/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#ifndef _PhysicsObject_h_
#define _PhysicsObject_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "PhysicsPrereqs.h"

// Graphics
#include "graphics/Object3D.h"

namespace Cing
{

/**
 * @internal
 * @brief 3D object with physics associated
 * This physics object creates a triangle based physic mesh, so if the 3d object is a simple primitive
 * such as a box or a sphere, specialized classes should be used to get better performance (like PhysicsBox...)
 */
class PhysicsObject: public Object3D
{
public:

	// Constructor / Destructor
	PhysicsObject					();
	virtual ~PhysicsObject();

	// Init / Release / Update
	void	end			();	

	// Physics control
	virtual void	enablePhysics			( bool staticObject = false );
	void			disablePhysics			();

	// Velocities and accelerations
	void			setMass					( float mass );
	void			setGravity				( const Vector& gravity );
	void			setLinearVelocity		( const Vector& velocity );
	void			setAngularVelocity		( const Vector& angVelocity );

	// Forces and Impulses
	void			applyCentralForce		( const Vector& force );
	void			applyCentralImpulse		( const Vector& impulse );
	void			applyForce				( const Vector& force, const Vector& relPos );
	void			applyImpulse			( const Vector& impulse, const Vector& relPos );
	void			applyTorque				( const Vector& torque );
	void			applyTorqueImpulse		( const Vector& torque );

	// Advanced physics control
	btRigidBody*	getBulletRigidBody		();

	// Query  Methods
	bool			isValid					() { return Object3D::isValid(); }

protected:

	// Protected methods
	void	enableRigidBodyPhysics( OgreBulletCollisions::CollisionShape* collisionShape, bool staticObject );

	// Protected attributes
	OgreBulletDynamics::RigidBody*	m_rigidBody;			///< Physics rigid body that will control and define the physics of this object
	bool							m_physicsEnabled;		///< Indicates whether the physics is activated for this object or not

private:

	// Attributes

};

} // namespace Cing

#endif // _PhysicsObject_h_