#ifndef _GOLDENSMOKER_COOKING_ACTION_BLOC_
#define _GOLDENSMOKER_COOKING_ACTION_BLOC_

#include "CookingMode.h"
#include "dynamicArray.h"

class BaseCookingMode;

//  Что именно будет работать в текущем этапе
class ActionBlockWorks {
  public:
    bool bTen = false;
    bool bFans = false;
    bool bWater = false;
    bool bSmoke = false;
    bool bAir = false;

  void clear(){
    bTen = false;
    bFans = false;
    bWater = false;
    bSmoke = false;
    bAir = false;
  } 
};

/* Представляет из себя один этап приготовления рецепта, в него входят 
выбранный режим Нагрева/Охлаждения. И настройки мешалки которые нужно применить
*/
class ActionBlock
{
private:
  // Если _cookingMode = NULL, ничего не будет происходить, сразу будет выдавать что завершен
  // BaseCookingMode *_cookingMode = nullptr;

  DynamicArray<BaseCookingMode> cookingModeArray;

  // Итого что включено и что выключенно
  void _totalWork();

  //  Что в итоге будет работать
  ActionBlockWorks works;

public:
  ActionBlock();
  // Вызывается при переходе на этот этап рецепта (Позволяет режимам нагрева применить первоначальные параметры + изменяет параметры мешалки если надо)
  void start();

  // Котроллер вызывает этот метод при завершении этапа
  void end();

  // Вызывает метод tick у режма нагрева, а тот уже решает что делать с ТЭНами / Клапаном
  void tick();

  // Вызывает метод tick у режма нагрева, а тот уже решает что делать с ТЭНами / Клапаном
  void tick(ActionBlockWorks works);

  // Возвращет завершен ли текущий этап
  bool isComplete();

  //  Добавить действие
  void add(BaseCookingMode* cookingMode);

  String string();

  ~ActionBlock();
};


#endif
