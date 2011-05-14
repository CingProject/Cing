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

#ifndef _Cing_GUIPrereqs_H_
#define _Cing_GUIPrereqs_H_

// Precompiled headers
#include "Cing-Precompiled.h"

/**
 * Prereqs File: This header file contains forward class declarations of the
 * GUI namespace to speed up compiling.
 */

/// Contains the GUI management of the library
namespace CEGUI
{
	class OgreRenderer;
	class System;
	class Listbox;
}

namespace Ogre
{
	class RenderWindow;
	class SceneManager;
}

namespace QuickGUI
{
  class Root;
  class GUIManager;
  class Sheet;
  class Label;
}


#endif // _GUIPrereqs_H_
