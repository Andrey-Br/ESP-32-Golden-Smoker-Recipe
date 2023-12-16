#ifndef _COOKING_CONTROLLER_H_
#define _COOKING_CONTROLLER_H_

#include "dynamicArray.h"
#include "CookingMode.h"
#include "CookingActionBlock.h"

class BaseCookingMode;

class ActionBlock;

/* Представляет из себя механизм управления приготовлением рецепта. 
В нем хранятся все этапы приготовления (ActionBlock), он позволяет добавлять и удалять этапы.
Когда запущен, он автоматически включает нужный этап, и меняет его при завершении
*/ 
class CRecipeController
{
public:
  // Включен ли ручной режим 
  bool bManualModeEnable = false;

  // Действие в ручном режиме 
  ActionBlock * manualAction = nullptr;

  // BaseCookingMode* manualMode; 
  // Динамический массив этапов приготовления
  DynamicArray<ActionBlock> arrStage;
  // Активированно ли приготовление по рецепту
  bool bAutoCooking = false;
  // Поставлена ли пауза в приготовлении
  bool bPause = false;
  // Текущий этап приготолвния  
  int countStage = 0;

  // Закончить процесс приготовления. Выключает все системы. И сообщает о завершении
  void finishCooking();

  // Начать процесс приготовления с самого первого этапа. 
  void startCooking();

  // Перейти к определенному этапу приготовления 
  void setStage(int index);

  // Перейти на следующий этап приготовления
  void nextStage();

  // Приостановить процесс приготовления рецепта
  void pause(bool bEnable);

  // Вызывает функцию выполнения у текущего этапа приготовления 
  void tick();

  // Добавляет в конец рецепта этап
  void addLast(ActionBlock *actionBlock);

  // Удаляет последний этап из рецепта
  void deleteLast();

  // Добавить элемент на место index
  void addIndex(int index, ActionBlock *actionBlock);

  // Удалить элемент под номером
  void deleteIndex(int index);

  // Изменить элемент под номером index
  void changeIndex(int index, ActionBlock *newActionBlock);

  // Вывести весь рецепт
  void printRecipe();
  
  // Количество этапов
  uint8_t size();

  // Очистить все этапы рецепта
  void clear();

  // Начать приготовление вне рецепта
  void manualStart(ActionBlock *manual);

  // Ручной режим
  void manualMode();

  // Отключить все в автоматическом режиме 
  void manualDisable();

  // Вывести строчку текущего этапа
  String stringCurrentMode();

  // Строка состояния рецепта
  String toString();
};

// Объект контроллера, с помощью которого производим все манипуляции связанные с приготовлением по рецепту
extern CRecipeController RecipeController;

#endif
