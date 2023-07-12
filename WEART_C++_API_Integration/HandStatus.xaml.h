//
// HandStatus.xaml.h
// Declaration of the HandStatus class
//

#pragma once

#include "HandStatus.g.h"
#include <WEART_SDK/WeArtCommon.h>

namespace WEART_C___API_Integration
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HandStatus sealed
	{
	public:
		HandStatus();

		property Platform::String^ Hand;
		property Platform::String^ BatteryLevel;
		property Platform::String^ MacAddress;
		property bool Connected;
		property bool Charging;

		property bool ThumbConnected;
		property bool ThumbOk;

		property bool IndexConnected;
		property bool IndexOk;

		property bool MiddleConnected;
		property bool MiddleOk;

		void Refresh();

	private:
		Windows::UI::Color ThimbleColor(bool connected, bool ok);
		double ThimbleOpacity(bool connected, bool ok);
	};
}
