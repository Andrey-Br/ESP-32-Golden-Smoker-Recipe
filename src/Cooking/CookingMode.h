#ifndef _COOKING_MODE_H_
#define _COOKING_MODE_H_

#include "Arduino.h"
#include "CookingController.h"
#include "CookingActionBlock.h"
#include "Decode/decode.h"

// Сообщаем что где то будут эти классы
class ActionBlock;
class ActionBlockWorks;

// Базовый класс режима нагрева/охлаждения, от него наследуюся все режимы
class BaseCookingMode
{
public:
  virtual bool isComplete();
  virtual void tick(ActionBlockWorks & works);
  virtual void start();
  virtual String string();
  virtual void end();
  virtual ~BaseCookingMode();
};


//  Ручной режим
#define CM_MANUAL "M!"
class ManualMode : public BaseCookingMode
{
private:
  bool bTen = false;
  bool bFans = false;
  bool bAir = false;
  bool bSmoke = false;
  bool bWater = false;

public:
  ManualMode(String input);
  void tick(ActionBlockWorks & works) override;
  String string() override;
};

//  Пустой режим
#define CM_EMPTY "ME"
class EmptyMode : public BaseCookingMode
{
  void tick(ActionBlockWorks & works) override;
  String string() override;
};

#define CM_INFINITY_MODE "MI"
class InfinityMode : public BaseCookingMode {
  ActionBlock* actionBlock;
public:
  InfinityMode(String param);
  bool isComplete() override;
  String string() override;
  void tick(ActionBlockWorks & works) override;
  ~InfinityMode();
};

//  Выдержка по времени
#define CM_T_TIME "MT"
class DelayMode : public BaseCookingMode
{
private:
  bool bRun = false;
  uint32_t _timeStart;
  uint32_t start_second = 0;

public:
  // DelayMode(ActionBlock *delayedStage, int seconds_delay = 0);
  DelayMode(String input);
  ActionBlock *delayedStage;
  uint32_t seconds_delay;
  bool isComplete() override;
  void tick(ActionBlockWorks & works) override;
  void start() override;
  String string() override;
  void end () override;
  ~DelayMode();
};

//  Нагрев камеры
#define CM_HEAT_BOX "MB"
class HeatBoxMode : public BaseCookingMode {
  float need_temp = 0;
public:
  HeatBoxMode(String param);
  bool isComplete() override;
  String string() override;
  void tick(ActionBlockWorks & works) override;
};

//  Нагрев продукта
#define CM_HEAT_PRODUCT "MP"
class HeatProductMode : public BaseCookingMode {
  float need_temp_product = 0;
  float need_temp_box = 0;
public:
  HeatProductMode(String param);
  bool isComplete() override;
  String string() override;
  void tick(ActionBlockWorks & works) override;
};


#endif
