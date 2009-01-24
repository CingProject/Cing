#ifndef QUICKGUITYPEDEF_H
#define QUICKGUITYPEDEF_H

namespace QuickGUI
{
	// Stores the Texture of a quad, and the index of the first quad following it with a different texture.
	typedef std::pair<std::string,unsigned int > TextureQuadIndex;
	typedef std::vector<TextureQuadIndex > TextureChangeList;

	typedef std::list<Quad*> QuadList;
	typedef std::vector<Quad*> QuadArray;

	typedef std::list<QuadContainer*> QuadContainerList;

	typedef std::vector<MemberFunctionSlot*> EventHandlerArray;

	typedef std::vector<Widget*> WidgetArray;
}

#endif
