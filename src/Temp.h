#ifndef _ANROID_GOLDENSMOKER_TEMP_H
#define _ANROID_GOLDENSMOKER_TEMP_H

#include <OneWire.h>

#include "Settings.h"
#include "Serial/USB.h"

//С какой периодичностью оправшиваем датчики температуры
#define TIME_RESET_TEMP_SENSOR 1000

// //Структура для калибровки датчика in - "сырых" значение с датчика, out - какое должно быть
// struct str_Calibrate{
//   float in, out;
// };


// Класс для взаимодействия с температурами
class Temp {
private:
  //  Получить температуру от датчика
  static float getDallas(OneWire &ds);
  //  Обновить температуры, false Если без защиты от помех
  static void updateTemp(bool guard = true);
  //  Объект датчика молока 
  static  OneWire ds_product;
  //  Объект датчика воды
  static OneWire ds_box;

public:
  //  Откалибровання температура молока
  static float product;
  //  Откалибровання температура воды
  static float box;
  //  Неоткалиброванная температура с датчика молока
  static float productRawData;
  //  Неоткалиброванная температура с датчика воды
  static float boxRawData;
  //  Обновление данных по истечению времени
  static void tick();
  //  Перезаписать структуру калибровки
  static void calibrate(str_Calibrate &Calibr, float inputTemp, float needTemp);
  //  Декодирование и перезапись структуры калибровки
  static void calibrate(String input);
  //  Сбросить калиброку на стандартную
  static void resetCalibrate();
  //  Преобразование температур в строку
  static String toString();
  //  Строка значений калибровки
  static String stringCalibrate();
  //  Вывести сыре данные с датчков
  static String stringRaw();
};

#endif