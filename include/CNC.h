#ifndef _CNC_H_
#define _CNC_H_

#include "Arduino.h"

void cncInit();
void cncRun(uint32_t now=0);
void cncJogCurrentAxis(int32_t steps);
void cncChangeStep(int32_t steps);
void cncChangeSpeed(int32_t steps);

typedef enum CncAxis {CNC_AXIS_NONE, CNC_AXIS_X, CNC_AXIS_Y, CNC_AXIS_Z, CNC_AXIS_R, CNC_AXIS_MAX} CncAxis;

#define CNC_RUN_MS 10

#endif // _BATTERY_H_