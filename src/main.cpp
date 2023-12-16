#ifndef _ANDROID_GOLDENSMOKER_MAIN_
#define _ANDROID_GOLDENSMOKER_MAIN_

#include "Serial/USB.h"
#include "Bluetooth/Bluetooth.h"
#include "Temp.h"
#include "Hardware.h"
#include "info.h"
#include "Cooking/CookingController.h"
#include "EEPROM.h"

void setup()
{
  bluetothInit();
  
  EEPROM.begin(255);

  Serial.begin(115200);

  Hardware.init();

}

void loop()
{
  Temp::tick();
  Hardware.tick();

  bluetothTick();
  checkSerial();

  RecipeController.tick();

  infoTick();
}

#endif