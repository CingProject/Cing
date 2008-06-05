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

#ifndef _Vision_H_
#define _Vision_H_


/**
 * @internal
 * @file This is the entry include file from applications
 * It should include all the necessary files for the vision library, so the users don't need to 
 * worry about including files or namespaces
 */

#include <cstdio>
#include <cstring>
#include <stdlib.h>

// Graphics
#include "graphics/Object3D.h"
#include "graphics/Box.h"
#include "graphics/Sphere.h"
#include "graphics/PointLight.h"
#include "graphics/GraphicsTypes.h"
#include "graphics/Image.h"
#include "graphics/GraphicsUserAPI.h"
#include "graphics/LightingUserAPI.h"

// CameraInput
#include "cameraInput/OCVCamera.h"
#include "cameraInput/PVCamera.h"
#include "cameraInput/VICamera.h"

// Framework 
#include "framework/AppMain.h"
#include "framework/AppFramework.h"
#include "framework/UserAppGlobals.h"

// Common
#include "common/MathUtils.h"

// Computer Vision
#include "computervision/CompVision.h"

// namespaces (the library user don't need to know about them...)
using namespace Framework;
using namespace CameraInput;
using namespace ComputerVision;
using namespace Graphics;
using namespace Globals;
using namespace Common;

#endif // _Vision_H_
