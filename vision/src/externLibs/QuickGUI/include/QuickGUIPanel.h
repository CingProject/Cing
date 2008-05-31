#ifndef QUICKGUIPANEL_H
#define QUICKGUIPANEL_H

#include "OgreStringConverter.h"

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIBorder.h"
#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIConsole.h"
#include "QuickGUIImage.h"
#include "QuickGUILabelArea.h"
#include "QuickGUINStateButton.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIRadioButtonGroup.h"
#include "QuickGUIScrollPane.h"
#include "QuickGUIText.h"
#include "QuickGUITextBox.h"
#include "QuickGUITree.h"
#include "QuickGUIHorizontalTrackBar.h"
#include "QuickGUIVerticalTrackBar.h"
#include "QuickGUIWidget.h"
#include "QuickGUIQuadContainer.h"

namespace QuickGUI
{
	/** Represents a Widget Container.
	@remarks
	The Panel class has the ability to create the majority of defined Widgets.
	The Sheet and Window Widgets derive from this widget (Panel), giving them the
	same abilities.
	@note
	Panels cannot create the TitleBar, Window, or Sheet widget.
	@note
	Panels are meant to be created via the Window and Sheet widget.
	*/
	class _QuickGUIExport Panel :
		public Widget,
		public QuadContainer,
		public RadioButtonGroup
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width, and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				group QuadContainer containing this widget.
			@param
				parentWidget parent widget which created this widget.
        */
		Panel(const std::string& name, GUIManager* gm);

		virtual void addChild(Widget* w);

		Button* createButton();
		Button* createButton(const std::string& name);

		CheckBox* createCheckBox();
		CheckBox* createCheckBox(const std::string& name);

		ComboBox* createComboBox();
		ComboBox* createComboBox(const std::string& name);

		Console* createConsole();
		Console* createConsole(const std::string& name);

		HorizontalScrollBar* createHorizontalScrollBar();
		HorizontalScrollBar* createHorizontalScrollBar(const std::string& name);

		Image* createImage();
		Image* createImage(const std::string& name);

		Label* createLabel();
		Label* createLabel(const std::string& name);

		List* createList();
		List* createList(const std::string& name);

		LabelArea* createLabelArea();
		LabelArea* createLabelArea(const std::string& name);

		NStateButton* createNStateButton();
		NStateButton* createNStateButton(const std::string& name);

		Panel* createPanel();
		Panel* createPanel(const std::string& name);

		ProgressBar* createProgressBar();
		ProgressBar* createProgressBar(const std::string& name);

		TextBox* createTextBox();
		TextBox* createTextBox(const std::string& name);

//		Tree* createTree();
//		Tree* createTree(const std::string& name);

		HorizontalTrackBar* createHorizontalTrackBar();
		HorizontalTrackBar* createHorizontalTrackBar(const std::string& name);

		VerticalScrollBar* createVerticalScrollBar();
		VerticalScrollBar* createVerticalScrollBar(const std::string& name);

		VerticalTrackBar* createVerticalTrackBar();
		VerticalTrackBar* createVerticalTrackBar(const std::string& name);

		virtual QuadContainer* getQuadContainer();
		ScrollPane* getScrollPane();
		virtual Widget* getTargetWidget(const Point& pixelPosition);
		virtual const Widget* getTargetWidget(const Point& pixelPosition) const;

		virtual void show();

	protected:
		virtual ~Panel();
		virtual void setQuadContainer(QuadContainer* container);
		virtual Widget*	_createComponent(const std::string& name, Type t);
	protected:
		ScrollPane* mScrollPane;

		VerticalScrollBar* mRightScrollBar;
		HorizontalScrollBar* mBottomScrollBar;

		void onChildAdded(const EventArgs& args);
		void onChildRemoved(const EventArgs& args);
		void onSizeChanged(const EventArgs& args);
	};
}

#endif
