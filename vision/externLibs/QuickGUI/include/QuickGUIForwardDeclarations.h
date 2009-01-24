#ifndef QUICKGUIFORWARDDECLARATIONS_H
#define QUICKGUIFORWARDDECLARATIONS_H

// Ogre specific forward declarations
namespace Ogre
{
	class Image;
	class RenderSystem;
}

// QuickGUI specific forward declarations
namespace QuickGUI
{
	////////////DRAWING/////////////
	const int VERTICES_PER_QUAD = 6;

	class Rect;
	class Point;
	class Size;

	struct Vertex;
	class VertexBuffer;
	class QuadContainer;
	class Quad;

	////////////EVENTS/////////////
	class MemberFunctionSlot;
	class EventArgs;
	class WidgetEventArgs;
	class MouseEventArgs;

	////////////UTILS/////////////
	class MouseCursor;
	class Utility;
	class ConfigScriptLoader;

	////////////Handlers/////////////
	class GUIManager;
	class Sheet;
	class SkinSet;

	////////////Effects/////////////
	class Effect;
	class AlphaEffect;
	class MoveEffect;
	class SizeEffect;

	////////////Widgets/////////////
	class Widget;

	class Window;

	class Image;
	class Panel;
	class Border;
	class ScrollPane;
	class Button;
	class NStateButton;

	class Text;
	class Label;
	class MenuLabel;
	class LabelArea;
	class TextBox;
	class Console;
	class TextArea;
	class TextBox;

	class List;
	class ComboBox;
	class TitleBar;
	class Tree;

	class ProgressBar;
	class HorizontalTrackBar;
	class VerticalTrackBar;
	class VerticalScrollBar;
	class HorizontalScrollBar;

}
#endif //__QUICKPREQUISITES_H
