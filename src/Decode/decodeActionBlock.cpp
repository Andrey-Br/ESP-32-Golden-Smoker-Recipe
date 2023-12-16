#include "Settings.h"
#include "decodeActionBlock.h"

#include "Decode/commands.h"
#include "Decode/decode.h"

DecodeActionBloc::DecodeActionBloc(String input)
{
  int countInputChar = input.indexOf(CHAR_ACTION_INPUT);

  if (countInputChar < -1)
  {
    param = input;
  }
  else
  {
    param = input.substring(0, countInputChar);                             // input.substring(0, countInputChar - 1);
    _actionBlock = generateActionBlock(input.substring(countInputChar + 1)); // String s2 = input.substring(countInputChar + 1);
    };
};

ActionBlock* DecodeActionBloc::generateActionBlock(String input)
{

  // Посимвольно проходим, ищем ключевые символы и декодируем

  int8_t search = 0;
  // String sup_s;

  ActionBlock* AB = new ActionBlock();
  
  while (search < input.length())
  {

    if (input[search] == CHAR_ACTION_SPLIT)
    {
      AB->add(actionCommandDecode(input.substring(0, search)));
      input = input.substring(search + 1);
      search = 0;
    }

    else if (input[search] == CHAR_ACTION_INPUT)
    {
      AB->add(actionCommandDecode(input));
      return AB;
    }

    else
      search++;
  }

  AB->add(actionCommandDecode(input));
  return AB;
};

DecodeActionBloc::~DecodeActionBloc(){
  if (!_getActionBlock) {
    if (_actionBlock != nullptr) {
      delete _actionBlock;
    }
  }
};

ActionBlock* DecodeActionBloc:: getActionBlock() {
    _getActionBlock = true;
    
    if (_actionBlock == nullptr) {
      _actionBlock = new ActionBlock();  
      _actionBlock->add(new EmptyMode());
    }

    return _actionBlock;
  }

// ###################################################################################################
// ###################################################################################################
// ###################################################################################################
// ###################################################################################################
// ###################################################################################################

//TODO: Декодоривание одной этапной команды
BaseCookingMode*  DecodeActionBloc::actionCommandDecode(String input)
{

  if (input.length() < 2) {
    return new EmptyMode();
  };

  char command[3];
  input.toCharArray(command, 3);
  String param = input.substring(2);

  // Ст
  switch (S(command))
  {

  case S(CM_MANUAL):
      return new ManualMode(param);
    break;

  case S(CM_INFINITY_MODE):
    return new InfinityMode(param);
    break;

  case S(CM_T_TIME):
    return new DelayMode(param);
    break;

  case S(CM_HEAT_BOX):
    return new HeatBoxMode(param);
    break;

  case S(CM_HEAT_PRODUCT):
    return new HeatProductMode(param);
    break;

  case S(CM_EMPTY):
    return new EmptyMode();
    break;

  default:
      throw "Unknown mode";
  }
};


