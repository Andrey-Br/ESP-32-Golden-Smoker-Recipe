#include "Hardware.h"

#include "Decode/commands.h"
#include "Serial/USB.h"

CHardware Hardware;

void CHardware::init(){
  pinMode(PIN_LAMP, OUTPUT);
  pinMode(PIN_COMPRESSOR, OUTPUT);
  pinMode(PIN_FANS, OUTPUT);
  pinMode(PIN_TEN_SMOKER, OUTPUT);
  pinMode(PIN_TEN_PAROGEN, OUTPUT);
  pinMode(PIN_AIR, OUTPUT);

  pinMode(PIN_WATER_DETECT,INPUT);

};

// Функция сбрасывающая состояние при первом не подоходящем сигнале на milliseconds
bool bounce(bool now ,bool need, uint32_t milliseconds = 2000) {
  static uint32_t tim = millis();
  if (now != need) {
    tim = millis();
    return now;
  } else if (millis() - tim > milliseconds) {
    return now;
  }

  return !now;
}

void CHardware::tick(){
  bool bNewWaterInParogen = bounce(_isSteamHaveWater(), true);
  if (_bWaterInParogen != bNewWaterInParogen ) {
    _bWaterInParogen = bNewWaterInParogen;
    serialSend(_bWaterInParogen ? COMMAND_INFO_WATER_FULL : COMMAND_INFO_WATER_ERROR);
  }
};

// TODO: Убрать инверсию датчика уровня воды (Сделанно только для тестов)
bool CHardware::_isSteamHaveWater(){
  return !digitalRead(PIN_WATER_DETECT);
};

//  Есть ли вода в парогенераторе
bool CHardware::isSteamHaveWater(){
  return _bWaterInParogen;
};

void CHardware::tenWork(bool enable)
{
  digitalWrite(PIN_TEN_SMOKER, enable);
};

// Выключить все системы
void CHardware::disableAll()
{
  tenWork(false);
  waterWork(false);
  smokeWork(false);
  fansWork(false);
  airWork(false);
};

//  Вентиялторы
void CHardware::fansWork(bool enable){
  digitalWrite(PIN_FANS, enable);
};

//  Парогенератор
void CHardware::waterWork(bool enable)
{
  digitalWrite(PIN_TEN_PAROGEN, enable && isSteamHaveWater());
};

//  Дымогенератор
void CHardware::smokeWork(bool enable){
  digitalWrite(PIN_COMPRESSOR, enable);
};

//  Заслонки
void CHardware::airWork(bool enable){
  digitalWrite(PIN_AIR, enable);
};

// Работает ли лампа 
  bool CHardware::isLampWork(){
    return digitalRead(PIN_LAMP);
  };

// Включить/ Выключить лампу
  void CHardware::lampWork(bool enable){ 
    digitalWrite(PIN_LAMP, enable);
  };

