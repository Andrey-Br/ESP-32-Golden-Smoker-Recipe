#include "CookingController.h"
#include "Arduino.h"
#include "Hardware.h"
#include "Decode/commands.h"
#include "CookingActionBlock.h"

// Объект контроллера, с помощью которого производим все манипуляции связанные с приготовлением по рецепту
CRecipeController RecipeController;

// ####################################################################################################
// ####################################################################################################
// ####################################################################################################

void CRecipeController::finishCooking()
{
  bAutoCooking = false;
  countStage = 0;
  serialSend(COMMAND_RECIPE_FINISH);
  Hardware.disableAll();
};

void CRecipeController::startCooking()
{
  manualDisable();

  if (arrStage.length() == 0)
  {
    serialSend(COMMAND_RECIPE_SIZE + String("0"));
    bAutoCooking = false;
    return;
  };

  bAutoCooking = true;
  bPause = false;
  countStage = 0;
  serialSend(COMMAND_RECIPE_START);
  setStage(countStage);
};

void CRecipeController::setStage(int index)
{
  if ((countStage < arrStage.length()) && (countStage >= 0)) arrStage[countStage].end();
  
  if (index >= arrStage.length())
  {
    finishCooking();
    return;
  };

  serialSend(COMMAND_RECIPE_NUMBER + String(index));
  countStage = index;
  if (bAutoCooking && !bPause)
    arrStage[countStage].start();
};

void CRecipeController::nextStage()
{
  // serialSend("R^");
  setStage(countStage + 1);
};

void CRecipeController::pause(bool bEnable)
{
  bPause = bEnable;

  if (bPause)
  {
    Hardware.disableAll();
  }
  else
  {
    manualDisable();
    // setStage(countStage);
  }
};

void CRecipeController::tick()
{
  // Если не автоматическое приготовление или пауза, то переходим в ручной режим
  if (!bAutoCooking || (bAutoCooking && bPause))
    return manualMode();

  arrStage[countStage].tick();
  if (arrStage[countStage].isComplete())
    nextStage();
};

void CRecipeController::addLast(ActionBlock *actionBlock)
{
  arrStage.addLast(actionBlock);
};

void CRecipeController::deleteLast()
{
  deleteIndex(size() - 1);
}

void CRecipeController::addIndex(int index, ActionBlock *actionBlock)
{
  arrStage.addTo(index, actionBlock);

  // Если добавили перед текущим этапом, то текущий этап сместился, поэтому поправляем
  if (index <= countStage)
    countStage++;
};

void CRecipeController::deleteIndex(int index)
{
  arrStage.deleteFrom(index);

  // Если удалили текущий этап, следующий сместился и встал на сесто текущего, поэтому просто стартуем его
  if (index == countStage)
    setStage(countStage);

  // Если удалили перед текущим этапом, то нужный этап сместился, поэтому смещаем и курсор
  if (index < countStage)
    countStage--;
};

void CRecipeController::changeIndex(int index, ActionBlock *newActionBlock)
{
  arrStage.change(index, newActionBlock);
};

void CRecipeController::clear()
{
  if (bAutoCooking){
    finishCooking();
  };
  arrStage.clear();
};

uint8_t CRecipeController::size()
{
  return arrStage.length();
};

void CRecipeController::printRecipe()
{

  String msg = COMMAND_RECIPE_WRITE;

  for (int i = 0; i < arrStage.length(); i++)
  {
    msg += String(CHAR_SPLIT_COMMAND);
    msg += COMMAND_RECIPE_INDEX + String(i) + String(CHAR_ACTION_INPUT) + arrStage[i].string();
  };
  
  serialSend(msg);
};

// Начать приготовление вне рецепта
void CRecipeController::manualStart(ActionBlock *manual)
{
  if (bAutoCooking && !bPause)
  {
    if (manual != nullptr)
      delete manual;
    bManualModeEnable = false;
    return;
  };

  bManualModeEnable = true;

  if (manualAction != nullptr)
    delete manualAction;

  manualAction = manual;
  manualAction->start();
}

void CRecipeController::manualMode()
{
  if (bManualModeEnable && manualAction != nullptr)
  {
    manualAction->tick();
  } else Hardware.disableAll();
};

void CRecipeController::manualDisable()
{
  bManualModeEnable = false;

  if (manualAction != nullptr)
  {
    delete manualAction;
    manualAction = nullptr;
  };
};

String CRecipeController::stringCurrentMode()
{
  return arrStage[countStage].string();
}

String CRecipeController::toString(){
  String str = "";

  str += COMMAND_RECIPE_SIZE;
  str += RecipeController.size();
  str += CHAR_SPLIT_COMMAND;

  if (RecipeController.bAutoCooking)
  {
    str += COMMAND_RECIPE_START;
    str += CHAR_SPLIT_COMMAND;

    str += COMMAND_RECIPE_COUNT;
    str += RecipeController.countStage;
    str += CHAR_SPLIT_COMMAND;

    if (RecipeController.bPause)
    {
      str += COMMAND_RECIPE_PAUSE;
      str += CHAR_SPLIT_COMMAND;
    };

    str += RecipeController.stringCurrentMode();
  }
  else
  {
    str += COMMAND_RECIPE_FINISH;
  }

  if (((RecipeController.bAutoCooking == false) || (RecipeController.bPause == true)) && RecipeController.bManualModeEnable)
  {
    str += CHAR_SPLIT_COMMAND;

    str += COMMAND_RECIPE_MANUAL;
    str += CHAR_ACTION_INPUT;
    if ( RecipeController.manualAction != nullptr ) str += RecipeController.manualAction->string();

  }

  return str;
}
