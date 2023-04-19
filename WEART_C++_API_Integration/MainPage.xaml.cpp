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

	// Add calibration tracker
	calibration = new WeArtTrackingCalibration();
	weArtClient->AddMessageListener(calibration);

	// instance a new effect with feeling properties and add effect to thimble
	touchEffect = new TouchEffect(temperature, force, texrure);
	hapticObject->AddEffect(touchEffect);

	// Add all thimble tracking objects for closure
	indexRightThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Index);
	weArtClient->AddThimbleTracking(indexRightThimbleTracking);

	thumbRightThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Thumb);
	weArtClient->AddThimbleTracking(thumbRightThimbleTracking);

	middleRightThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Middle);
	weArtClient->AddThimbleTracking(middleRightThimbleTracking);

	indexLeftThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Left, ActuationPoint::Index);
	weArtClient->AddThimbleTracking(indexLeftThimbleTracking);

	thumbLeftThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Left, ActuationPoint::Thumb);
	weArtClient->AddThimbleTracking(thumbLeftThimbleTracking);

	middleLeftThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Left, ActuationPoint::Middle);
	weArtClient->AddThimbleTracking(middleLeftThimbleTracking);


	// RIGHT THIMBLE - RAW SENSORS DATA

	// index thimble raw sensors data
	indexRightRawSensorData = new WeArtRawSensorsData(HandSide::Right, ActuationPoint::Index);
	weArtClient->AddThimbleRawSensors(indexRightRawSensorData);

	//thumb thimble raw sensors data
	thumbRightRawSensorData = new WeArtRawSensorsData(HandSide::Right, ActuationPoint::Thumb);
	weArtClient->AddThimbleRawSensors(thumbRightRawSensorData);

	//thumb thimble raw sensors data
	middleRightRawSensorData = new WeArtRawSensorsData(HandSide::Right, ActuationPoint::Middle);
	weArtClient->AddThimbleRawSensors(middleRightRawSensorData);

	//palm raw sensors data
	palmRightRawSensorData = new WeArtRawSensorsData(HandSide::Right, ActuationPoint::Palm);
	weArtClient->AddThimbleRawSensors(palmRightRawSensorData);

	// LEFT THIMBLE - RAW SENSORS DATA

	// index thimble raw sensors data
	indexLeftRawSensorData = new WeArtRawSensorsData(HandSide::Left, ActuationPoint::Index);
	weArtClient->AddThimbleRawSensors(indexLeftRawSensorData);

	//thumb thimble raw sensors data
	thumbLeftRawSensorData = new WeArtRawSensorsData(HandSide::Left, ActuationPoint::Thumb);
	weArtClient->AddThimbleRawSensors(thumbLeftRawSensorData);

	//thumb thimble raw sensors data
	middleLeftRawSensorData = new WeArtRawSensorsData(HandSide::Left, ActuationPoint::Middle);
	weArtClient->AddThimbleRawSensors(middleLeftRawSensorData);

	//palm raw sensors data
	palmLeftRawSensorData = new WeArtRawSensorsData(HandSide::Left, ActuationPoint::Palm);
	weArtClient->AddThimbleRawSensors(palmLeftRawSensorData);

	// schedule reading closure value any 0.1secs
	TimeSpan period;
	period.Duration = 0.1 * 10000000; // 0.1sec
	ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(this, &MainPage::TestTimer), period);

	// run socket communication 
	weArtClient->Run();
}

void MainPage::TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer)
{
	Dispatcher->RunAsync(CoreDispatcherPriority::High,
		ref new DispatchedHandler([this]()
			{
				RenderCalibrationStatus();
				RenderRawSensorsData();
			}));
}

