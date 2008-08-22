/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
 
Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html
 
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
 
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/

#ifndef __GLXCONFIGDIALOG_H__
#define __GLXCONFIGDIALOG_H__

#include "OgrePrerequisites.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"

namespace Ogre
{
	/** Defines the behaviour of an automatic renderer configuration dialog.
	@remarks
		OGRE comes with it's own renderer configuration dialog, which
		applications can use to easily allow the user to configure the
		settings appropriate to their machine. This class defines the
		interface to this standard dialog. Because dialogs are inherently
		tied to a particular platform's windowing system, there will be a
		different subclass for each platform.
	@author
		Steven J. Streeting
	*/
	class _OgreExport ConfigDialog
	{
	public:
		ConfigDialog();

		/** Displays the dialog.
		@remarks
			This method displays the dialog and from then on the dialog
			interacts with the user independently. The dialog will be
			calling the relevant OGRE rendering systems to query them for
			options and to set the options the user selects. The method
			returns when the user closes the dialog.
		@returns
			If the user accepted the dialog, <b>true</b> is returned.
		@par
			If the user cancelled the dialog (indicating the application
			should probably terminate), <b>false</b> is returned.
		@see
			RenderSystem
		*/
		bool display();

	protected:
		RenderSystem* mSelectedRenderSystem;
	};
}
#endif
