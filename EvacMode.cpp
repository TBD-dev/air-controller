#include "IControllerMode.h"
#include "EvacMode.h"
#include "Arduino.h"

namespace MADAutoDev
{
	EvacMode::EvacMode(int dumpValvePin, int supplyValvePin) : ControllerModeBase(dumpValvePin, supplyValvePin)
	{
		strcpy(ModeName, "DUMP");
	}

	EvacMode::~EvacMode()
	{

	}

	void EvacMode::Logic(int sensorPins[], int userPreset, int offset, char *message)
	{
		char _int_char[6];

		// Read Pressure
		timer++;

		if (timer >= 100)
		{
			PressureSensorValue = analogRead(sensorPins[0]) / 10;
			timer = 0;
		}

		// Update Current Mode State
		CurrentModeState = UpdateCurrentModeState(userPreset, offset);

		// Set valves to dump air
		if (PressureSensorValue > 0)
		{
			digitalWrite(SupplyValvePin, HIGH); // Close Supply Valve
			digitalWrite(DumpValvePin, LOW);  // Open Dump Valve
		}
		else 
		{
			digitalWrite(SupplyValvePin, HIGH); // Close Supply Valve
			digitalWrite(DumpValvePin, HIGH);  // Close Dump Valve
		}

		// Create Message For LCD
		
		if (PressureSensorValue < 5) {
			strcat(message, "  LOW AIR !!    ");
		}
		else if (PressureSensorValue > 5 && PressureSensorValue < 99) {
			strcat(message, "DUMPING AIR...  ");
			
		}
		else if (PressureSensorValue < 0 || PressureSensorValue > 99) {
			strcat(message, "SENSOR ERROR!!  ");
		}
		strcat(message, "\0");
	}

	ControllerModeBase::ModeState EvacMode::UpdateCurrentModeState(int userPreset, int offset)
	{
		switch (CurrentModeState)
		{
		case ControllerModeBase::Dumping:
			return PressureSensorValue <= userPreset ? ControllerModeBase::InAcceptableRange : CurrentModeState;
		case ControllerModeBase::Supplying:
			return PressureSensorValue >= userPreset ? ControllerModeBase::InAcceptableRange : CurrentModeState;
		case ControllerModeBase::NoReading:
		case ControllerModeBase::InAcceptableRange:
		default:
			if (PressureSensorValue < (userPreset - offset)) { return ControllerModeBase::Supplying; }
			else if (PressureSensorValue >(userPreset + offset)) { return ControllerModeBase::Dumping; }
			else { return ControllerModeBase::InAcceptableRange; }
		}

	}

}