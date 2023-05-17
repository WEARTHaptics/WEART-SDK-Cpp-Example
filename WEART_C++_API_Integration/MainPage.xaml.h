//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include <WEART_SDK/WeArtClient.h>
#include <WEART_SDK/WeArtHapticObject.h>
#include <WEART_SDK/WeArtThimbleTrackingObject.h>
#include <WEART_SDK/WeArtTrackingCalibration.h>

namespace WEART_C___API_Integration
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
	private:
		void ButtonStartClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonStopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonStartCalibration_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonStopCalibration_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonRemoveEffect_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void Connect();
		void TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer);

		void RenderRawSensorsData();
		void RenderCalibrationStatus();

		void OnConnectionStatusChanged(bool connected);

		WeArtClient* weArtClient;
		WeArtHapticObject* hapticObject;
		TouchEffect* touchEffect;

		WeArtTrackingCalibration* calibration;
		bool calibrating = false;

		// Closure tracking
		WeArtThimbleTrackingObject* indexRightThimbleTracking;
		WeArtThimbleTrackingObject* thumbRightThimbleTracking;
		WeArtThimbleTrackingObject* middleRightThimbleTracking;

		WeArtThimbleTrackingObject* indexLeftThimbleTracking;
		WeArtThimbleTrackingObject* thumbLeftThimbleTracking;
		WeArtThimbleTrackingObject* middleLeftThimbleTracking;
	};
}
