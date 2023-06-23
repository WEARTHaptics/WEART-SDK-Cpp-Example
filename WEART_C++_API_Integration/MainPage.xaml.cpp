//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <locale>
#include <codecvt>

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

// The Blank Page dispatchItem template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	
	weArtClient = new WeArtClient(WeArtConstants::DEFAULT_IP_ADDRESS, WeArtConstants::DEFAULT_TCP_PORT); //IP ADDRESS and PORT of Middleware PC	
	
	// Add connection status callback to get notified when the client connects and disconnects from the middleware
	weArtClient->AddConnectionStatusCallback([this](bool connected) { OnConnectionStatusChanged(connected); });

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
	indexRightThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Index); weArtClient->AddThimbleTracking(indexRightThimbleTracking);
	thumbRightThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Thumb); weArtClient->AddThimbleTracking(thumbRightThimbleTracking);
	middleRightThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Right, ActuationPoint::Middle); weArtClient->AddThimbleTracking(middleRightThimbleTracking);

	indexLeftThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Left, ActuationPoint::Index); weArtClient->AddThimbleTracking(indexLeftThimbleTracking);
	thumbLeftThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Left, ActuationPoint::Thumb); weArtClient->AddThimbleTracking(thumbLeftThimbleTracking);
	middleLeftThimbleTracking = new WeArtThimbleTrackingObject(HandSide::Left, ActuationPoint::Middle); weArtClient->AddThimbleTracking(middleLeftThimbleTracking);

	// Add all sensor data trackers
	AddSensor("LEFT", "INDEX");
	AddSensor("LEFT", "MIDDLE");
	AddSensor("LEFT", "THUMB");
	AddSensor("LEFT", "PALM");
	AddSensor("RIGHT", "INDEX");
	AddSensor("RIGHT", "MIDDLE");
	AddSensor("RIGHT", "THUMB");
	AddSensor("RIGHT", "PALM");

	// schedule reading closure value any 0.1secs
	TimeSpan period;
	period.Duration = 0.1 * 10000000; // 0.1sec
	ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(this, &MainPage::TestTimer), period);

	// run socket communication
	Connect();
}

void MainPage::Connect() {
	auto workItem = ref new WorkItemHandler([this](IAsyncAction^ workItem) {
	 	while (!weArtClient->IsConnected())
			weArtClient->Run();
		});
	ThreadPool::RunAsync(workItem);
}

void MainPage::TestTimer(Windows::System::Threading::ThreadPoolTimer^ timer)
{
	Dispatcher->RunAsync(CoreDispatcherPriority::High,
		ref new DispatchedHandler([this]()
			{
				RenderCalibrationStatus();
				RenderClosureAbduction();
				RenderRawSensorsData();
			}));
}

void MainPage::RenderClosureAbduction() {
	ValueIndexRightClosure->Text = indexRightThimbleTracking->GetClosure().ToString();
	ValueThumbRightClosure->Text = thumbRightThimbleTracking->GetClosure().ToString();
	ValueThumbRightAbduction->Text = thumbRightThimbleTracking->GetAbduction().ToString();
	ValueMiddleRightClosure->Text = middleRightThimbleTracking->GetClosure().ToString();

	ValueIndexLeftClosure->Text = indexLeftThimbleTracking->GetClosure().ToString();
	ValueThumbLeftClosure->Text = thumbLeftThimbleTracking->GetClosure().ToString();
	ValueThumbLeftAbduction->Text = thumbLeftThimbleTracking->GetAbduction().ToString();
	ValueMiddleLeftClosure->Text = middleLeftThimbleTracking->GetClosure().ToString();
}

void MainPage::RenderRawSensorsData() {
	
	// Find right sensor
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	auto handSideItem = safe_cast<ComboBoxItem^>(HandSideChoice->SelectedItem);
	if (handSideItem == nullptr) return;
	auto handSideWString = handSideItem->Content->ToString();
	if (handSideWString->Length() == 0) return;
	std::string handSide = converterX.to_bytes(handSideWString->Data());

	auto apItem = safe_cast<ComboBoxItem^>(ActuationPointChoice->SelectedItem);
	if (apItem  == nullptr) return;
	auto actuationPointWString = apItem->Content->ToString();
	if (actuationPointWString->Length() == 0) return;
	std::string actuationPoint = converterX.to_bytes(actuationPointWString->Data());
	
	auto key = std::make_pair(handSide, actuationPoint);
	if (sensors.find(key) == sensors.end())
		return;
	WeArtRawSensorsData* sensor = sensors[key];

	// Print data
	WeArtRawSensorsData::Sample sample = sensor->GetLastSample();
	Acc_X->Text = sample.data.accelerometer.x.ToString();
	Acc_Y->Text = sample.data.accelerometer.y.ToString();
	Acc_Z->Text = sample.data.accelerometer.z.ToString();

	Gyro_X->Text = sample.data.gyroscope.x.ToString();
	Gyro_Y->Text = sample.data.gyroscope.y.ToString();
	Gyro_Z->Text = sample.data.gyroscope.z.ToString();

	TimeOfFlight->Text = sample.data.timeOfFlight.distance.ToString();

	LastSampleTime->Text = sample.timestamp.ToString();
}

void MainPage::RenderCalibrationStatus() {
	if (!calibrating) {
		if(weArtClient->IsConnected())
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

void MainPage::OnConnectionStatusChanged(bool connected) {
	Dispatcher->RunAsync(CoreDispatcherPriority::High,
		ref new DispatchedHandler([this, connected]() {
			TextConnectionStatus->Text = connected ? "Connected" : "Not Connected";
			TextConnectionStatus->Foreground = ref new SolidColorBrush(connected ? Windows::UI::Colors::Green : Windows::UI::Colors::Red);

			ButtonStartClient->IsEnabled = connected;
			ButtonStopClient->IsEnabled = connected;

			ButtonStartCalibration->IsEnabled = connected;
			ButtonStartRawData->IsEnabled = connected;
			ButtonStopRawData->IsEnabled = connected;

			ButtonEffectSample1->IsEnabled = connected;
			ButtonEffectSample2->IsEnabled = connected;
			ButtonEffectSample3->IsEnabled = connected;
			ButtonRemoveEffect->IsEnabled = connected;

			}));

	if (!connected)
		Connect();
}



void MainPage::ButtonStartClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->Start();
}


void MainPage::ButtonStopClient_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->Stop();
}

void MainPage::ButtonStartCalibration_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	calibrating = true;
	weArtClient->StartCalibration();
}

void MainPage::ButtonStartRawData_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->StartRawData();
}

void MainPage::ButtonStopRawData_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	weArtClient->StopRawData();
}

void MainPage::ButtonEffectSample1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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


void MainPage::ButtonEffectSample2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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


void MainPage::ButtonEffectSample3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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


void MainPage::ButtonRemoveEffect_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//remove effect from thimble actuations
	hapticObject->RemoveEffect(touchEffect);
}



void MainPage::AddSensor(std::string handSide, std::string actuationPoint) {

	HandSide hs = StringToHandside(handSide);
	ActuationPoint ap = StringToActuationPoint(actuationPoint);

	auto sensor = new WeArtRawSensorsData(hs, ap);
	sensors[std::make_pair(handSide, actuationPoint)] = sensor;
	weArtClient->AddMessageListener(sensor);
}
