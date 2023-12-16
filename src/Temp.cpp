#include "Temp.h"
#include "Decode/commands.h"
#include "Decode/decode.h"

// оригинальная функция map, переделанная под float. Нужна для калибровки датчика
float map_f(float x, float in_min, float in_max, float out_min, float out_max)
{
  const float run = in_max - in_min;
  if (run == 0)
  {
    return -1; // AVR returns -1, SAM returns 0
  }
  const float rise = out_max - out_min;
  const float delta = x - in_min;
  return (delta * rise) / run + out_min;
}


// CTemp Temp;

OneWire Temp::ds_product = OneWire(PIN_TEMP_PRODUCT);
OneWire Temp::ds_box = OneWire(PIN_TEMP_BOX);

float Temp::product = 0;
float Temp::box = 0;
float Temp::productRawData = 0;
float Temp::boxRawData = 0;

float Temp::getDallas(OneWire &ds)
{
  ds.reset();        // сброс шины
  ds.write(0xCC, 0); // пропуск ROM
  ds.write(0xBE, 0); // команда чтения памяти датчика
  uint8_t bufData[9];
  ds.read_bytes(bufData, 9); // чтение памяти датчика, 9 байтов

  delayMicroseconds(500);

  ds.reset();        // сброс шины
  ds.write(0xCC, 0); // пропуск ROM
  ds.write(0x44, 0); // инициализация измерения

  uint64_t stitch = ((bufData[1]) << 8) | bufData[0];

  // Если отрицательное, то переводим из дополнительного кода
  if (bufData[1] & B10000000)
  {
    stitch = (0xFFFF << 16) + stitch;
  }

  float temp = (float)(int)stitch * 0.0625 + 0.03125;

  int temp_i = (int)(temp * 100);

  if (temp_i % 10 >= 5)
    temp_i += 10;

  temp_i = temp_i / 10;

  temp = (float)temp_i / 10;

  return temp;
};

void Temp::updateTemp(bool guard)
{

  productRawData = getDallas(ds_product);
  product = map_f(productRawData, settings.CalibrateProduct[0].in, settings.CalibrateProduct[1].in, settings.CalibrateProduct[0].out, settings.CalibrateProduct[1].out);

  boxRawData = getDallas(ds_box);
  box = map_f(boxRawData, settings.CalibrateBox[0].in, settings.CalibrateBox[1].in, settings.CalibrateBox[0].out, settings.CalibrateBox[1].out);
};

void Temp::tick()
{
  static int64_t time_temp = millis();

  if (time_temp < millis())
  {
    time_temp = millis() + TIME_RESET_TEMP_SENSOR;

    updateTemp(false);
  };
};

void Temp::calibrate(str_Calibrate &Calibr, float inputTemp, float needTemp)
{
  Calibr.in = inputTemp;
  Calibr.out = needTemp;

  updateSettingsInEEPROM();
};

void Temp::calibrate(String input)
{
  String command = input.substring(0, 2);
  String param = input.substring(2);
  int8_t c = param.indexOf(CHAR_NEXT_PARAM);
  String param1;
  String param2;

  if (c > 0)
  {
    param1 = param.substring(0, c);
    param2 = param.substring(c + 1);
  }

  if (command.length() < 2)
    throw "Argument";

  switch (S(command))
  {
  case S("PL"):
    if (c > 0)
    {
      calibrate(settings.CalibrateProduct[0], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateProduct[0], Temp::productRawData, strToF(param));
    }
    break;

  case S("PH"):
    if (c > 0)
    {
      calibrate(settings.CalibrateProduct[1], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateProduct[1], Temp::productRawData, strToF(param));
    }
    break;

  case S("BL"):
    if (c > 0)
    {
      calibrate(settings.CalibrateBox[0], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateBox[0], Temp::boxRawData, strToF(param));
    }
    break;

  case S("BH"):
    if (c > 0)
    {
      calibrate(settings.CalibrateBox[1], strToF(param1), strToF(param2));
    }
    else
    {
      calibrate(settings.CalibrateBox[1], Temp::boxRawData, strToF(param));
    }
    break;
  };
};

void Temp::resetCalibrate()
{
  str_settings standart;

  settings.CalibrateProduct[0].in = standart.CalibrateProduct[0].in;
  settings.CalibrateProduct[0].out = standart.CalibrateProduct[0].out;
  settings.CalibrateProduct[1].in = standart.CalibrateProduct[1].in;
  settings.CalibrateProduct[1].out = standart.CalibrateProduct[1].out;

  settings.CalibrateBox[0].in = standart.CalibrateBox[0].in;
  settings.CalibrateBox[0].out = standart.CalibrateBox[0].out;
  settings.CalibrateBox[1].in = standart.CalibrateBox[1].in;
  settings.CalibrateBox[1].out = standart.CalibrateBox[1].out;

  updateSettingsInEEPROM();
};

String Temp::toString()
{
  String message;

  message += COMMAND_INFO_TEMP_PRODUCT;
  message += (int32_t)(product * 10);
  message += CHAR_SPLIT_COMMAND;
  message += COMMAND_INFO_TEMP_BOX;
  message += (int32_t)(box * 10);

  return message;
};

String Temp::stringCalibrate() {
      String str;
      str += "PH";
      str += fToStr(settings.CalibrateProduct[1].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateProduct[1].out);
      str += CHAR_SPLIT_COMMAND;

      str += "PL";
      str += fToStr(settings.CalibrateProduct[0].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateProduct[0].out);
      str += CHAR_SPLIT_COMMAND;

      str += "BH";
      str += fToStr(settings.CalibrateBox[1].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateBox[1].out);
      str += CHAR_SPLIT_COMMAND;

      str += "BL";
      str += fToStr(settings.CalibrateBox[0].in);
      str += CHAR_NEXT_PARAM;
      str += fToStr(settings.CalibrateBox[0].out);
      str += CHAR_SPLIT_COMMAND;

      return str;
}

String Temp::stringRaw(){
  String message;

  message += "P?";
  message += fToStr(productRawData);
  message += CHAR_SPLIT_COMMAND;
  message += "B?";
  message += fToStr(boxRawData);

  return message;
}