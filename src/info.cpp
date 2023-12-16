#include "info.h"

#include "Settings.h"
#include "Decode/commands.h"
#include "Decode/decode.h"
#include "Temp.h"
#include "Cooking/CookingController.h"
#include "Hardware.h"

void infoTick()
{
  static uint32_t tim = millis();
  if (millis() >= tim)
  {
    tim = millis() + 1000;
    printInfo();
  }
}

// Вывести информацию о датчиках и кнопках
void printInfo()
{
  // Выводим температуру
  String message = Temp::toString();
  
  message += CHAR_SPLIT_COMMAND;

  // Включено ли освещение   
  message += Hardware.isLampWork() ? COMMAND_HARDWARE_LAMP_ON : COMMAND_HARDWARE_LAMP_OFF;   

  message += CHAR_SPLIT_COMMAND;

  // Есть ли вода в парогенераторе
  message += Hardware.isSteamHaveWater() ? COMMAND_INFO_WATER_FULL : COMMAND_INFO_WATER_ERROR; 

  message += CHAR_SPLIT_COMMAND;

  // Выводим строчку рецепта
  message += RecipeController.toString();

  serialSend(message);
}
