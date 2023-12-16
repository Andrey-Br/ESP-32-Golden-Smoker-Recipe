#ifndef _ANDROID_GOLDENSMOKER_BLUETOOTH_H_
#define _ANDROID_GOLDENSMOKER_BLUETOOTH_H_

#include "Decode/decode.h"

// #define _ANDROID_GOLDENSMOKER_USE_BLE_

#ifdef _ANDROID_GOLDENSMOKER_USE_BLE_

  #include "BLE.h"

#else // #ifdef _ANDROID_CHEESE_USE_BLE_

  #include "Bluetooth_Classic.h" 

#endif    // #ifdef _ANDROID_CHEESE_USE_BLE_

#endif