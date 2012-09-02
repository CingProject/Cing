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

#ifndef _Cing_Plugin_H
#define _Cing_Plugin_H

namespace Cing
{

// Plugin potential init times (sorted in order of appearance)
enum PluginInitTime
{
	NO_INIT,
	INIT_BEFORE_GRAPHICS,		///< The plugin init will be called before initializing the graphics core (the log system is ready at this point)
	INIT_BEFORE_RESOURCE_INIT,	///< The plugin init will be called when the app window and the core graphics are initialized, but the resources have not been loaded. This allow to do stuff pre-resource loading.
	INIT_AFTER_GRAPHICS,		///< At this point the Graphics are initialized, but no other subsystem is (input, gui, etc)
	INIT_AFTER_SUBSYSTEMS		///< The plugin init will be called right after the graphics and other subsystems (input, gui) have been initialized, but before the draw cycle starts

};

// Plugin potential end times (sorted in order of appearance), for resource release
enum PluginEndTime
{
	NO_END,
	END_BEFORE_USER,	///< Plugin end is called before the user's end() function is called 
	END_AFTER_USER,		///< Plugin end is called right after user's end() function is called (which happens right before the core subsystems end() call)
	END_AFTER_CORE		///< Plugin end is called after all the core subsystems have been released
};

// Plugin potential update times (sorted in order of appearance), for plugin update/draw per frame tasks
enum PluginUpdateTime
{
	NO_UPDATE,
	UPDATE_BEFORE_USER,	///< Plugin update is called every frame before the user's draw function is called
	UPDATE_AFTER_USER,	///< Plugin update is called every frame right after the user's draw function is called, but before the scene is rendered
	UPDATE_AFTER_RENDER	///< Plugin update will be called after the scene is rendered, at the end of the draw/update loop
};

/**
 * Base class to create an plugins for Cing (aka Cing-Ings).
 */
class Plugin
{
public:
	Plugin( const std::string& pluginName ): m_initTime(NO_INIT), m_endTime(NO_END), m_updateTime(NO_UPDATE), m_pluginName(pluginName) {}

	virtual bool		init	()	{ return true; };	///< Override this method if you want your pluing init to be called. To define the call time, use setPluginInitTime method
	virtual void		end		()	{};					///< Override this method if you want your pluing init to be called. To define the call time, use setPluginInitTime method
	virtual bool		update	()	{ return true; };	///< Override this method if you want the plugin to have its update method called every frame

	// Getters
	const std::string&	getName	()	const { return m_pluginName; }

	// Plugin method call times
	void				setPluginInitTime	( PluginInitTime time)	{ m_initTime = time; }
	void				setPluginEndTime	( PluginEndTime time)	{ m_endTime = time; }
	void				setPluginUpdateTime	( PluginUpdateTime time){ m_updateTime = time; }
	PluginInitTime		getPluginInitTime	() const				{ return m_initTime; }
	PluginEndTime		getPluginEndTime	() const				{ return m_endTime; }
	PluginUpdateTime	getPluginUpdateTime	() const				{ return m_updateTime; }

private:
	PluginInitTime		m_initTime;		///< Moment when the plugin init() method should be called by the app framework
	PluginEndTime		m_endTime;		///< Moment when the plugin end() method should be called by the app framework
	PluginUpdateTime	m_updateTime;	///< Moment when the plugin update() method should be called by the app framework
	std::string			m_pluginName;	///< Name of the plugin (can be used to retrieve the plugin from anywhere in the code)
};

} // namespace

#endif