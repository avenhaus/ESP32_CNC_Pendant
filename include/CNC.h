#ifndef _CNC_H_
#define _CNC_H_

#include "Arduino.h"


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
    void showCoordinates();

    CncAxisEnum axis;
    char letter;
    RegGroup configGroup;
    ConfigFloat feed;
    ConfigFloat jogFeed;
    ConfigFloat jogStep;
    StateFloat machinePos;
    StateFloat workCoordinate;
};

void cncInit();
void cncRun(uint32_t now=0);
CncAxisEnum cncIncActiveAxis(int32_t steps);
void cncIncFeed(int32_t steps);
void cncIncJogFeed(int32_t steps);
void cncIncJogStep(int32_t steps);
void cncJogAxis(int32_t steps);

void cncAxisEncoderPress();


typedef enum CncSettingsEncoderMode { CSEM_NONE, CSEM_JOG_STEP, CSEM_JOG_FEED, CSEM_FEED_OVERRIDE, CSEM_RAPIDS_OVERRIDE, CSEM_SPEED_OVERRIDE, CSEM_MAX } CncSettingsEncoderMode;
void cncIncSettingsEncoder(int32_t steps);
void cncChangeSettingsEncoderMode(int32_t steps);

/* ============================================== *\
 * Constants
\* ============================================== */

#define CNC_RUN_MS 10

#endif // _BATTERY_H_