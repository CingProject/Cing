/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

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

#ifndef _LightingUserAPI_h_
#define _LightingUserAPI_h_

#include "GraphicsPrereqs.h"

namespace Graphics
{

/**
 * @brief Lighting related functions that user can call directly
 */

// Ambient lighting
void ambientLight( float gray );
void ambientLight( float red, float green, float blue );


} // namespace Graphics

#endif // _LightingUserAPI_h_
