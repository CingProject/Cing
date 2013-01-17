///*
//  This source file is part of the Cing project
//  For the latest info, see http://www.cing.cc
//
//  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//*/
//
//// Precompiled headers
#include "Cing-Precompiled.h"
//
//#include "PhysicsUserAPI.h"
//#include "PhysicsManager.h"
//
//#include "OgreBullet/Dynamics/include/OgreBulletDynamicsWorld.h"
//#include "OgreBullet/Collisions/include/Utils/OgreBulletConverter.h"
//
//
//namespace Cing
//{
//
///*
// * @brief Enables/Disables physics on the scene
// *
// * @param enable if true physics will be enabled (but objects need to have it enabled too), otherwise it will be disabled.
// */
//void enablePhysics( bool enable /*= true*/ )
//{
//	PhysicsManager::getSingleton().enable( enable );
//}
//
///// Disables the physics. Equivalent to enablePhysics( false );
//void disablePhysics() 
//{ 
//	enablePhysics( false ); 
//}
//
///// Sets physcis simulation gravity (only vertical axis)
//void setGravity( float y )
//{
//	setGravity( 0, y, 0 );
//}
//
///// Sets physcis simulation gravity (any axis)
//void setGravity( float x, float y, float z )
//{
//	PhysicsManager::getSingleton().setGravity( Vector(x, y, z) );
//}
//
//
///*
// * @brief Controls if the physic data (collision volumes and contact points) should be drawn or not
// *
// * @param draw if true, physics data will be drawn. Pass falseo to deactivate physics data rendering
// */
//void drawPhysics( bool draw )
//{
//	PhysicsManager::getSingleton().drawPhysics( draw );
//}
//
//} // namespace Cing
// 