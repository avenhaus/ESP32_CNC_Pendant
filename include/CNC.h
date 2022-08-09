#ifndef _CNC_H_
#define _CNC_H_

#include "Arduino.h"
#include "VUEF.h"

/* ============================================== *\
 * Axis
\* ============================================== */

#define CNC_MAX_AXES 6
typedef enum CncAxisEnum {CNC_AXIS_NONE, CNC_AXIS_X, CNC_AXIS_Y, CNC_AXIS_Z, CNC_AXIS_A, CNC_AXIS_B, CNC_AXIS_C, CNC_AXIS_MAX} CncAxisEnum;

class CncAxis {
public:
    CncAxis(const char* groupName, CncAxisEnum axis, char letter, float defaultFeed, float defaultJogFeed, float defaultJogStep, float defaultMaxFeed, float defaultMaxTravel);
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
    ConfigFloat maxTravel;
    ConfigFloat throttle;

    StateFloat machinePos;
    StateFloat workCoordinate;
};

void cncInit();
void cncRun(uint32_t now=0);
void cncSend(const char* text);
CncAxisEnum cncIncActiveAxis(int32_t steps);
void cncIncFeed(int32_t steps);
void cncIncJogFeed(int32_t steps);
void cncIncJogStep(int32_t steps);
void cncJogAxis(int32_t steps);

void cncAxisEncoderPress();

typedef enum CncConnectionTypeEnum {CCT_NONE, CCT_WIFI, CCT_BT, CCT_UART, CCT_USB, CCT_ESP_NOW} CncConnectionTypeEnum;
extern const size_t configCncConnectionTypeOptionsSize;
extern ConfigEnum configCncConnectionType;

typedef enum CncSettingsEncoderMode { CSEM_NONE, CSEM_JOG_STEP, CSEM_JOG_FEED, CSEM_FEED_OVERRIDE, CSEM_RAPIDS_OVERRIDE, CSEM_SPEED_OVERRIDE, CSEM_MAX } CncSettingsEncoderMode;
void cncIncSettingsEncoder(int32_t steps);
void cncChangeSettingsEncoderMode(int32_t steps);

typedef enum CncConnectionStateEnum { CCS_UNKNOWN, CCS_CONNECTING, CCS_GET_CONFIG, CCS_CONNECTED, CCS_TIMEOUT, CCS_ERROR, CCS_CONNECTION_LOST  } CncConnectionStateEnum;
extern StateEnum stateCncConnectionState;
void cncSetConnectionState(CncConnectionStateEnum state);

typedef enum CncStateEnum {CS_UNKNOWN, CS_CONNECTING, CS_TIMEOUT, CS_IDLE, CS_ALARM, CS_CHECK, CS_HOMING, CS_RUN, CS_JOG, CS_HOLD, CS_DOOR, CS_SLEEP } CncStateEnum;
extern StateEnum configCncState;
void cncSetState(CncStateEnum state);

typedef enum CncMachineTypeEnum {CMT_UNKNOWN, CMT_GRBL, CMT_FLUIDNC} CncMachineTypeEnum;
extern ConfigEnum configCncMachineType;
void cncSetCncMachineType(CncMachineTypeEnum type);



extern StateFloat stateCncFeed;
extern StateFloat stateCncFeedOverride;
extern StateFloat stateCncRapidsOverride;
extern StateFloat stateCncSpeed;
extern StateFloat stateCncSpeedOverride;

extern ConfigUInt32 configShowMessageToastMs;
extern ConfigUInt32 configShowErrorToastMs;

extern CncAxis cncAxis[CNC_MAX_AXES];

extern ConfigFloat joyJogFeedMode;

extern uint32_t cncCmdCnt;
extern uint32_t cncResponseCnt;
extern bool cncResponseError;

extern const char* CNC_CMD_ALARM_RESET;
extern const char* CNC_CMD_CONTROL_START;
extern const char* CNC_CMD_CONTROL_PAUSE;
extern const char* CNC_CMD_CONTROL_RESET;
extern const char* CNC_CMD_HOME_ALL;
extern const char* CNC_CMD_ZERO_ALL;
extern const char* CNC_CMD_ZERO_XYZ;
extern const char* CNC_CMD_RESTART;


/* ============================================== *\
 * Constants
\* ============================================== */

#define CNC_RUN_MS 10
#define CNC_MAX_JOG_RATE_MS 100
#define CNC_CMD_TIMEOUT_MS 3000
#define CNC_STATUS_TIMEOUT_MS 800

#endif // _CNC_H_