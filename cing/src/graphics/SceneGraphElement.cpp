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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "SceneGraphElement.h"
#include "GraphicsManager.h"
#include "common/CommonUtilsIncludes.h"

// Ogre
#include "OgreSceneNode.h"

namespace Cing
{

	/// Destructor
	SceneGraphElement::~SceneGraphElement()
	{	
		Ogre::SceneManager* sceneManager = GraphicsManager::getSingleton().getSceneManagerPtr();
		if ( m_sceneNode && sceneManager)
		{
			m_sceneNode->removeAndDestroyAllChildren();
			m_sceneNode->detachAllObjects();
		}
	}

	/**
	 * @brief Adds an scene node as a child of this object's scene node
	 * It allows to create hierachies of objects/scene nodes in a scene
	 * @param child Scene Graph element to be added as a child (the scene nodes will be linked)
	 */
	void SceneGraphElement::addChild( SceneGraphElement& child )
	{
		// Check we have a valid scene node
		if ( !child.getSceneNode() || !m_sceneNode )
		{
			LOG_ERROR( "SceneGraphElement::addChild. The received scene node is not valid, or the current object has not been initialized." );
			return;
		}

		// Remove the scene node from its current parent
		child.getSceneNode()->getParentSceneNode()->removeChild( child.getSceneNode() );

		// Add it as child of this object
		m_sceneNode->addChild( child.getSceneNode() );
	}

	/**
	 * @brief Changes the current parent scene node (detaches from current and attaches to the new node)
	 * @param parent New parent node of this element
	 */
	void SceneGraphElement::changeParentNode( Ogre::SceneNode* parent )
	{
		// Check the parent is valid
		if ( !parent )
		{
			LOG_ERROR( "SceneGraphElement::changeParentNote. The received scene node is NULL, or the current object has not been initialized." );
			return;
		}

		// Check we have a valid scene node
		if ( !m_sceneNode )
		{
			LOG_ERROR( "SceneGraphElement::changeParentNote. This scene node elemnet has not been initialized" );
			return;
		}
	
		// Remove this node from our current parent
		m_sceneNode->getParentSceneNode()->removeChild( m_sceneNode );
		parent->addChild( m_sceneNode );
	}

} // namespace Cing
