#include "IControllerMode.h"
#include "AutoMode.h"
#include "Arduino.h"

namespace MADAutoDev
{
	AutoMode::AutoMode(int dumpValvePin, int supplyValvePin) : ControllerModeBase(dumpValvePin, supplyValvePin)
	{
		strcpy(ModeName, "AUTO   ");
	//	Hold Valves Closed 
	//	digitalWrite(dumpValvePin, HIGH);
	//	digitalWrite(supplyValvePin, HIGH);  
	}

	AutoMode::~AutoMode()
	{

	}

	void AutoMode::Logic(int sensorPins[], int userPreset, int offset, char *message)
	{
		char _int_char[6];

		// Read Pressure
		
		timer++;
		
		if (timer >= 100)
		{
			PressureSensorValue = analogRead(sensorPins[0]) / 10;
			timer = 0;
			noSupply = false;
		}
		
		// Update Current Mode State
		CurrentModeState = UpdateCurrentModeState(userPreset, offset);

		// Take action based on Current Mode State
		switch (CurrentModeState)
		{
		case ControllerModeBase::Dumping:
			digitalWrite(SupplyValvePin, HIGH); // Close Supply Valve
			digitalWrite(DumpValvePin, LOW);  // Open Dump Valve
			strcpy(ModeName, "AUTO <-");
			break;
		case ControllerModeBase::Supplying:
			digitalWrite(SupplyValvePin, LOW); // Open Supply Valve
			digitalWrite(DumpValvePin, HIGH);    // Close Dump Valve
			strcpy(ModeName, "AUTO ->");
			break;
		case ControllerModeBase::NoReading:

		case ControllerModeBase::InAcceptableRange:
		default:
			digitalWrite(SupplyValvePin, HIGH); // Close Suppy Valve
			digitalWrite(DumpValvePin, HIGH);   // Close Dump Valve
			strcpy(ModeName, "AUTO   ");
			break;
		}

		// Create Message For LCD
		
		if (PressureSensorValue < 5) {
			strcat(message, "  LOW AIR !!    ");
		}
		else if (PressureSensorValue > 5 && PressureSensorValue < 99) {
			strcpy(message, "SET:");
			sprintf(_int_char, "%d", userPreset);
			strcat(message, _int_char);
			sprintf(_int_char, "%d", PressureSensorValue);
			strcat(message, " READ:");
			strcat(message, _int_char);
		}
		else if (PressureSensorValue < 0 || PressureSensorValue > 99) {
			strcat(message, "SENSOR ERROR!!  ");
		}
		strcat(message, "\0");
	}

	ControllerModeBase::ModeState AutoMode::UpdateCurrentModeState(int userPreset, int offset)
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
			if (PressureSensorValue < (userPreset - offset) && !noSupply) { return ControllerModeBase::Supplying; }
			else if (PressureSensorValue >(userPreset + offset)) { return ControllerModeBase::Dumping; }
			else { return ControllerModeBase::InAcceptableRange; }
		}

	}

}