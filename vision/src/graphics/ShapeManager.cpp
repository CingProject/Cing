/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

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

#include "ShapeManager.h"
#include "GraphicsManager.h"
#include "Ogre3d/include/OgreSceneManager.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
ShapeManager::ShapeManager(): m_bIsValid ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
ShapeManager::~ShapeManager()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 */
bool ShapeManager::init()
{
	// Init line data
	m_lines = Shape(LINES);

	// Init manual objects to allow 3d simple primitives drawing (lines...)
	int estimatedVertexCount = 10000;

	m_shapesManualObject = GraphicsManager::getSingleton().getSceneManager().createManualObject("Graphics::Shapes");
	m_shapesManualObject->setDynamic(true);
	m_shapesManualObject->estimateVertexCount( estimatedVertexCount );

	// Attach m_polys sceneNode
	GraphicsManager::getSingleton().getSceneManager().getRootSceneNode()->createChildSceneNode()->attachObject(m_shapesManualObject);
	//m_isShapeClosed = true;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Release the class resoruces
 */
void ShapeManager::update()
{
	// Clear old geometry
	m_shapesManualObject->clear();


	// TODO: Si hay que dibujar el mismo número de vértices que en el fotograma anterior,
	// usar beginUpdate() en lugar de begin()
	// Draw shapes
	for (unsigned int i = 0; i < m_shapes.size() ; i++)
	{
		Ogre::RenderOperation renderOp;
		switch(	m_shapes[i].m_renderOp )
		{
		case POINTS:
			renderOp.operationType = Ogre::RenderOperation::OT_POINT_LIST;
			break;
		case LINES:
			renderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;
			break;
		case TRIANGLES:
			renderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
			break;
		case TRIANGLE_FAN:
			renderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_FAN;
			break;
		case TRIANGLE_STRIP:
			renderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
			break;
		case QUADS:
			// Not implemented yet
			break;
		case QUAD_STRIP:
			// Not implemented yet
			break;
		default:
			break;
		}

		// Draw filled regions
		if (m_shapes[i].isFill())
		{
			m_shapesManualObject->begin("VertexColorMaterial", renderOp.operationType);
			for (int j=0; j<(int)m_shapes[i].m_vertexData.size(); j++)
			{
				m_shapesManualObject->position(	m_shapes[i].m_vertexData[j]);
				m_shapesManualObject->colour(		m_shapes[i].m_FillColor.r/255.0f,
																				m_shapes[i].m_FillColor.g/255.0f,
																				m_shapes[i].m_FillColor.b/255.0f,
																				m_shapes[i].m_FillColor.a/255.0f   );
			}
			m_shapesManualObject->end();

		}else{
			// Point and Lines are special cases, because it must be use strokeColor instead fillColor for drawing
			if (m_shapes[i].m_renderOp == POINTS || m_shapes[i].m_renderOp == LINES)
			{
				m_shapesManualObject->begin("VertexColorMaterial", renderOp.operationType);
				for (int j=0; j<(int)m_shapes[i].m_vertexData.size(); j++)
				{
					m_shapesManualObject->position(	m_shapes[i].m_vertexData[j]);
					m_shapesManualObject->colour(		m_shapes[i].m_FillColor.r/255.0f,
						m_shapes[i].m_FillColor.g/255.0f,
						m_shapes[i].m_FillColor.b/255.0f,
						m_shapes[i].m_FillColor.a/255.0f   );
				}
				m_shapesManualObject->end();
			}

		}

		// Draw borders
		if (m_shapes[i].isStroke())
		{
			m_shapesManualObject->begin("VertexColorMaterial", Ogre::RenderOperation::OT_LINE_LIST);
			for (int j=0; j<(int)m_shapes[i].m_vertexData.size()-1; j++)
			{
				m_shapesManualObject->position(	m_shapes[i].m_vertexData[j] );
				m_shapesManualObject->colour(		m_shapes[i].m_StrokeColor.r/255.0f,
																				m_shapes[i].m_StrokeColor.g/255.0f,
																				m_shapes[i].m_StrokeColor.b/255.0f,
																				m_shapes[i].m_StrokeColor.a/255.0f   );
				m_shapesManualObject->position(	m_shapes[i].m_vertexData[j+1] );
				m_shapesManualObject->colour(		m_shapes[i].m_StrokeColor.r/255.0f,
																				m_shapes[i].m_StrokeColor.g/255.0f,
																				m_shapes[i].m_StrokeColor.b/255.0f,
																				m_shapes[i].m_StrokeColor.a/255.0f   );
			}
			m_shapesManualObject->end();
		}

		// Clear the shape
		m_shapes[i].clear();

	}

	// Draw lines
	m_shapesManualObject->begin("VertexColorMaterial", Ogre::RenderOperation::OT_LINE_LIST);
	for (int i=0; i<(int)m_lines.getVertexCount(); i++)
	{
		m_shapesManualObject->position(	m_lines.m_vertexData[i] );
		m_shapesManualObject->colour  (	m_lines.m_vertexColors[i].r/255.0f,
																		m_lines.m_vertexColors[i].g/255.0f,
																		m_lines.m_vertexColors[i].b/255.0f,
																		m_lines.m_vertexColors[i].a/255.0f);
	}
	m_shapesManualObject->end();

	// Clear the shapes ( vertex position and vertex colors )
	m_shapes.clear();
	m_lines.clear();
}

/**
 * @internal
 * @brief Release the class resoruces
 */
void ShapeManager::end()
{
	m_bIsValid = false;
}

/**
 * @internal
 * @brief
 */
void ShapeManager::line( float x1, float y1, float z1, float x2, float y2, float z2)
{
	m_lines.addVertex(x1, y1, z1, GraphicsManager::getSingleton().getStrokeColor() );
	m_lines.addVertex(x2, y2, z2, GraphicsManager::getSingleton().getStrokeColor() );
};
/**
 * @internal
 * @brief
 */
void ShapeManager::line(  float x1, float y1, float z1, float x2, float y2, float z2, Color theColor )
{
	m_lines.addVertex(x1, y1, z1, theColor);
	m_lines.addVertex(x2, y2, z2, theColor);
};

/**
 * @internal
 * @brief
 */
void ShapeManager::beginShape()
{
	// Add a new shape
	Shape newShape = Shape( Style(GraphicsManager::getSingleton().getFillColor(), GraphicsManager::getSingleton().getStrokeColor(), GraphicsManager::getSingleton().getStrokeWeight()), TRIANGLE_FAN );

	// Check if the user select fill and/or stroke
	newShape.m_isFill   = GraphicsManager::getSingleton().getFill();
	newShape.m_isStroke = GraphicsManager::getSingleton().getStroke();
	newShape.m_isClose  = false;
	newShape.m_StrokeColor = GraphicsManager::getSingleton().getStrokeColor();
	newShape.m_FillColor = GraphicsManager::getSingleton().getFillColor();

	// Add it to the container
	m_shapes.push_back(newShape);
};

/**
 * @internal
 * @brief
 */
void ShapeManager::beginShape(GraphicsType operation)
{
	// Create a new shape
	Shape newShape = Shape( operation );

	// Check if the user select fill and/or stroke
	newShape.m_isFill      = GraphicsManager::getSingleton().getFill();
  newShape.m_isStroke    = GraphicsManager::getSingleton().getStroke();
	newShape.m_isClose     = false;
	newShape.m_StrokeColor = GraphicsManager::getSingleton().getStrokeColor();
	newShape.m_FillColor   = GraphicsManager::getSingleton().getFillColor();

	// Check type of the new shape
	switch(	operation )
	{
	case POINTS:
		newShape.m_isStroke = false;
	  newShape.m_FillColor  = GraphicsManager::getSingleton().getStrokeColor();
		break;
	case LINES:
		newShape.m_isStroke = false;
	  newShape.m_FillColor  = GraphicsManager::getSingleton().getStrokeColor();
		break;
	case TRIANGLES:
		break;
	case TRIANGLE_FAN:
		break;
	case TRIANGLE_STRIP:
		break;
	case QUADS:
		// Not implemented yet
		break;
	case QUAD_STRIP:
		// Not implemented yet
		break;
	default:
		break;
	}

	// Add it to the container
	m_shapes.push_back(newShape);
};

/**
 * @internal
 * @brief
 */
void ShapeManager::endShape()
{
	m_shapes.back().m_isClose = true;
};

/**
 * @internal
 * @brief
 */
void ShapeManager::endShape(GraphicsType operation)
{

	if (operation == CLOSE)
	{
		Ogre::Vector3 firstVertex = m_shapes.back().m_vertexData[0];
		m_shapes.back().addVertex( firstVertex.x,firstVertex.y,firstVertex.z,GraphicsManager::getSingleton().getFillColor() );
		//m_lines.addVertex(firstVertex.x,firstVertex.y,firstVertex.z, GraphicsManager::getSingleton().getStrokeColor());
		m_shapes.back().m_isClose = true;
	}

};

/**
 * @internal
 * @brief
 */
void ShapeManager::vertex(float x, float y)
{
	m_shapes.back().addVertex(x,y,GraphicsManager::getSingleton().getFillColor());

	//m_lines.addVertex(x, y, 0, GraphicsManager::getSingleton().getStrokeColor());
};

/**
 * @internal
 * @brief
 */
void ShapeManager::vertex(float x, float y, float z)
{
	m_shapes.back().addVertex(x,y,z, GraphicsManager::getSingleton().getFillColor() );

	//m_lines.addVertex(x, y, z, GraphicsManager::getSingleton().getStrokeColor());
};

} // namespace Graphics
