#pragma once
#include "LiquidCrystal.h"
#include "IControllerMode.h"
#include "AutoMode.h"
#include "EEPROM.h"

namespace MADAutoDev
{
	class AirController
	{
	public:
		enum Buttons { Left, Right, Up, Down, Select, None };
		enum Modes { Auto, Evac, NoMode };
		AirController(int dumpValvePin, int supplyValvePin, int pressureSensorPin);
		~AirController();
		void Setup();
		void ExecuteControllerLogic();
		void ProcessButtonClicks(int buttonPin);
	private:
		Buttons getButtonType(int buttonValue);
		LiquidCrystal *LCDDisplay;
		IControllerMode *Mode;
		Modes _NextMode = NoMode;
		int _SupplyValvePin = 0;
		int _DumpValvePin = 0;
		int _PressureSensorPin = 0;
		int _UserDefault = 35;
		int eeAddress = 1;
	//	Modes eeAddressChar = 2;
		int _Offset = 3;
		int buttonCheck = 0;
		Buttons _CurrentButton = None;
		Modes _CurrentMode = NoMode;
	};
}