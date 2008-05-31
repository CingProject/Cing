#ifndef QUICKGUISIZE_H
#define QUICKGUISIZE_H

#include "OgrePrerequisites.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIExportDLL.h"

namespace QuickGUI
{
	class _QuickGUIExport Size
	{
	public:
		Size();
		Size(float Width, float Height);

		inline Size( const Size& s )
            : width( s.width ), height( s.height )
        {
        }

		inline Size& operator = ( const Size& s )
        {
            width = s.width;
            height = s.height;

            return *this;
        }

		inline bool operator != ( const Size& s ) const
        {
            return ( width != s.width ||
                height != s.height );
        }

		inline Size operator * ( const float& r ) const
        {
            return Size(width * r,height * r);
        }

		inline Size operator * ( const Size& s ) const
        {
			return Size(width * s.width,height * s.height);
        }

		inline Size operator / ( const Size& s ) const
        {
            return Size(width / s.width,height / s.height);
        }

		inline Size operator + ( const Size& s ) const
        {
            return Size(width + s.width,height + s.height);
        }

		inline Size operator - ( const Size& s ) const
        {
            return Size(width - s.width,height - s.height);
        }

		float width;
		float height;

		static const Size ZERO;
	};
}

#endif
