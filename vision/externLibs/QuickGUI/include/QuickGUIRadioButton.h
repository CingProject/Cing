#ifndef QUICKGUIRADIOBUTTON_H
#define QUICKGUIRADIOBUTTON_H

#include "QuickGUIButton.h"

namespace QuickGUI
{
	class _QuickGUIExport RadioButton :
		public Button
	{
	public:
		friend class RadioButtonGroup;
	public:
		RadioButton(const std::string& name, GUIManager* gm);
		~RadioButton();

		/**
		* Add user defined events, that will be called when a check state has changed.
		*/
		template<typename T> void addOnSelectedEventHandler(void (T::*function)(const EventArgs&), T* obj)
		{
			mOnSelectedUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj)); 
		}
		void addOnSelectedEventHandler(MemberFunctionSlot* function);

		bool isSelected();

		void select();

	protected:
		bool mSelected;

		// User defined event handlers that are called when a Selection is made.
		EventHandlerArray mOnSelectedUserEventHandlers;
	protected:
		/**
		* Event Handler for the EVENT_MOUSE_BUTTON_DOWN event.
		*/
		virtual void onMouseButtonUp(const EventArgs& args);

		void _select();
		void _deselect();
	};
}

#endif
