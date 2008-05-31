#ifndef QUICKGUIMULTILINETEXTBOX_H
#define QUICKGUIMULTILINETEXTBOX_H

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUILabelArea.h"

namespace QuickGUI
{
	class _QuickGUIExport TextArea :
		public LabelArea
	{
	public:
		TextArea(const std::string& name, GUIManager* gm);
		~TextArea();

	protected:
	};
}

#endif
