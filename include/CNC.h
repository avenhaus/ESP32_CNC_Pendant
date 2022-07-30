#ifndef _CNC_H_
#define _CNC_H_

#include "Arduino.h"


/* ============================================== *\
 * Axis
\* ============================================== */

#define CNC_MAX_AXES 6
typedef enum CncAxisEnum {CNC_AXIS_NONE, CNC_AXIS_X, CNC_AXIS_Y, CNC_AXIS_Z, CNC_AXIS_A, CNC_AXIS_B, CNC_AXIS_C, CNC_AXIS_MAX} CncAxisEnum;

class CncAxis {
public:
    CncAxis(const char* groupName, CncAxisEnum axis, char letter, float defaultFeed, float defaultJogFeed, float defaultJogStep, float defaultMaxFeed, float defaultMaxPos, float defaultMinPos);
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
    ConfigFloat maxFeed;
    ConfigFloat maxPos;
    ConfigFloat minPos;
    ConfigFloat throttle;

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

typedef enum CncConnectionStateEnum {CCS_UNKNOWN, CCS_CONNECTING, CCS_GOT_CONFIG, CCS_CONNECTED, CCS_TIMEOUT, CCS_ERROR } CncConnectionStateEnum;
extern StateEnum stateCncConnectionState;

typedef enum CncStateEnum {CS_UNKNOWN, CS_CONNECTING, CS_TIMEOUT, CS_IDLE, CS_ALARM, CS_CHECK, CS_HOMING, CS_RUN, CS_JOG, CS_HOLD, CS_DOOR, CS_SLEEP } CncStateEnum;
extern StateEnum configCncState;
extern int32_t cncStateColor[];

extern StateFloat stateCncFeed;
extern StateFloat stateCncFeedOverride;
extern StateFloat stateCncRapidsOverride;
extern StateFloat stateCncSpeed;
extern StateFloat stateCncSpeedOverride;

extern CncAxis cncAxis[CNC_MAX_AXES];

extern ConfigFloat joyJogFeedMode;

/* ============================================== *\
 * Constants
\* ============================================== */

#define CNC_RUN_MS 10
#define CNC_MAX_JOG_RATE_MS 100

#endif // _BATTERY_H_