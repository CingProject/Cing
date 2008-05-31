#ifndef QUICKGUIRADIOBUTTONGROUP_H
#define QUICKGUIRADIOBUTTONGROUP_H

#include "QuickGUIRadioButton.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport RadioButtonGroup
	{
	public:
		RadioButtonGroup(Widget* owner);
		~RadioButtonGroup();

		RadioButton* createRadioButton();
		RadioButton* createRadioButton(const std::string& name);

		RadioButton* getSelectedRadioButton();

		void setSelectedRadioButton(RadioButton* b);

	protected:
		Widget* mOwner;

		RadioButton* mSelectedRadioButton;
		std::vector<RadioButton*> mRadioButtons;

		void onRadioButtonRemoved(const EventArgs& args);
	};
}

#endif
