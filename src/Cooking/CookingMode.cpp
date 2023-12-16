#include "CookingMode.h"
#include "Hardware.h"
#include "PID.h"
#include "Decode/decodeActionBlock.h"
#include "Decode/commands.h"
#include "Settings.h"
#include "Temp.h"

//  BaseCookingMode
#pragma region BaseCookingMode
bool BaseCookingMode::isComplete() { return true; };
void BaseCookingMode::tick(ActionBlockWorks &works){};
void BaseCookingMode::start(){};
void BaseCookingMode::end(){};
String BaseCookingMode::string() { return "[]"; };
BaseCookingMode::~BaseCookingMode()
{
    // serialSend("CM deleted");
};

#pragma endregion

#pragma endregion

#pragma region ManualMode
ManualMode::ManualMode(String input)
{
    {

        int index = 0;

        while (index < input.length())
        {
            switch (input[index])
            {
            case 'T':
                bTen = true;
                bFans = true;
                break;

            case 'F':
                bFans = true;
                break;

            case 'A':
                bAir = true;
                break;

            case 'S':
                bSmoke = true;
                break;

            case 'W':
                bWater = true;
                break;
            };

            index++;
        }
    };
};
String ManualMode::string()
{
    String result = CM_MANUAL;

    if (bTen)
        result += "T";
    if (bFans)
        result += "F";
    if (bAir)
        result += "A";
    if (bSmoke)
        result += "S";
    if (bWater)
        result += "W";

    return result;
};
void ManualMode::tick(ActionBlockWorks &works){
   if (bTen)
        works.bTen = true;
    if (bFans)
        works.bFans = true;
    if (bAir)
        works.bAir = true;
    if (bSmoke)
        works.bSmoke = true;
    if (bWater)
        works.bWater= true;
};

#pragma endregion

//  EmptyMode
#pragma region EmptyMode

void EmptyMode::tick(ActionBlockWorks &works){};
String EmptyMode::string() { return CM_EMPTY; };

#pragma endregion


//  InfinityMode
#pragma region InfinityMode

InfinityMode::InfinityMode(String param)
{
    DecodeActionBloc decode(param);
    actionBlock = decode.getActionBlock();
};
void InfinityMode::tick(ActionBlockWorks &works)
{
    if (actionBlock != nullptr)
    {
        actionBlock->tick(works);
    };
};
bool InfinityMode::isComplete() { return false; };
String InfinityMode::string()
{
    String result = CM_INFINITY_MODE + String(CHAR_ACTION_INPUT);

    if (actionBlock != nullptr)
    {
        result += actionBlock->string();
    }

    return result;
}
InfinityMode::~InfinityMode()
{
    if (actionBlock != nullptr)
    {
        delete actionBlock;
    }
}
#pragma endregion

//  TimeMode
#pragma region TimeMode

DelayMode::DelayMode(String input)
{
    DecodeActionBloc decode(input);
    this->delayedStage = decode.getActionBlock();

    int8_t c = decode.param.indexOf(CHAR_NEXT_PARAM);
    if (c > 0)
    {
        seconds_delay = decode.param.substring(0, c).toInt();
        start_second = decode.param.substring(c + 1).toInt();
    }
    else
        seconds_delay = decode.param.toInt();
};
bool DelayMode::isComplete() { return (millis() - _timeStart) >= (seconds_delay * 1000); };
void DelayMode::end()
{
    bRun = false;
    if (delayedStage != nullptr)
        delayedStage->end();
};
void DelayMode::start()
{
    bRun = true;
    _timeStart = millis() - (start_second * 1000);
    delayedStage->start();
};
void DelayMode::tick(ActionBlockWorks &works)
{
    if (delayedStage != nullptr)
        delayedStage->tick(works);
};
String DelayMode::string()
{
    String result = CM_T_TIME + String(seconds_delay);

    if (bRun)
    {
        result += CHAR_NEXT_PARAM;
        result += (millis() - _timeStart) / 1000;
    }
    else if (start_second != 0)
    {
        result += CHAR_NEXT_PARAM;
        result += start_second;
    };

    result += CHAR_ACTION_INPUT;
    if (delayedStage != nullptr)
        result += delayedStage->string();

    return result;
};
DelayMode::~DelayMode()
{
    if (delayedStage != nullptr)
    {
        delete delayedStage;
    }
}

#pragma endregion


//  HeatBoxMode
#pragma region HeatBoxMode

HeatBoxMode::HeatBoxMode(String param)
{
    need_temp = strToF(param);
};
void HeatBoxMode::tick(ActionBlockWorks &works){
    works.bFans = true;
    works.bTen = Temp::box < need_temp;
};
String HeatBoxMode::string()
{
    return CM_HEAT_BOX + fToStr(need_temp);
}
bool HeatBoxMode::isComplete()
{
    return Temp::box >= need_temp;
}
#pragma endregion


//  HeatProductMode
#pragma region HeatProductMode

HeatProductMode::HeatProductMode(String param)
{
    int8_t c = param.indexOf(CHAR_NEXT_PARAM);
    
    if (c > 0)
    {
        need_temp_product = strToF(param.substring(0, c));
        need_temp_box = strToF(param.substring(c + 1));
    }
    else {
        need_temp_product = strToF(param);
        need_temp_box = need_temp_product + 5;
    };
};
void HeatProductMode::tick(ActionBlockWorks &works){
    works.bFans = true;
    works.bTen = Temp::box < need_temp_box;
};
String HeatProductMode::string()
{
    return CM_HEAT_PRODUCT + fToStr(need_temp_product) + String(CHAR_NEXT_PARAM) +  fToStr(need_temp_box);
}
bool HeatProductMode::isComplete(){return Temp::product >= need_temp_product;}
#pragma endregion

