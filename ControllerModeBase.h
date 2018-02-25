#pragma once
#include "string.h"
#include "IControllerMode.h"

namespace MADAutoDev
{
	class ControllerModeBase
	{
	public:
		enum ModeState { Dumping, Supplying, NoReading, InAcceptableRange };
		ControllerModeBase(int dumpValvePin, int supplyValvePin);
		~ControllerModeBase();
		int PressureSensorValue = 0;
		int DumpValvePin = 0;
		int SupplyValvePin = 0;
		ModeState CurrentModeState = NoReading;
	};
}