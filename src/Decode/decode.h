#ifndef _DECODE_COMMAND_H_
#define _DECODE_COMMAND_H_

#include "Cooking/CookingActionBlock.h"
#include "Arduino.h"

// макрос для преобразовывания символов в число (нужен для switch)
#define S(x) ((uint16_t)(((uint8_t)x[0] << 8) + ((uint8_t)x[1])))

// Float to Strinf  -   Конвертирование дроби в строку
String fToStr(float value);

// String to Float  -   Конвертирование строки в дробное число
float strToF(String str);

// Делит строку на подстроки через каждый split_char, и вызывает func на каждую полученную подстроку
void splitAndRunForEach(String input, char split_char, void (*func)(String));

// Декодирование одной общей команды
void oneCommandDecode(String input);

// Декодирование всей строки с общими командами
void decode(String input);

#endif