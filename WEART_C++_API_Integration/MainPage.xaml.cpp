//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

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
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	
	weArtClient = new WeArtClient("192.168.1.109", "13031");

	hapticObject = new WeArtHapticObject(weArtClient);
	hapticObject->handSideFlag = HandSide::Right;
	hapticObject->actuationPointFlag = ActuationPoint::Index;

	WeArtTemperature temperature = WeArtTemperature();
	WeArtForce force = WeArtForce();
	WeArtTexture texrure = WeArtTexture();

	touchEffect = new TouchEffect(temperature, force, texrure);

	hapticObject->AddEffect(touchEffect);

	indexThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Index);
	weArtClient->AddThimbleTracking(indexThimbleTracking);

	TimeSpan period;
	period.Duration = 0.2 * 10000000; // 0.2se
	ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(this, &MainPage::TestTimer), period);

	weArtClient->Run();
}

void MainPage::TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer)
{
	std::ostringstream ss;
	ss << indexThimbleTracking->GetClosure();
	std::string s = "Index Thimble Closure: " + ss.str() + "\n";
	
	OutputDebugStringA(s.c_str());

}



void WEART_C___API_Integration::MainPage::ButtonStartClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->Start();
}


void WEART_C___API_Integration::MainPage::ButtonStopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->Stop();
}


void WEART_C___API_Integration::MainPage::ButtonEffectSample1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	WeArtTemperature temperature = WeArtTemperature();
	temperature.active = true;
	temperature.value = 0.2f;

	WeArtForce force = WeArtForce();
	force.active = true;
	force.value = 0.4f;

	WeArtTexture texture = WeArtTexture();

	touchEffect->Set(temperature, force, texture);

	if (hapticObject->activeEffects.size() <= 0)
		hapticObject->AddEffect(touchEffect);
	else 
		hapticObject->UpdateEffects();
}


void WEART_C___API_Integration::MainPage::ButtonEffectSample2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	WeArtTemperature temperature = WeArtTemperature();
	temperature.active = true;
	temperature.value = 0.4f;

	WeArtForce force = WeArtForce();
	force.active = true;
	force.value = 0.6f;

	WeArtTexture texture = WeArtTexture();

	touchEffect->Set(temperature, force, texture);

	if (hapticObject->activeEffects.size() <= 0)
		hapticObject->AddEffect(touchEffect);
	else
		hapticObject->UpdateEffects();
}


void WEART_C___API_Integration::MainPage::ButtonEffectSample3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	WeArtTemperature temperature = WeArtTemperature();
	temperature.active = true;
	temperature.value = 0.8f;

	WeArtForce force = WeArtForce();
	force.active = true;
	force.value = 0.9f;

	WeArtTexture texture = WeArtTexture();
	texture.active = true;
	texture.textureType = TextureType::TextileMeshMedium;

	touchEffect->Set(temperature, force, texture);

	if (hapticObject->activeEffects.size() <= 0)
		hapticObject->AddEffect(touchEffect);
	else
		hapticObject->UpdateEffects();
}


void WEART_C___API_Integration::MainPage::ButtonRemoveEffect_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	hapticObject->RemoveEffect(touchEffect);
}
