#include "Cooking/CookingActionBlock.h"
#include "Decode/commands.h"
#include "Hardware.h"

ActionBlock::ActionBlock()
{

}

void ActionBlock::start()
{
  for (int i = 0; i<cookingModeArray.length(); i++) {
    cookingModeArray[i].start();
  }
}

void ActionBlock::end() {
  for (int i = 0; i<cookingModeArray.length(); i++) {
    cookingModeArray[i].end();
  }
}

void ActionBlock::tick()
{
  works.clear();

  for (int i = 0; i < cookingModeArray.length() ; i++) {
    cookingModeArray[i].tick(works);
  }

  _totalWork();
}

void ActionBlock::tick(ActionBlockWorks works)
{
  for (int i = 0; i < cookingModeArray.length() ; i++) {
    cookingModeArray[i].tick(works);
  }
}

void ActionBlock::_totalWork() {
  Hardware.tenWork(works.bTen);
  Hardware.fansWork(works.bFans);
  Hardware.smokeWork(works.bSmoke);
  Hardware.waterWork(works.bWater);
  Hardware.airWork(works.bAir);
};

bool ActionBlock::isComplete()
{
  bool result = true;

  for (int i = 0; i < cookingModeArray.length() ; i++) {
    result = result && cookingModeArray[i].isComplete();
  }

  return result;
};

ActionBlock::~ActionBlock()
{
  cookingModeArray.clear();
}

String ActionBlock::string()
{
  // int count = cookingModeArray.length();
  
  // serialSend("count init");
  // serialSend(String(count));//, String(cookingModeArray.length()));

  // serialSend(String(_cookingModeArray.length()));
  // serialSend(_cookingModeArray[0].string(0));

  // _cookingModeArray.addFirst(new EmptyMode());
  


  String result = "";

  for (int i = 0 ; i < cookingModeArray.length(); i++) {
  // result += "a"; 
    result+= cookingModeArray[i].string();

    // Если еще будут команды добавляем символ
    if (i < cookingModeArray.length() - 1) {
      result += CHAR_ACTION_SPLIT;
    }
  }
  
  return result;
};

//  Добавить действие
void ActionBlock::add(BaseCookingMode* cookingMode){  
  if (cookingMode != nullptr) {
    cookingModeArray.addLast(cookingMode);
  }
};
