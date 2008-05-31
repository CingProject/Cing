#ifndef QUICKGUIPOINT_H
#define QUICKGUIPOINT_H

#include "OgrePrerequisites.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISize.h"

namespace QuickGUI
{
	class _QuickGUIExport Point
	{
	public:
		Point();
		Point(float X, float Y);

		inline Point( const Point& p )
            : x( p.x ), y( p.y )
        {
        }

		inline Point& operator = ( const Point& p )
        {
            x = p.x;
            y = p.y;

            return *this;
        }

		inline bool operator != ( const Point& p ) const
        {
            return ( x != p.x ||
                y != p.y );
        }

		inline Point operator * ( const float r ) const
        {
            return Point(x * r,y * r);
        }

		inline Point operator / ( const Point& p ) const
        {
            return Point(x / p.x,y / p.y);
        }

		inline Point operator / ( const Size& s ) const
        {
            return Point(x / s.width,y / s.height);
        }

		inline Point operator + ( const Point& p ) const
        {
            return Point(x + p.x,y + p.y);
        }

		inline Point operator + ( const Size& s ) const
        {
            return Point(x + s.width,y + s.height);
        }

		inline Point operator += ( const Size& s ) const
        {
            return Point(x + s.width,y + s.height);
        }

		inline Point operator - ( const Point& p ) const
        {
            return Point(x - p.x,y - p.y);
        }

		float x;
		float y;

		static const Point ZERO;
	};
}

#endif
