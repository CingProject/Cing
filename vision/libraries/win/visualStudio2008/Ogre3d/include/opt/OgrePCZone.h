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
PCZone.h  -  Portal Connected Zone (PCZone) header file.
-----------------------------------------------------------------------------
Portal Connected Zones are spatial constructs for partitioning space into cross
connected zones.  Each zone is connected to other zones using Portal nodes.

Zones contain references to nodes which touch them.  However, zones do not
care how nodes are arranged hierarchically.  Whether or not a node is 
referenced as being part of a zone is entirely determined by the user or
by the node crossing a portal into or out-of the zone.

Nodes can be referenced by several zones at once, but only one zone is
considered the "home" zone of the node.  Home zone is determined by location
of the centerpoint of the node.  Nodes can "touch" other zones if the node 
BV intersects a portal (this is also called "visiting" a zone).  Nodes keep
a pointer to their home zone and a list of references to zones they are 
"visiting".
-----------------------------------------------------------------------------
begin                : Tue Feb 20 2007
author               : Eric Cha
email                : ericc@xenopi.com
Code Style Update	 :
-----------------------------------------------------------------------------
*/

#ifndef PCZONE_H
#define PCZONE_H

#include "OgrePCZPrerequisites.h"
#include "OgrePCZSceneNode.h"
#include "OgrePCZCamera.h"
#include "OgrePCZFrustum.h"

namespace Ogre
{

    class PCZone;
    class Portal;
	class PCZSceneNode;
	class PCZSceneManager;
    class PCZLight;

    typedef std::map<String, PCZone*> ZoneMap;
	typedef std::list<PCZone*> PCZoneList;
    typedef std::list < Portal * > PortalList;
	typedef std::list < SceneNode * > NodeList;
	typedef std::set < PCZSceneNode * > PCZSceneNodeList;
    typedef std::map<String, SceneNode*> SceneNodeList;

    /** Portal-Connected Zone datastructure for managing scene nodes.
    @remarks
    */

	class _OgrePCZPluginExport PCZone : public SceneCtlAllocatedObject
    {
    public:

		enum NODE_LIST_TYPE
		{
			HOME_NODE_LIST = 1,
			VISITOR_NODE_LIST = 2
		};

        PCZone( PCZSceneManager *, const String& );
        virtual ~PCZone();

		// get the zone type name (ex: "ZoneType_Terrain")
		const String& getZoneTypeName() const { return mZoneTypeName; }

		// Returns the name of the zone
		const String& getName(void) const { return mName; }

		/* get a pointer to the enclosure node for this PCZone
		*/
		SceneNode * getEnclosureNode(void) {return mEnclosureNode;}

		/** if sky should be drawn with this zone
		*/
		void setHasSky(bool yesno) {mHasSky = yesno;}

		/* return whether or not this zone has sky
		*/
		bool hasSky(void) {return mHasSky;}

		/* set the lastVisibleFrame counter */
		void setLastVisibleFrame(unsigned long frameCount) {mLastVisibleFrame = frameCount;}

        /* get the lastVisibleFrame counter value */
        unsigned long getLastVisibleFrame(void) {return mLastVisibleFrame;}

		/* set the lastVisibleFromCamera pointer */
		void setLastVisibleFromCamera(PCZCamera * camera) {mLastVisibleFromCamera = camera;}

		/* get the lastVisibleFromCamera pointer */
		PCZCamera* getLastVisibleFromCamera() {return mLastVisibleFromCamera;}

	public:
		/** Set the enclosure node for this PCZone
		*/
		virtual void setEnclosureNode(PCZSceneNode *) = 0;

        /** Adds an SceneNode to this PCZone.
        @remarks
        The PCZSceneManager calls this function to add a node
        to the zone.  Home or Visitor list is selected based on the node's home zone
        */
        virtual void _addNode( PCZSceneNode * ) = 0;

        /** Removes all references to a SceneNode from this PCZone.
        */
        virtual void removeNode( PCZSceneNode * ) = 0;

		/** Remove all nodes from the node reference list and clear it
		*/
		virtual void _clearNodeLists(short nodeListTypes);

		/** Indicates whether or not this zone requires zone-specific data for 
		 *  each scene node
		 */
		virtual bool requiresZoneSpecificNodeData(void) = 0;

		/** create zone specific data for a node
		*/
		virtual void createNodeZoneData(PCZSceneNode *);

		/** find a matching portal (for connecting portals)
		*/
		virtual Portal * findMatchingPortal(Portal *);

		/* Add a portal to the zone
		*/
		virtual void _addPortal( Portal * ) = 0;

		/* Remove a portal from the zone
		*/
		virtual void _removePortal( Portal * ) = 0;

		/** (recursive) check the given node against all portals in the zone
		*/
		virtual void _checkNodeAgainstPortals(PCZSceneNode *, Portal * ) = 0;

