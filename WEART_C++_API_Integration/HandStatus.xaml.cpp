//
// HandStatus.xaml.cpp
// Implementation of the HandStatus class
//

#include "pch.h"
#include "HandStatus.xaml.h"

using namespace WEART_C___API_Integration;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

HandStatus::HandStatus()
{
	InitializeComponent();
}

void WEART_C___API_Integration::HandStatus::Refresh()
{	
	Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
		ref new Windows::UI::Core::DispatchedHandler([this]()
			{
				// Set hand scale (reversed if right)
				ScaleTransform^ scale = ref new ScaleTransform();
				scale->ScaleX = Hand == "RIGHT" ? -1 : 1;
				HandCanvas->RenderTransform = scale;

				// Set connection status
				HandImage->Opacity = Connected ? 1 : 0.5;
				
				// Set mac address
				MacAddressText->Visibility = Connected ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
				MacAddressText->Text = MacAddress;
				
				// Render battery
				BatteryPanel->Visibility = Connected ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
				BatteryIconCharging->Visibility = Charging ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
				BatteryIconNotCharging->Visibility = !Charging ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
				BatteryLevelText->Text = BatteryLevel;

				// Set Thimbles
				ThumbThimble->Fill = ref new SolidColorBrush(ThimbleColor(ThumbConnected, ThumbOk));
				ThumbThimble->Opacity = ThimbleOpacity(ThumbConnected, ThumbOk);

				IndexThimble->Fill = ref new SolidColorBrush(ThimbleColor(IndexConnected, IndexOk));
				IndexThimble->Opacity = ThimbleOpacity(IndexConnected, IndexOk);

				MiddleThimble->Fill = ref new SolidColorBrush(ThimbleColor(MiddleConnected, MiddleOk));
				MiddleThimble->Opacity = ThimbleOpacity(MiddleConnected, MiddleOk);
			}));
}

Windows::UI::Color WEART_C___API_Integration::HandStatus::ThimbleColor(bool connected, bool ok)
{
	if (!Connected || !connected) return Windows::UI::Colors::Black;
	return ok ? Windows::UI::Colors::Green : Windows::UI::Colors::Red;
}

double WEART_C___API_Integration::HandStatus::ThimbleOpacity(bool connected, bool ok)
{
	return Connected && connected ? 1 : 0.4;
}
