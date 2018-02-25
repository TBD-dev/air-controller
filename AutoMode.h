#pragma once
#include "IControllerMode.h"
#include "ControllerModeBase.h"

namespace MADAutoDev
{
	class AutoMode :
		public ControllerModeBase, public IControllerMode
	{
	public:
		int timer = 0;
		bool noSupply = true;
		AutoMode(int dumpValvePin, int supplyValvePin);
		~AutoMode();
		void Logic(int sensorPins[], int userDefault, int offset, char *message);
		ControllerModeBase::ModeState UpdateCurrentModeState(int userPreset, int offset);

	};
}