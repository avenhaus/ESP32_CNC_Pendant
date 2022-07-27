#ifndef _CNC_H_
#define _CNC_H_

#include "Arduino.h"

void cncInit();
void cncRun(uint32_t now=0);
void cncIncFeed(int32_t steps);
void cncIncJogFeed(int32_t steps);
void cncIncJogStep(int32_t steps);
void cncJogAxis(int32_t steps);

/* ============================================== *\
 * Axis
\* ============================================== */

typedef enum CncAxisEnum {CNC_AXIS_NONE, CNC_AXIS_X, CNC_AXIS_Y, CNC_AXIS_Z, CNC_AXIS_A, CNC_AXIS_B, CNC_AXIS_C, CNC_AXIS_MAX} CncAxisEnum;

class CncAxis {
public:
    CncAxis(const char* groupName, CncAxisEnum axis, char letter, float defaultFeed, float defaultJogFeed, float defaultJogStep);
    void jog(int steps);
    float incFeed(int32_t steps);
    float incJogFeed(int32_t steps);
    float incJogStep(int32_t steps);

    CncAxisEnum axis;
    char letter;
    RegGroup configGroup;
    ConfigFloat feed;
    ConfigFloat jogFeed;
    ConfigFloat jogStep;
    StateFloat machinePos;
    StateFloat workCoordinate;
};

/* ============================================== *\
 * Constants
\* ============================================== */

#define CNC_RUN_MS 10

#endif // _BATTERY_H_