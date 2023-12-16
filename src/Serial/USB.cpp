#include "Serial/USB.h"
#include "Bluetooth/Bluetooth.h"
#include "Decode/commands.h"
#include "Temp.h"

uint64_t timeConnect = 0;

void serialSend(String s1, String s2)
{
  Serial.println(s1 + s2);
  bluetothSend(s1 + s2);
}

void checkSerial()
{
  if (Serial.available() == 0)
    return;
  String data;
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c != '\n')
      data += c;
    // чтобы не пропустить символы, без него были ложные деления на подстроки
    delayMicroseconds(100);
  }

  // serialSend(">>", data);
  decode(data);
}
