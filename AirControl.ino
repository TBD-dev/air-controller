/*
 Name:		AirControl.ino
 Created:	11/29/2015 2:20:40 PM
 Author:	Scotty
*/

// the setup function runs once when you press reset or power the board
#include "EEPROM.h"
#include "LiquidCrystal.h"
#include "AirController.h"
using MADAutoDev::AirController;

AirController _AirController(3, 2, 1);


void setup() {
	_AirController.Setup();
}

// the loop function runs over and over again until power down or reset
void loop() {
	_AirController.ProcessButtonClicks(0);
	_AirController.ExecuteControllerLogic();
}
