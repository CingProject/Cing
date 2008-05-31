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

#ifndef _PhysicsBox_h_
#define _PhysicsBox_h_

#include "PhysicsPrereqs.h"
#include "PhysicsObject.h"

namespace Physics
{

/**
 * @internal
 * @brief Controls the physics associated with a box object
 */
class PhysicsBox: public PhysicsObject
{
public:

	// Constructor / Destructor
	PhysicsBox();
	virtual ~PhysicsBox	();

	// Init / Release / Update
	virtual void	init 	( Graphics::Object3D& object );
	void					end		();	

	// Query  Methods
	bool	isValid				() { return m_bIsValid; }

private:

	// Attributes
	bool	m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Physics

#endif // _PhysicsBox_h_