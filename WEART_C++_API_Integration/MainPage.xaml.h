﻿//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include <WEART_SDK/WeArtClient.h>
#include <WEART_SDK/WeArtHapticObject.h>
#include <WEART_SDK/WeArtThimbleTrackingObject.h>
#include "TouchEffect.h"

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
		void ButtonEffectSample1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonEffectSample3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ButtonRemoveEffect_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer);

		WeArtClient* weArtClient;
		WeArtHapticObject* hapticObject;
		TouchEffect* touchEffect;
		WeArtThimbleTrackingObject* indexThimbleTracking;
	};
}