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

#ifndef _Ribbon_h_
#define _Ribbon_h_

#include "common/CommonTypes.h"


// Forward declarations
namespace Ogre
{
	class SceneNode;
	class ParticleSystem;
}

/**
 * @internal
 * @brief 
 */
class Ribbon
{
public:

	// Constructor / Destructor
	Ribbon( float x, float y, float z, float phase, float length, float duration, const std::vector< Vector >& path );
	~Ribbon();

	// Control 
	bool update			();
	void setPosition( float x, float y, float z );

private:
	static int						index;
	
	
	Ogre::RibbonTrail*		m_ribbon;
	Ogre::SceneNode*			m_ribbonNode;
	Ogre::SceneNode*			m_animNode;
	Ogre::Animation*			m_ribbonAnimation;
	Ogre::AnimationState* m_ribbonAnimState;
	std::vector< Vector >	m_path;
	int										m_targetIndex;
	float									m_timeControl;
	float									m_speed;
	float									m_duration;
	Vector								m_pos, m_prevPos;
	size_t								m_prevNumParticles;
};

#endif // _Ribbon_h_