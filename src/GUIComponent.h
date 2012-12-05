#pragma once
#include "FastDelegate.h"
#include "EventArgs.h"

namespace Habanero
{
	namespace GUI
	{
		class GUIComponent
		{
		private:
			fastdelegate::FastDelegate2<const GUIComponent &, const MouseEventArgs &> onClickCallback;
			fastdelegate::FastDelegate2<const GUIComponent &, const MouseEventArgs &> onMouseOverCallback;
			fastdelegate::FastDelegate2<const GUIComponent &, const MouseEventArgs &> onMouseOutCallback;
			fastdelegate::FastDelegate2<const GUIComponent &, const MouseEventArgs &> onMouseDownCallback;
			fastdelegate::FastDelegate2<const GUIComponent &, const MouseEventArgs &> onMouseUpCallback;
		};
	}
}