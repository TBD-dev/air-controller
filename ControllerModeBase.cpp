#include "ControllerModeBase.h"

namespace MADAutoDev
{
	ControllerModeBase::ControllerModeBase(int dumpValvePin, int supplyValvePin)
	{
		DumpValvePin = dumpValvePin;
		SupplyValvePin = supplyValvePin;
	}

	ControllerModeBase::~ControllerModeBase()
	{
	}
}