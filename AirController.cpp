#include "AirController.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "EvacMode.h"

namespace MADAutoDev
{
	AirController::AirController(int dumpValvePin, int supplyValvePin, int pressureSensorPin)
	{
		LCDDisplay = new LiquidCrystal(8, 9, 4, 5, 6, 7);
		_DumpValvePin = dumpValvePin;
		_SupplyValvePin = supplyValvePin;
		Mode = new AutoMode(_DumpValvePin, _SupplyValvePin);
		_CurrentMode = Auto;
		_PressureSensorPin = pressureSensorPin;
	}

	AirController::~AirController()
	{
		free(LCDDisplay);
		free(Mode);
	}

	void AirController::Setup()
	{
		LCDDisplay->begin(16, 2);
		pinMode(_PressureSensorPin, INPUT);
		pinMode(_DumpValvePin, OUTPUT);
		pinMode(_SupplyValvePin, OUTPUT);
		digitalWrite(_DumpValvePin, HIGH);
		digitalWrite(_SupplyValvePin, HIGH);
		_UserDefault = EEPROM.read(eeAddress);
/*		_CurrentMode = EEPROM.read(eeAddress2);  */

	}

	void AirController::ExecuteControllerLogic()
	{
		if (_CurrentButton != None) { return; }
		LCDDisplay->setCursor(3, 0);
		LCDDisplay->print("MODE: ");
		LCDDisplay->setCursor(9, 0);
		if ((_NextMode == AirController::Evac) && (_CurrentMode != Evac))
		{
			LCDDisplay->print(">>DUMP?  ");
		}
		if ((_NextMode == AirController::Auto) && (_CurrentMode != Auto))
		{
			LCDDisplay->print(">>AUTO?  ");
		}

		else
		{
			LCDDisplay->print(Mode->ModeName);
		}

		LCDDisplay->setCursor(1, 1);
		char _message[16] = { '\0' };
		char *message = _message;
		int pins[1] = { _PressureSensorPin };
		Mode->Logic(pins, _UserDefault, _Offset, message);
		LCDDisplay->print(_message);
		EEPROM.put(eeAddress, _UserDefault);
	}

	void AirController::ProcessButtonClicks(int buttonPin)
	{
		int buttonValue = analogRead(buttonPin);
		AirController::Buttons button = getButtonType(buttonValue);
		LCDDisplay->setCursor(9, 0);

		switch (button)
		{
		case AirController::Left:

			if (_CurrentButton != AirController::Left)
			{
				LCDDisplay->print("INVALID\0");
				_CurrentButton = AirController::Left;
			}
			break;

		case AirController::Right:
			if (_CurrentButton != AirController::Right)
			{
				_CurrentButton = AirController::Right;


				if (_NextMode == AirController::NoMode)
				{
					_NextMode = AirController::Evac;

				}

				else if (_NextMode == AirController::Evac)
				{
					_NextMode = AirController::Auto;
				}

				else if (_NextMode == AirController::Auto)
				{
					_NextMode = AirController::NoMode;
				}
			}
			break;

		case AirController::Up:
			if (_CurrentButton != AirController::Up && _UserDefault <= 90)
			{
				_CurrentButton = AirController::Up;
				_UserDefault = _UserDefault + 5;

			}
			break;

		case AirController::Down:
			if (_CurrentButton != AirController::Down)
			{
				_CurrentButton = AirController::Down;
				_UserDefault = _UserDefault - 5;
			}
			break;

		case AirController::Select:
			if (_CurrentButton != AirController::Select)
			{
				_CurrentButton = AirController::Select;
				LCDDisplay->print(Mode->ModeName);
				if (_NextMode != NoMode)
				{
					_CurrentMode = _NextMode;
				}
				if (_CurrentMode == Evac)
				{
					Mode = new EvacMode(_DumpValvePin, _SupplyValvePin);
				}
				else if (_CurrentMode == Auto)
				{
					Mode = new AutoMode(_DumpValvePin, _SupplyValvePin);
				}
			}
			break;

		case AirController::None:
			if (_CurrentButton != AirController::None)
			{

				_CurrentButton = AirController::None;
				LCDDisplay->print("          \0");
			}
			break;

		}

	}

	AirController::Buttons 	AirController::getButtonType(int buttonValue)
	{
		//delay(30);
		if (buttonValue > 790)  return AirController::None;
		if (buttonValue < 50) 	return AirController::Right;
		if (buttonValue < 195)  return AirController::Up;
		if (buttonValue < 380)  return AirController::Down;
		if (buttonValue < 555)  return AirController::Left;
		if (buttonValue < 790)  return AirController::Select;
		return AirController::None;

	}

} 