        /** (recursive) check the given light against all portals in the zone
        */
        virtual void _checkLightAgainstPortals(PCZLight *, 
                                               long, 
                                               PCZFrustum *,
                                               Portal *) = 0;

		/** Update the spatial data for the portals in the zone
		*/
		virtual void updatePortalsSpatially(void) = 0;

		/* Update the zone data for each portal 
		*/
		virtual void updatePortalsZoneData(void) = 0;

		/* Update a node's home zone */
		virtual PCZone * updateNodeHomeZone(PCZSceneNode * pczsn, bool allowBackTouces) = 0;

        /** Find and add visible objects to the render queue.
        @remarks
        Starts with objects in the zone and proceeds through visible portals   
        This is a recursive call (the main call should be to _findVisibleObjects)
        */
        virtual void findVisibleNodes(PCZCamera *, 
									  NodeList & visibleNodeList,
									  RenderQueue * queue,
									  VisibleObjectsBoundsInfo* visibleBounds, 
									  bool onlyShadowCasters,
									  bool displayNodes,
									  bool showBoundingBoxes) = 0;

		/* Functions for finding Nodes that intersect various shapes */
		virtual void _findNodes( const AxisAlignedBox &t, 
						         PCZSceneNodeList &list, 
                                 PortalList &visitedPortals,
						         bool includeVisitors,
						         bool recurseThruPortals,
						         PCZSceneNode *exclude) = 0;
	    virtual void _findNodes( const Sphere &t, 
						         PCZSceneNodeList &list, 
                                 PortalList &visitedPortals,
						         bool includeVisitors,
						         bool recurseThruPortals,
						         PCZSceneNode *exclude ) = 0;
	    virtual void _findNodes( const PlaneBoundedVolume &t, 
						         PCZSceneNodeList &list, 
                                 PortalList &visitedPortals,
						         bool includeVisitors,
						         bool recurseThruPortals,
						         PCZSceneNode *exclude ) = 0;
	    virtual void _findNodes( const Ray &t, 
						         PCZSceneNodeList &list, 
                                 PortalList &visitedPortals,
						         bool includeVisitors,
						         bool recurseThruPortals,
						         PCZSceneNode *exclude ) = 0;

		/** Sets the options for the Zone */
		virtual bool setOption( const String &, const void * ) = 0;
		/** called when the scene manager creates a camera in order to store the first camera created as the primary
			one, for determining error metrics and the 'home' terrain page.
		*/
		virtual void notifyCameraCreated( Camera* c ) = 0;
		/* called by PCZSM during setWorldGeometryRenderQueue() */
		virtual void notifyWorldGeometryRenderQueue(uint8 qid) = 0;
		/* Called when a _renderScene is called in the SceneManager */
		virtual void notifyBeginRenderScene(void) = 0;
		/* called by PCZSM during setZoneGeometry() */
		virtual void setZoneGeometry(const String &filename, PCZSceneNode * parentNode) = 0;
		/* get the world coordinate aabb of the zone */
		virtual void getAABB(AxisAlignedBox &);
		void setPortalsUpdated(bool updated)   { mPortalsUpdated = updated;    }
		bool getPortalsUpdated(void)      { return mPortalsUpdated;   }
		/* get & set the user data */
		void * getUserData(void) {return mUserData;}
		void setUserData(void * userData) {mUserData = userData;}
		/** list of Portals which this zone contains (each portal leads to another zone)
		*/
		PortalList mPortals;
		// pointer to the pcz scene manager that created this zone
		PCZSceneManager * mPCZSM;

	protected:
		// name of the zone (must be unique)
		String mName;
		/// Zone type name
		String mZoneTypeName;
        // frame counter for visibility
        unsigned long mLastVisibleFrame;
		// last camera which this zone was visible to
		PCZCamera * mLastVisibleFromCamera;
		// flag determining whether or not this zone has sky in it.
		bool mHasSky;
		//SceneNode which corresponds to the enclosure for this zone
		SceneNode * mEnclosureNode;
		// list of SceneNodes contained in this particular PCZone
        PCZSceneNodeList mHomeNodeList;
		// list of SceneNodes visiting this particular PCZone
        PCZSceneNodeList mVisitorNodeList;
		// flag recording whether any portals in this zone have moved 
		bool mPortalsUpdated;   
		// user defined data pointer - NOT allocated or deallocated by the zone!  
		// you must clean it up yourself!
		void * mUserData;

    };

	class _OgrePCZPluginExport ZoneData : public SceneCtlAllocatedObject
	{
	public:
		/** Standard Constructor */
		ZoneData(PCZSceneNode *, PCZone * );
		/** Standard destructor */
		virtual ~ZoneData();
		/** Update data if neccessary */
		virtual void update(void);
	public:
		PCZone *		mAssociatedZone;
		PCZSceneNode *	mAssociatedNode;

	};

}

#endif


