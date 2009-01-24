#ifndef QUICKGUIRECT_H
#define QUICKGUIRECT_H

#include "OgrePrerequisites.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIPoint.h"
#include "QuickGUISize.h"

namespace QuickGUI
{
	class _QuickGUIExport Rect
	{
	public:
		Rect();
		Rect(float x, float y, float width, float height);
		Rect(Point p, Size s);

		inline Rect( const Rect& r )
            : x( r.x ), y( r.y ), width( r.width ), height( r.height )
        {
        }

		inline Rect& operator = ( const Rect& r )
        {
            x = r.x;
            y = r.y;
            width = r.width;
            height = r.height;

            return *this;
        }

		inline bool operator != ( const Rect& r ) const
        {
            return ( x != r.x ||
                y != r.y ||
                width != r.width ||
                height != r.height );
        }

		bool operator == ( const Rect& r ) const
        {
			return ( x == r.x &&
				y == r.y &&
				width == r.width &&
				height == r.height );
        }

		Rect getIntersection( const Rect& r ) const;

		bool inside(const Rect& r) const;
		bool intersectsRect(const Rect& r) const;
		bool isPointWithinBounds(const Point& pixelPosition) const;

		float x;
		float y;
		float width;
		float height;

		static const Rect ZERO;
	};
}

#endif
