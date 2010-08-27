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

#ifndef _PhysicsObject_h_
#define _PhysicsObject_h_

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
	void			applyCentralForce		( const Vector& force );
	void			applyCentralImpulse		( const Vector& impulse );
	void			applyForce				( const Vector& force, const Vector& relPos );
	void			applyImpulse			( const Vector& impulse, const Vector& relPos );
	void			applyTorque				( const Vector& torque );
	void			applyTorqueImpulse		( const Vector& torque );

	// Advanced physics control
	btRigidBody*	getBulletRigidBody	();

	// Query  Methods
	bool			isValid				() { return Object3D::isValid(); }

protected:

	// Protected methods
	void	enableRigidBodyPhysics( OgreBulletCollisions::CollisionShape* collisionShape, bool staticObject );

	// Protected attributes
	OgreBulletDynamics::RigidBody*	m_rigidBody;				///< Physics rigid body that will control and define the physics of this object
	bool														m_physicsEnabled;		///< Indicates whether the physics is activated for this object or not

private:

	// Attributes

};

} // namespace Cing

#endif // _PhysicsObject_h_