//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "HandStatus.xaml.h"
#include <WEART_SDK/WeArtCommon.h>
#include <WEART_SDK/WeArtClient.h>
#include <WEART_SDK/WeArtHapticObject.h>
#include <WEART_SDK/WeArtThimbleTrackingObject.h>
#include <WEART_SDK/WeArtTrackingCalibration.h>
#include <WEART_SDK/MiddlewareStatusListener.h>

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
		void ButtonEffectSample1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonRemoveEffect_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonStartRawData_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonStopRawData_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void Connect();
		void TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer);

		void RenderClosureAbduction();
		void RenderTrackingRawSensorsData();
		void RenderAnalogRawSensorData();
		void RenderCalibrationStatus();
		void RenderMiddlewareStatus();
		void RenderDevicesStatus();

		void RenderHand(HandStatus^ hand, bool connected, ConnectedDeviceStatus status);

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

		// Raw Tracking Sensors
		std::map < std::pair < std::string, std::string > , WeArtTrackingRawData* > trackingSensors;
		std::map < std::pair < std::string, std::string >, WeArtAnalogSensorData* > analogSensors;

		std::pair<std::string, std::string> GetSensorChoice();
		void AddSensor(std::string handSide, std::string actuationPoint);

		// Middleware status
		MiddlewareStatusListener* mwListener;

		Platform::String^ stdToPlatformString(const std::string& input);
		void TextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
