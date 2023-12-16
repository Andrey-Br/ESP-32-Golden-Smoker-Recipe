/*
  В этом файле описаны функции для работы с датчиками и для приготовления
*/

#ifndef _ANDROID_GOLDENSMOKER_COOCKIN_H_
#define _ANDROID_GOLDENSMOKER_COOCKIN_H_

#include "Serial/USB.h"
#include "Settings.h"

// Класс для управления аппаратной частью нагрева / охлажденния
class CHardware
{
private:
  // Состояние, есть ли вода в парагенораторе 
  bool _bWaterInParogen = false;

  //  Считывание датчика уровня воды в парогнереторе 
  bool _isSteamHaveWater();

public:
  // Настроить пины 
  void init();

  // Проверка датчиков
  void tick();

  // Работает ли лампа 
  bool isLampWork();

  // Включить/ Выключить лампу
  void lampWork(bool enable);

  //  Есть ли вода в парогенераторе
  bool isSteamHaveWater();

  //  Вентиялторы
  void fansWork(bool enable);

  //  Парогенератор
  void waterWork(bool enabe);

  //  Дымогенератор
  void smokeWork(bool enable);

  //  ТЭН
  void tenWork(bool enable);

  //  Заслонки
  void airWork(bool enable);

  // Выключить все системы
  void disableAll();
};

extern CHardware Hardware;


#endif