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
	
	weArtClient = new WeArtClient("127.0.0.1", WeArtConstants::DEFAULT_TCP_PORT); //IP ADDRESS and PORT of Middleware PC

	// create haptic object to manage actuation on Righ hand and Index Thimble
	hapticObject = new WeArtHapticObject(weArtClient);
	hapticObject->handSideFlag = HandSide::Right;
	hapticObject->actuationPointFlag = ActuationPoint::Index;

	//define feeling properties to create an effect
	WeArtTemperature temperature = WeArtTemperature();
	WeArtForce force = WeArtForce();
	WeArtTexture texrure = WeArtTexture();

	// instance a new effect with feeling properties and add effect to thimble
	touchEffect = new TouchEffect(temperature, force, texrure);

	hapticObject->AddEffect(touchEffect);

	// reference a thimble tracking to read clousure values
	indexThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Index);
	weArtClient->AddThimbleTracking(indexThimbleTracking);

	// index thimble raw sensors data
	indexRightRawSensorData = new WeArtRawSensorData(HandSide::Right, ActuationPoint::Index);
	weArtClient->AddThimbleRawSensors(indexRightRawSensorData);

	//thumb thimble raw sensors data
	thumbRightRawSensorData = new WeArtRawSensorData(HandSide::Right, ActuationPoint::Thumb);
	weArtClient->AddThimbleRawSensors(thumbRightRawSensorData);

	//thumb thimble raw sensors data
	middleRightRawSensorData = new WeArtRawSensorData(HandSide::Right, ActuationPoint::Middle);
	weArtClient->AddThimbleRawSensors(middleRightRawSensorData);


	// schedule reading closure value any 0.2secs
	TimeSpan period;
	period.Duration = 0.2 * 10000000; // 0.2sec
	ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(this, &MainPage::TestTimer), period);

	// run socket communication 
	weArtClient->Run();
}

void MainPage::TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer)
{
	/*
	// get closure value and print 
	std::ostringstream ss;
	ss << indexThimbleTracking->GetClosure();
	std::string s = "Index Thimble Closure: " + ss.str() + "\n";
	
	OutputDebugStringA(s.c_str());
	*/
	Dispatcher->RunAsync(CoreDispatcherPriority::High,
		ref new DispatchedHandler([this]()
			{
				RenderRawSensorsData();

			}));
}

void WEART_C___API_Integration::MainPage::RenderRawSensorsData() {
	// Index Right
	ValueIndexRightAccX->Text = indexRightRawSensorData->AccX.ToString();
	ValueIndexRightAccY->Text = indexRightRawSensorData->AccY.ToString();
	ValueIndexRightAccZ->Text = indexRightRawSensorData->AccZ.ToString();

	ValueIndexRightGyroX->Text = indexRightRawSensorData->GyroX.ToString();
	ValueIndexRightGyroY->Text = indexRightRawSensorData->GyroY.ToString();
	ValueIndexRightGyroZ->Text = indexRightRawSensorData->GyroZ.ToString();

	ValueIndexRightTOF->Text = indexRightRawSensorData->TOF.ToString();

	// Thumb Right
	ValueThumbRightAccX->Text = thumbRightRawSensorData->AccX.ToString();
	ValueThumbRightAccY->Text = thumbRightRawSensorData->AccY.ToString();
	ValueThumbRightAccZ->Text = thumbRightRawSensorData->AccZ.ToString();

	ValueThumbRightGyroX->Text = thumbRightRawSensorData->GyroX.ToString();
	ValueThumbRightGyroY->Text = thumbRightRawSensorData->GyroY.ToString();
	ValueThumbRightGyroZ->Text = thumbRightRawSensorData->GyroZ.ToString();

	ValueThumbRightTOF->Text = thumbRightRawSensorData->TOF.ToString();

	// MIddle Right
	ValueMiddleRightAccX->Text = middleRightRawSensorData->AccX.ToString();
	ValueMiddleRightAccY->Text = middleRightRawSensorData->AccY.ToString();
	ValueMiddleRightAccZ->Text = middleRightRawSensorData->AccZ.ToString();

	ValueMiddleRightGyroX->Text = middleRightRawSensorData->GyroX.ToString();
	ValueMiddleRightGyroY->Text = middleRightRawSensorData->GyroY.ToString();
	ValueMiddleRightGyroZ->Text = middleRightRawSensorData->GyroZ.ToString();

	ValueMiddleRightTOF->Text = middleRightRawSensorData->TOF.ToString();
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
	// define temperature
	WeArtTemperature temperature = WeArtTemperature();
	temperature.active = true; //must be active if you want feel
	temperature.value(0.2f);

	// define force
	WeArtForce force = WeArtForce();
	force.active = true;
	force.value(0.4f);

	// define null Texture
	WeArtTexture texture = WeArtTexture();

	// set properties to effect
	touchEffect->Set(temperature, force, texture);

	// add effect to thimble or update
	if (hapticObject->activeEffects.size() <= 0)
		hapticObject->AddEffect(touchEffect);
	else 
		hapticObject->UpdateEffects();
}


void WEART_C___API_Integration::MainPage::ButtonEffectSample2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// define temperature
	WeArtTemperature temperature = WeArtTemperature();
	temperature.active = true;
	temperature.value(0.4f);

	// define force
	WeArtForce force = WeArtForce();
	force.active = true;
	force.value(0.6f);

	// define null Texture
	WeArtTexture texture = WeArtTexture();

	// set properties to effect
	touchEffect->Set(temperature, force, texture);

	// add effect to thimble or update
	if (hapticObject->activeEffects.size() <= 0)
		hapticObject->AddEffect(touchEffect);
	else
		hapticObject->UpdateEffects();
}


void WEART_C___API_Integration::MainPage::ButtonEffectSample3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// define temperature
	WeArtTemperature temperature = WeArtTemperature();
	temperature.active = true;
	temperature.value(0.8f);

	// define force
	WeArtForce force = WeArtForce();
	force.active = true;
	force.value(0.9f);

	// define TextileMeshMedium Texture
	WeArtTexture texture = WeArtTexture();
	texture.active = true;
	texture.textureType(TextureType::TextileMeshMedium);

	// set properties to effect
	touchEffect->Set(temperature, force, texture);

	// add effect to thimble or update
	if (hapticObject->activeEffects.size() <= 0)
		hapticObject->AddEffect(touchEffect);
	else
		hapticObject->UpdateEffects();
}


void WEART_C___API_Integration::MainPage::ButtonRemoveEffect_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//remove effect from thimble actuations
	hapticObject->RemoveEffect(touchEffect);
}
