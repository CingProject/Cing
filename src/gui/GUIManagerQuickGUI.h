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

#ifndef _GUIManagerQuickGUI_H_
#define _GUIManagerQuickGUI_H_

#include "GUIPrereqs.h"

// Common
#include "common/Singleton.h"

namespace GUI
{

/**
 * @internal
 * 
 */
class GUIManagerQuickGUI: public Common::SingletonStatic< GUIManagerQuickGUI >
{
public:

	// Required for singleton
	friend class Common::SingletonStatic< GUIManagerQuickGUI >;

	// Destructor
	virtual ~GUIManagerQuickGUI	();

	// Init / Release / Update
  void  preInit       ();
	bool  init          ();
	void  end           ();
	void  update        ();

	// Query methods
	bool  isValid       () const { return m_bIsValid; }
	
  // Debug text method
  void  showDebugText ();
  void  hideDebugText ();
  void  setDebugText ( const char* text );

private:

	// private constructor to ensure singleton
	GUIManagerQuickGUI();

	// Attributes
  QuickGUI::Root*       m_quickGuiRoot;     ///< Main Quick gui object
  QuickGUI::GUIManager* m_quickGUIManagerQuickGUI;  ///< Quick gui manager -> used as Quick gui interface
  QuickGUI::Sheet*      m_defaultSheet;     ///< Default sheet. Used to display object in the screen (is a transparent surface to place ui objects)
  QuickGUI::Label*      m_debugLabelText; ///< Label to place text
	bool                  m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace GUI

#endif // _GUIManagerQuickGUI_H_
