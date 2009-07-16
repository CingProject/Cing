/*
This source file is part of the Cing project
For the latest info, see http://www.XXX.org

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

#ifndef _ShapeManager_H_
#define _ShapeManager_H_

// Common
#include "common/Singleton.h"

// Graphics
#include "GraphicsPrereqs.h"
#include "Color.h"
#include "Style.h"
#include "Shape.h"

namespace Cing
{

	/**
	* @internal
	* 
	*/
	class ShapeManager: public SingletonStatic< ShapeManager >
	{
	public:
		// Required for singleton
		friend class SingletonStatic< ShapeManager >;

		// Constructor / Destructor
		ShapeManager								();
		virtual ~ShapeManager				();

		bool init										();
		void update									();
		void end										();

		// Query methods
		bool isValid								() const  { return m_bIsValid; } 

		// Simple primitives 3d related drawing ( lines, circles , ...)
		void line				( float x1, float y1, float z1, float x2, float y2, float z2 );
		void line				( float x1, float y1, float z1, float x2, float y2, float z2, Color theColor );

		void beginShape	();
		void beginShape	(GraphicsType operation);
		void endShape		();
		void endShape		(GraphicsType operation);
		void vertex			(float x, float y);
		void vertex			(float x, float y, float z);

	private:

		Shape						m_lines;				///< It contains all line data, including borders of shapes
		std::vector <Shape>		m_shapes;				///< It contains all shape data ( it will be filled polygons later... )
		Ogre::ManualObject*					m_shapesManualObject;	///< The shapes and lines are drawn using the same Ogre::ManualObject
		Ogre::SceneNode*					m_shapesSceneNode;		///< The ManualOnject is attached to this Ogre::SceneNode
		bool								m_isShapeClosed;		///< Temp
		bool								m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
	};

} // namespace Cing

#endif // _ShapeManager_H_
