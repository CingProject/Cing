/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2010 Julio Obelleiro and Jorge Cano

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

#ifndef _SceneGraphElement_H_
#define _SceneGraphElement_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"


namespace Cing
{

/**
 * Base class for all elements that are part of the Scene Graph. This is, object that are attached to a SceneNode
 */
class SceneGraphElement
{
public:

	// Constructor / Destructor
	SceneGraphElement(): m_sceneNode(NULL) {}
	virtual ~SceneGraphElement();

	// Scene Node related methods
	void				addChild			( SceneGraphElement& child );	
	void				changeParentNode	( Ogre::SceneNode* parent );
	Ogre::SceneNode*	getSceneNode		() { return m_sceneNode; }

protected:

	// Attributes
	Ogre::SceneNode*	m_sceneNode;  ///< Scene node where the object will be attached
};
} // namespace Cing

#endif // _BaseLight_H_
