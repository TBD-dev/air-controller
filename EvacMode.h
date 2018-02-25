#pragma once
#include "IControllerMode.h"
#include "ControllerModeBase.h"

namespace MADAutoDev
{
	class EvacMode :
		public ControllerModeBase, public IControllerMode
	{
	public:
		int timer = 0;
		EvacMode(int dumpValvePin, int supplyValvePin);
		~EvacMode();
		void Logic(int sensorPins[], int userDefault, int offset, char *message);
		ControllerModeBase::ModeState UpdateCurrentModeState(int userPreset, int offset);
	};
}
