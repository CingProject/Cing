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

#ifndef _DebugOverlay_H_
#define _DebugOverlay_H_

#include "GraphicsPrereqs.h"

namespace Cing
{

/**
 * @internal
 * @brief Class that controls the debug overlay.
 * It can be used to show system debug information
 */
class DebugOverlay
{
public:

	// Constructor / Destructor
	 DebugOverlay();
	~DebugOverlay();

	// Init / Release / Update
	bool  init   ();
	void  end    ();
	void  update ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }

	// Const static attributes
	static const std::string DEFAULT_DEBUG_PANEL_NAME;    ///< Default name of the debug panel
	static const std::string DEFAULT_DEBUG_TEXTAREA_NAME; ///< Default name of the debug text area that is inside the debug panel

private:

	// Attributes
	bool  m_bIsValid;	///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _DebugOverlay_H_
