#pragma once
#include "string.h"

namespace MADAutoDev
{
	class IControllerMode
	{
	public:
		virtual ~IControllerMode() {}
		char ModeName[16];
		virtual void Logic(int sensorPins[], int userPreset, int offset, char *message) = 0;
	};
}