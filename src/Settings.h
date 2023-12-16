#ifndef _ANDROID_GOLDENSMOKER_SETTINGS_
#define _ANDROID_GOLDENSMOKER_SETTINGS_

#define VERSION_PLATE 1

#include "Arduino.h"
#include "EEPROM.h"

//Структура коэффицентов PID
struct str_PIDkoef{
    float waterHeatProcent = 4;
    
    float HS_K2 = 4;

    float HF_K1 = 4;
    float HF_K2 = 6;

    float HA_Cooling_K1 = 3; 
    float HA_Cooling_K2 = 1;

    float CS_k1 = 4;
    float CS_k2 = 1; 
};

//Структура для калибровки датчика in - "сырых" значение с датчика, out - какое должно быть
struct str_Calibrate{
  float in, out;
};



struct str_settings {
  
  // Данные о калибровке молока
  str_Calibrate CalibrateProduct[2]= {{20, 20}, {90, 90}};

  // Данные о калибровке воды
  str_Calibrate CalibrateBox[2]= {{20, 20}, {90, 90}};

  //Значения необходимые для PID регулирования
  str_PIDkoef PID;
  
};

extern str_settings settings;

//Получить настройки находящиеся в EEPROM, true - если успешно
bool loadSettingsFromEEPROM();

//Записать текущие настройки в EEPROM
void updateSettingsInEEPROM();

// Макрос вывода значения переменной для отладки
#define Serial_DEBUG(x)      \
  do                         \
  {                          \
    Serial.print("DEBUG: "); \
    Serial.print(#x);        \
    Serial.print(" = ");     \
    Serial.println(x);       \
  } while (0)

// Макросс. Если число выходит за крайние границы приравниет к краям
#define RANGE(x, min, max) (((x) > (min)) ? (((x) < (max)) ? (x) : (max)) : (min))

/*  ____Регулирование мощности____
  переменная увеличивается на 1 каждые TIME_ONE_PERIOD_PID
  Если переменная больше нужной мощности ТЭН выключается, иначе включается

*/

// Время одного шага для регулирования мощности ТЭНа (мс)
#define PID_TIME_ONE_PERIOD 1500

// Максимальное количество регулирования
#define PID_COUNT_POWER_PERIOD 20

// Время отправки сообщений состояний всей системы
#define TIME_INFO 1000


#if (VERSION_PLATE == 1)

#define PIN_WATER_DETECT 2
#define PIN_AIR 16

#define PIN_TEMP_BOX 21
#define PIN_TEMP_PRODUCT 19

#define PIN_LAMP 32
#define PIN_COMPRESSOR 33
#define PIN_FANS 25
#define PIN_TEN_SMOKER 26
#define PIN_TEN_PAROGEN 27



#pragma endregion


#endif

#endif