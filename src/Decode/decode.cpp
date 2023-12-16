#include "decode.h"
#include "Arduino.h"

#include "Hardware.h"
#include "Temp.h"
#include "commands.h"
#include "Cooking/PID.h"

#include "decodeActionBlock.h"
#include "info.h"

// Float to Strinf - Конвертирование дроби в строку
String fToStr(float value)
{
  return String((int)(value * 10));
}

// String to Float  -   Конвертирование строки в дробное число
float strToF(String str)
{
  return (float)(str.toInt()) / 10;
}

void decode(String input)
{
  input.toUpperCase();
  serialSend(">>" + input);
  splitAndRunForEach(input, CHAR_SPLIT_COMMAND, oneCommandDecode);
}

void splitAndRunForEach(String input, char split_char, void (*func)(String))
{
  int8_t search;
  String sup_s;

  do
  {
    search = input.indexOf(split_char);
    if (search == -1)
      sup_s = input;
    else
      sup_s = input.substring(0, search);
    func(sup_s);
    input = input.substring(search + 1);

  } while (search > -1);
}

// Декодирование одной общей команды
void oneCommandDecode(String input)
{
  try
  {

    char command[3];
    input.toCharArray(command, 3);
    String param = input.substring(2);

    switch (S(command))
    {

    case S(COMMAND_HARDWARE_LAMP_OFF):
      Hardware.lampWork(false);
      break;

    case S(COMMAND_HARDWARE_LAMP_ON):
      Hardware.lampWork(true);
      break;

    case S(COMMAND_CALIBRATE_STRING):
      Temp::calibrate(param);
      break;

    // Calibrate MILK HIGH
    case S(COMMAND_CALIBRATE_PRODUCT_HIGH):
      Temp::calibrate(settings.CalibrateProduct[1], Temp::productRawData, strToF(param));
      // serialSend("CalibrateMilk HIGH in :", String(t));
      break;

    // Calibrate MILK LOW
    case S(COMMAND_CALIBRATE_PRODUCT_LOW):
      Temp::calibrate(settings.CalibrateProduct[0], Temp::productRawData, strToF(param));
      // serialSend("Calibrate Milk LOW in :", String(t));
      break;

    // Calibrate WATER HIGH
    case S(COMMAND_CALIBRATE_BOX_HIGH):
      Temp::calibrate(settings.CalibrateBox[1], Temp::boxRawData, strToF(param));
      // serialSend("Calibrate Water HIGH in :", String(t));
      break;

    // Calibrate WATER LOW
    case S(COMMAND_CALIBRATE_BOX_LOW):
      Temp::calibrate(settings.CalibrateBox[0], Temp::boxRawData, strToF(param));
      // serialSend("Calibrate Water LOW in :", String(t));
      break;

    // Sensors Reset
    case S(COMMAND_CALIBRATE_RESET):
      Temp::resetCalibrate();
      // serialSend("Calibrate Sensors Reset");
      break;

    case S(COMMAND_CALIBRATE_WRITE):
      serialSend(Temp::stringCalibrate());
      break;
    // OK (Соеденение платы и Android успешно)
    case S("OK"):
      timeConnect = millis() + TIME_LOST_CONNECT;
      break;

    case S(COMMAND_INFO_RAW_TEMPERATURE):
      serialSend(Temp::stringRaw());
      break;

    case S(COMMAND_PID_WRITE):
      serialSend(PID.stringKoef());
      break;

    case S(COMMAND_PID_SETTINGS):
      PID.setKoef(param);
      break;

    // Сохранить коэффиценты PID в  EEPROM
    case S(COMMAND_PID_SAVE):
      updateSettingsInEEPROM();
      serialSend("PID koef save in EEPROM");
      break;

    // Сбросить коэффиценты
    case S(COMMAND_PID_RESET):
    {
      str_settings s1;
      settings = s1;
      // serialSend("PID koef reset");
    }
    break;

    // Вывести информацию
    case S(COMMAND_INFO_WRITE):
      printInfo();
      break;

    // Добавить этап под заданным номером, если не написать номер, то добавится в конец
    case S(COMMAND_RECIPE_ADD):
    {
      DecodeActionBloc decode(param);
      if (decode.param == "")
        RecipeController.addLast(decode.getActionBlock());
      else
        RecipeController.addIndex(decode.param.toInt(), decode.getActionBlock());
    }
    break;

    // Удалить этап под заданным номером, если не написать номер, то удалиться последний
    case S(COMMAND_RECIPE_SUB):
      if (param == "")
        RecipeController.deleteLast();
      else
        RecipeController.deleteIndex(param.toInt());
      break;

    // Изменить этап под заданным номером
    case S(COMMAND_RECIPE_INDEX):
    {
      DecodeActionBloc decode(param);
      RecipeController.changeIndex(decode.param.toInt(), decode.getActionBlock());
    }
    break;

    // Вывести все этапы рецепта
    case S(COMMAND_RECIPE_WRITE):
      RecipeController.printRecipe();
      break;

    // Очистить рецепт
    case S(COMMAND_RECIPE_CLEAR):
      RecipeController.clear();
      break;

    // Вывести количество этапов в рецепте
    case S(COMMAND_RECIPE_SIZE):
      serialSend(COMMAND_RECIPE_SIZE + String(RecipeController.size()));
      break;

    // Вывести текущий этап
    case S(COMMAND_RECIPE_COUNT):
      serialSend(COMMAND_RECIPE_COUNT + String(RecipeController.countStage));
      break;

    // Перейти на следующий этап
    case S(COMMAND_RECIPE_NEXT):
      RecipeController.nextStage();
      break;

    // Остановить приготовление
    case S(COMMAND_RECIPE_STOP):
      RecipeController.finishCooking();
      break;

    // Начать приготовление
    case S(COMMAND_RECIPE_START):
      RecipeController.startCooking();
      break;

    // Поставить приготовление на паузу
    case S(COMMAND_RECIPE_PAUSE):
      RecipeController.pause(true);
      break;

    // Возобновить приготовление
    case S(COMMAND_RECIPE_CONTINUE):
      RecipeController.pause(false);
      break;

    // Перейти к этапу под номером
    case S(COMMAND_RECIPE_NUMBER):
      RecipeController.setStage(param.toInt());
      break;

    // Войти в ручной режим
    case S(COMMAND_RECIPE_MANUAL):
    {
      DecodeActionBloc decode(param);
      RecipeController.manualStart(decode.getActionBlock());
    };
    break;

    // Выйти из режима ручного управления
    case S(COMMAND_RECIPE_AUTO):
      RecipeController.manualDisable();
      break;

    default:
      throw "Unknown command";
    };
  }
  catch (const char *error_message)
  {
    serialSend("ER: " + String(error_message) + " : " + input);
  }
  catch (...)
  {
    serialSend("ER : Unknown Error " + input);
  };
};