void WEART_C___API_Integration::MainPage::RenderRawSensorsData() {

	// Index Right
	ValueIndexRightClosure->Text = indexRightThimbleTracking->GetClosure().ToString();
	ValueIndexRightAccX->Text = indexRightRawSensorData->GetLastSample()->AccX.ToString();
	ValueIndexRightAccY->Text = indexRightRawSensorData->GetLastSample()->AccY.ToString();
	ValueIndexRightAccZ->Text = indexRightRawSensorData->GetLastSample()->AccZ.ToString();

	ValueIndexRightGyroX->Text = indexRightRawSensorData->GetLastSample()->GyroX.ToString();
	ValueIndexRightGyroY->Text = indexRightRawSensorData->GetLastSample()->GyroY.ToString();
	ValueIndexRightGyroZ->Text = indexRightRawSensorData->GetLastSample()->GyroZ.ToString();

	ValueIndexRightTOF->Text = indexRightRawSensorData->GetLastSample()->TOF.ToString();

	// Thumb Right
	ValueThumbRightClosure->Text = thumbRightThimbleTracking->GetClosure().ToString();
	ValueThumbRightAccX->Text = thumbRightRawSensorData->GetLastSample()->AccX.ToString();
	ValueThumbRightAccY->Text = thumbRightRawSensorData->GetLastSample()->AccY.ToString();
	ValueThumbRightAccZ->Text = thumbRightRawSensorData->GetLastSample()->AccZ.ToString();

	ValueThumbRightGyroX->Text = thumbRightRawSensorData->GetLastSample()->GyroX.ToString();
	ValueThumbRightGyroY->Text = thumbRightRawSensorData->GetLastSample()->GyroY.ToString();
	ValueThumbRightGyroZ->Text = thumbRightRawSensorData->GetLastSample()->GyroZ.ToString();

	ValueThumbRightTOF->Text = thumbRightRawSensorData->GetLastSample()->TOF.ToString();

	// MIddle Right
	ValueMiddleRightClosure->Text = middleRightThimbleTracking->GetClosure().ToString();
	ValueMiddleRightAccX->Text = middleRightRawSensorData->GetLastSample()->AccX.ToString();
	ValueMiddleRightAccY->Text = middleRightRawSensorData->GetLastSample()->AccY.ToString();
	ValueMiddleRightAccZ->Text = middleRightRawSensorData->GetLastSample()->AccZ.ToString();

	ValueMiddleRightGyroX->Text = middleRightRawSensorData->GetLastSample()->GyroX.ToString();
	ValueMiddleRightGyroY->Text = middleRightRawSensorData->GetLastSample()->GyroY.ToString();
	ValueMiddleRightGyroZ->Text = middleRightRawSensorData->GetLastSample()->GyroZ.ToString();

	ValueMiddleRightTOF->Text = middleRightRawSensorData->GetLastSample()->TOF.ToString();

	// Palm Right
	ValuePalmRightAccX->Text = palmRightRawSensorData->GetLastSample()->AccX.ToString();
	ValuePalmRightAccY->Text = palmRightRawSensorData->GetLastSample()->AccY.ToString();
	ValuePalmRightAccZ->Text = palmRightRawSensorData->GetLastSample()->AccZ.ToString();

	ValuePalmRightGyroX->Text = palmRightRawSensorData->GetLastSample()->GyroX.ToString();
	ValuePalmRightGyroY->Text = palmRightRawSensorData->GetLastSample()->GyroY.ToString();
	ValuePalmRightGyroZ->Text = palmRightRawSensorData->GetLastSample()->GyroZ.ToString();

	ValuePalmRightTOF->Text = palmRightRawSensorData->GetLastSample()->TOF.ToString();

	// Index LEFT
	ValueIndexRightClosure->Text = indexRightThimbleTracking->GetClosure().ToString();
	ValueIndexLeftAccX->Text = indexLeftRawSensorData->GetLastSample()->AccX.ToString();
	ValueIndexLeftAccY->Text = indexLeftRawSensorData->GetLastSample()->AccY.ToString();
	ValueIndexLeftAccZ->Text = indexLeftRawSensorData->GetLastSample()->AccZ.ToString();

	ValueIndexLeftGyroX->Text = indexLeftRawSensorData->GetLastSample()->GyroX.ToString();
	ValueIndexLeftGyroY->Text = indexLeftRawSensorData->GetLastSample()->GyroY.ToString();
	ValueIndexLeftGyroZ->Text = indexLeftRawSensorData->GetLastSample()->GyroZ.ToString();

	ValueIndexLeftTOF->Text = indexLeftRawSensorData->GetLastSample()->TOF.ToString();

	// Thumb Left
	ValueThumbRightClosure->Text = thumbRightThimbleTracking->GetClosure().ToString();
	ValueThumbLeftAccX->Text = thumbLeftRawSensorData->GetLastSample()->AccX.ToString();
	ValueThumbLeftAccY->Text = thumbLeftRawSensorData->GetLastSample()->AccY.ToString();
	ValueThumbLeftAccZ->Text = thumbLeftRawSensorData->GetLastSample()->AccZ.ToString();

	ValueThumbLeftGyroX->Text = thumbLeftRawSensorData->GetLastSample()->GyroX.ToString();
	ValueThumbLeftGyroY->Text = thumbLeftRawSensorData->GetLastSample()->GyroY.ToString();
	ValueThumbLeftGyroZ->Text = thumbLeftRawSensorData->GetLastSample()->GyroZ.ToString();

	ValueThumbLeftTOF->Text = thumbLeftRawSensorData->GetLastSample()->TOF.ToString();

	// MIddle Left
	ValueMiddleRightClosure->Text = middleRightThimbleTracking->GetClosure().ToString();
	ValueMiddleLeftAccX->Text = middleLeftRawSensorData->GetLastSample()->AccX.ToString();
	ValueMiddleLeftAccY->Text = middleLeftRawSensorData->GetLastSample()->AccY.ToString();
	ValueMiddleLeftAccZ->Text = middleLeftRawSensorData->GetLastSample()->AccZ.ToString();

	ValueMiddleLeftGyroX->Text = middleLeftRawSensorData->GetLastSample()->GyroX.ToString();
	ValueMiddleLeftGyroY->Text = middleLeftRawSensorData->GetLastSample()->GyroY.ToString();
	ValueMiddleLeftGyroZ->Text = middleLeftRawSensorData->GetLastSample()->GyroZ.ToString();

	ValueMiddleLeftTOF->Text = middleLeftRawSensorData->GetLastSample()->TOF.ToString();

	// Palm Left
	ValuePalmLeftAccX->Text = palmLeftRawSensorData->GetLastSample()->AccX.ToString();
	ValuePalmLeftAccY->Text = palmLeftRawSensorData->GetLastSample()->AccY.ToString();
	ValuePalmLeftAccZ->Text = palmLeftRawSensorData->GetLastSample()->AccZ.ToString();

	ValuePalmLeftGyroX->Text = palmLeftRawSensorData->GetLastSample()->GyroX.ToString();
	ValuePalmLeftGyroY->Text = palmLeftRawSensorData->GetLastSample()->GyroY.ToString();
	ValuePalmLeftGyroZ->Text = palmLeftRawSensorData->GetLastSample()->GyroZ.ToString();

	ValuePalmLeftTOF->Text = palmLeftRawSensorData->GetLastSample()->TOF.ToString();
}

void WEART_C___API_Integration::MainPage::RenderCalibrationStatus() {
	if (!calibrating) {
		ButtonStartCalibration->IsEnabled = true;
		return;
	}

	TextCalibrationStatus->Text = "Calibrating...";

	if(calibration->getStatus() ==  CalibrationStatus::Running) { 
		if (calibration->getResult()) {
			TextCalibrationStatus->Text = "Calibrated!!";
		} else {
			TextCalibrationStatus->Text = "Calibration Error";
		}
		ButtonStartCalibration->IsEnabled = true;
		calibrating = false;
	}
}



void WEART_C___API_Integration::MainPage::ButtonStartClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->Start();
}


void WEART_C___API_Integration::MainPage::ButtonStopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->Stop();
}

void WEART_C___API_Integration::MainPage::ButtonStartCalibration_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	calibrating = true;
	weArtClient->StartCalibration();
	ButtonStartCalibration->IsEnabled = false;
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
