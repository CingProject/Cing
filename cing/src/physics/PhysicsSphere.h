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

#ifndef _PhysicsSphere_h_
#define _PhysicsSphere_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "PhysicsPrereqs.h"
#include "PhysicsObject.h"

namespace Cing
{

/**
 * @internal
 * @brief Represents a 3D sphere with physics capabilities. It can have an associated texture to draw it.
 */
	class PhysicsSphere: public PhysicsObject
{
public:

	// Constructor / Destructor
	PhysicsSphere();
	virtual ~PhysicsSphere();

	// Init / Release
	void	init 			( float diameter );

	// Query  Methods
	bool	isValid			() { return m_bIsValid; }
	float	getRadius		() { return m_radius;		}
	float	getDiameter	() { return m_radius * 2;		}

	// Physics control
	virtual void enablePhysics		( bool staticObject = false );

private:

	// Constant attributes
	static const std::string  DEFAULT_MESH;			///< Name of the default mesh used by this object
	static const std::string  DEFAULT_MATERIAL; ///< Name of the default material used by this object

	// Attributes
	float		m_radius;			///< Radius of the PhysicsSphere
	bool		m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _PhysicsSphere_h_