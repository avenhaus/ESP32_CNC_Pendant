#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "Arduino.h"

// Green LED is used to drop battery voltage by 1.685V (measured across LED) so we can safely measure up to 5V.
#ifndef BATTERY_LED_DROP_V
#define BATTERY_LED_DROP_V 1.685
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