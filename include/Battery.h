#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "Arduino.h"

#ifndef BATTERY_CONV_FACTOR
#define BATTERY_CONV_FACTOR ((56.0 + 33.0) / 56.0)
#endif

#ifndef BATTERY_CELLS
#define BATTERY_CELLS 1
#endif

#ifndef BATTERY_READ_MS
#define BATTERY_READ_MS 200
#endif 

void adcInit();
void batteryRun(uint32_t now=0);

extern float batteryVoltage;
extern float batteryVoltageFiltered;
extern int batteryChargeLevel;

#endif // _BATTERY_H_