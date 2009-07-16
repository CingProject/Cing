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

#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_


namespace Cing
{

/**
 * @internal
 * Macro to create and start an application. Defines the main
 * so the user does not have to define it.
 * Example of use:
 * @code
 *  CREATE_APPLICATION( "Application Name" )
 * @endcode
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 && !_CONSOLE

  #include "Windows.h"

  #define CREATE_APPLICATION( appName )                                     \
          INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)   \
          {                                                                       \
              RunApplication( appName  );                                         \
              return 0;                                                           \
          };

#else

#define CREATE_APPLICATION( appName )                                     \
        int main()                                                              \
        {                                                                       \
            RunApplication( appName  );                                         \
            return 0;                                                           \
        };
#endif

} // namespace

#endif // _APP_MAIN_H_

