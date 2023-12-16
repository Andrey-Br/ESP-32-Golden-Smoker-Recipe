#ifndef _DECODE_ACTION_BLOCK_H_
#define _DECODE_ACTION_BLOCK_H_

#include "Cooking/CookingController.h"

//Класс который декодирует этап приготовления
class DecodeActionBloc {
private:

  // Декодоривание одной этапной команды 
  BaseCookingMode * actionCommandDecode(String input);

  //Генерирует и возвращает указатель на объект блока рецепта
  ActionBlock *generateActionBlock(String input);

  //Сгенерированный ActionBlock
  ActionBlock* _actionBlock = nullptr;
  
  // Забрали ли блок, если нет, то удалим его в деструкторе
  bool _getActionBlock = false;

public:
  DecodeActionBloc(String input);
  ~DecodeActionBloc();
  
  ActionBlock* getActionBlock();

  // Параметр который 
  String param = "";

};


#endif
