#include <Arduino.h>
#include <string.h>
#include "Config.h"
#include "VUEF.h"
#include "ui.h"
#include "uiHelper.h"
#include "CNC.h"

// http://fluidnc.local/  192.168.0.184
// http://grblesp.local/

Stream* cncStream = &Serial2;
uint32_t cncTs_ = 0;
uint32_t cncStatusCheckTs_ = 0;
uint32_t cncStatusTs_ = 0;

#define CNC_STREAM_BUFFER_SIZE 256
char cncStreamReadBuffer[CNC_STREAM_BUFFER_SIZE];
size_t cncStreamReadBufferIndex = 0;

const char* PROGMEM CNC_HOST = "http://fluidnc.local/";

RegGroup configGroupCNC(FST("CNC"));
ConfigUInt32 configUart2Speed(FST("Baud"), 115200, FST("UART2 Speed"), 0, &configGroupCNC);
ConfigUInt32 configCheckStatusInterval(FST("Status Check"), 3000, FST("Check CNC status interval"), 0, &configGroupCNC);
ConfigStr configCncHost(FST("Host"), 32, CNC_HOST, FST("CNC"), 0, &configGroupCNC);
ConfigUInt8 configAxesCount(FST("Axes"), 3, FST("Number of axes"), 0, &configGroupCNC);

typedef enum CncConnectionTypeEnum {CCT_NONE, CCT_WIFI, CCT_BT, CCT_UART, CCT_USB } CncConnectionTypeEnum;
const ConfigEnum::Option configCncConnectionTypeOptions[] PROGMEM = {
  { "None", CCT_NONE },
  { "WIFI", CCT_WIFI },
  { "BT", CCT_BT },
  { "UART", CCT_UART },
  { "USB", CCT_USB }
};
const size_t configCncConnectionTypeOptionsSize = sizeof(configCncConnectionTypeOptions) / sizeof(ConfigEnum::Option);
ConfigEnum configCncConnectionType(FST("Connection Type"), configCncConnectionTypeOptions, configCncConnectionTypeOptionsSize, CCT_UART, FST("None/WIFI/BT/UART/USB"), 0, &configGroupCNC);

typedef enum CncConnectionStateEnum {CCS_UNKNOWN, CCS_CONNECTING, CCS_CONNECTED, CCS_TIMEOUT, CCS_ERROR } CncConnectionStateEnum;
const StateEnum::Option stateCncConnectionStateOptions[] PROGMEM = {
  { "Unknown", CCS_UNKNOWN },
  { "Connecting", CCS_CONNECTING },
  { "Connected", CCS_CONNECTED },
  { "Timeout", CCS_TIMEOUT },
  { "Error", CCS_ERROR }
};
const size_t stateCncConnectionStateOptionsSize = sizeof(stateCncConnectionStateOptions) / sizeof(StateEnum::Option);
StateEnum stateCncConnectionState(FST("Connection State"), stateCncConnectionStateOptions, stateCncConnectionStateOptionsSize, CCS_UNKNOWN, 0, 0, &configGroupCNC);

const ConfigEnum::Option configCncCurrentAxisOptions[] PROGMEM = {
  { "-", CNC_AXIS_NONE },
  { "X", CNC_AXIS_X },
  { "Y", CNC_AXIS_Y },
  { "Z", CNC_AXIS_Z },
  { "A", CNC_AXIS_A },
  { "B", CNC_AXIS_B },
  { "C", CNC_AXIS_C },
};
const size_t configCncCurrentAxisOptionsSize = sizeof(configCncCurrentAxisOptions) / sizeof(ConfigEnum::Option);
ConfigEnum configCncCurrentAxis(FST("Current Axis"), configCncCurrentAxisOptions, configCncCurrentAxisOptionsSize, CNC_AXIS_X, FST("-/X/Y/Z/A/B/C"), 0, &configGroupCNC);
const char CNC_AXIS_LETTER[] PROGMEM = "-XYZABC";

typedef enum CncStateEnum {CS_UNKNOWN, CS_CONNECTING, CS_TIMEOUT, CS_IDLE, CS_ALARM, CS_CHECK, CS_HOMING, CS_RUN, CS_JOG, CS_HOLD, CS_DOOR, CS_SLEEP } CncStateEnum;
const StateEnum::Option configCncStateOptions[] PROGMEM = {
  { "Unknown", CS_UNKNOWN },
  { "Connecting", CS_CONNECTING },
  { "Timeout", CS_TIMEOUT },
  { "Idle", CS_IDLE },
  { "Alarm", CS_ALARM },
  { "Check", CS_CHECK },
  { "Homing", CS_HOMING },
  { "Run", CS_RUN },
  { "Jog", CS_JOG },
  { "Hold", CS_HOLD },
  { "Door", CS_DOOR },
  { "Sleep", CS_SLEEP },
};
const size_t configCncStateOptionsSize = sizeof(configCncStateOptions) / sizeof(StateEnum::Option);
StateEnum configCncState(FST("CNC State"), configCncStateOptions, configCncStateOptionsSize, 1, FST("state"), 0, &configGroupCNC);

int32_t cncStateColor[] = {
  0xA0A0A0, // CS_UNKNOWN
  0xA0A0A0, // CS_CONNECTING
  0xB01010, // CS_TIMEOUT
  0x10B0B0, // CS_IDLE
  0xB01010, // CS_ALARM
  0xB0B010, // CS_CHECK
  0x10B050, // CS_HOMING
  0x10B010, // CS_RUN
  0x10B060, // CS_JOG
  0xB06010, // CS_HOLD
  0xB07030, // CS_DOOR
  0x4040A0 // CS_SLEEP
};


StateFloat stateCncFeed(FST("Feed"), 0.0, FST("Current feed rate"), 0, &configGroupCNC);
StateFloat stateCncFeedOverride(FST("Feed Override"), 0.0, FST("Feed rate override"), 0, &configGroupCNC);
StateFloat stateCncRapidsOverride(FST("Feed Override"), 0.0, FST("Feed rate override"), 0, &configGroupCNC);
StateFloat stateCncSpeed(FST("Speed"), 0.0, FST("Current spindle speed"), 0, &configGroupCNC);
StateFloat stateCncSpeedOverride(FST("Speed Override"), 0.0, FST("Spindle speed override"), 0, &configGroupCNC);

CncAxis cncAxis[] = {
    CncAxis(FST("X Axis"), CNC_AXIS_X, 'X', 100.0, 1000.0, 10.0),
    CncAxis(FST("Y Axis"), CNC_AXIS_Y, 'Y', 100.0, 1000.0, 10.0),
    CncAxis(FST("Z Axis"), CNC_AXIS_Z, 'Z', 10.0, 100.0, 1.0),
    CncAxis(FST("A Axis"), CNC_AXIS_A, 'C', 10.0, 100.0, 1.0),
    CncAxis(FST("B Axis"), CNC_AXIS_B, 'B', 10.0, 100.0, 1.0),
    CncAxis(FST("C Axis"), CNC_AXIS_C, 'A', 10.0, 100.0, 1.0),
};

void cncSend(const char* text) {
    if (cncStream) { cncStream->println(text); }
    if (debugStream && cncStream != debugStream) { debugStream->println(text); }
}

void cncSendCmdJog(CncAxisEnum axis, float speed, float distance) {
    if (axis == CNC_AXIS_NONE) { return; }
    char buffer[64];
    size_t n = snprintf(buffer, sizeof(buffer)-2, FST("$J=G91 G21 F%g %c%g"), speed, CNC_AXIS_LETTER[axis], distance);
    buffer[n] = 0;
    return cncSend(buffer);
}

size_t _readCncToken(const char* src, char* token, size_t max) {
    size_t n = 0;
    while (n < max) {
        char c = src[n];
        if (c == 0) { break; }
        token[n++] = c;
        if (strchr(FST("<>|:, \n\r"), c)) { break; }
    }
    token[n] = '\0';
    return n;
}

void _cncSetState(const char* token) {
    if (!strcasecmp(token, FST("Idle"))) { configCncState.set(CS_IDLE); }
    else if (!strcasecmp(token, FST("Alarm"))) { configCncState.set(CS_ALARM); }
    else if (!strcasecmp(token, FST("Check"))) { configCncState.set(CS_CHECK); }
    else if (!strcasecmp(token, FST("Homing"))) { configCncState.set(CS_HOMING); }
    else if (!strcasecmp(token, FST("Run"))) { configCncState.set(CS_RUN); }
    else if (!strcasecmp(token, FST("Jog"))) { configCncState.set(CS_JOG); }
    else if (!strcasecmp(token, FST("Hold"))) { configCncState.set(CS_HOLD); }
    else if (!strcasecmp(token, FST("Door"))) { configCncState.set(CS_DOOR); }
    else if (!strcasecmp(token, FST("Sleep"))) { configCncState.set(CS_SLEEP); }
    else {
        configCncState.set(CS_UNKNOWN);
        DEBUG_printf(FST("Unknown CNC state: %s\n"), token);
    }
    uiUpdateLabel(uiCncStateLabel, configCncState.getText(), -1, cncStateColor[configCncState.get()]);
}

void _parseCncResponse() {
    size_t n = 0;
    size_t i = 0;
    typedef enum CncReadStateEnum {PS_START, PS_STATE, PS_MPOS, PS_FS, PS_OV, PS_WCO, PS_DONE } CncReadStateEnum;
    CncReadStateEnum state = PS_START;
    size_t p = 0;
    char token[64];
    do {
        i = _readCncToken(cncStreamReadBuffer+n, token, sizeof(token));
        char sep = token[i-1];
        if (i > 1) { token[i-1] = '\0'; }
        if (!strcmp(token, FST("<"))) { state = PS_STATE; }
        else if (sep == ':') {
            if (!strcmp(token, FST("MPos"))) { state = PS_MPOS; }
            else if (!strcmp(token, FST("WCO"))) { state = PS_WCO; }
            else if (!strcmp(token, FST("FS"))) { state = PS_FS; }
            else if (!strcmp(token, FST("Ov"))) { state = PS_OV; }
            else { DEBUG_printf(FST("Unknown CNC info token: %s\n"), token); }
        }
        else if (state == PS_STATE) { _cncSetState(token); }
        else if (state == PS_MPOS) {
            if (p < CNC_AXIS_MAX-1) { 
                cncAxis[p].machinePos.set(atof(token));
                cncAxis[p].showMachineCoordinates();
            }
        }
        else if (state == PS_WCO) {
            if (p < CNC_AXIS_MAX-1) { 
                cncAxis[p].workCoordinate.set(atof(token)); 
                cncAxis[p].showWorkCoordinates();
            }
        }
        else if (state == PS_FS) {
            if (p == 0) { 
                stateCncSpeed.set(atof(token)); 
                uiUpdateSettingValue(uiPanelSettingsSpeed, stateCncSpeed.get());
            }
            else if (p == 1) { 
                stateCncFeed.set(atof(token)); 
                uiUpdateSettingValue(uiPanelSettingsFeed, stateCncFeed.get());
            }
        }
        else if (state == PS_OV) {
            if (p == 0) { stateCncSpeedOverride.set(atof(token)); }
            else if (p == 1) { stateCncRapidsOverride.set(atof(token)); }
            else if (p == 2) { stateCncFeedOverride.set(atof(token)); }
        }
        else { DEBUG_printf(FST("Unknown CNC state token:%s sep=%c state=%d\n"), token, sep, state); }
        if (sep == ',') { p++; }
        else { p = 0; }

        // DEBUG_print(token); DEBUG_print(" "); DEBUG_println(term);
        n += i;
    } while (i && cncStreamReadBuffer[n]);
}

void _readCncStream() {
  if (!cncStream) { return; }
  while (cncStream->available()) {
    char c = cncStream->read();
    if (c == '\n' || c == '\r') { 
        if (cncStreamReadBufferIndex > 0) {
            cncStreamReadBuffer[cncStreamReadBufferIndex] = '\0';
            cncStreamReadBufferIndex = 0;
            DEBUG_print(FST("CNC: "));
            DEBUG_println(cncStreamReadBuffer);
            cncStatusTs_ = millis();
            _parseCncResponse();
        }
    } else {
        cncStreamReadBuffer[cncStreamReadBufferIndex++] = c;
    }
    if (cncStreamReadBufferIndex > sizeof(cncStreamReadBuffer)-1) {
      cncStreamReadBufferIndex = sizeof(cncStreamReadBuffer)-1;
    }
  }
}

void cncInit() {
    Serial2.begin(115200, SERIAL_8N1, UART2_RXD, UART2_TXD);
}

void cncRun(uint32_t now) {
  if (now == 0) { now = millis(); }

  if (cncStream) {
    _readCncStream();
    int i = configCheckStatusInterval.get();
    if (i > 0 && now >= cncStatusCheckTs_ + i) {
        cncStatusCheckTs_ = now;
        cncStream->write('?');
        // DEBUG_println(FST("Check Status"));
    }
  }

  if (now < cncTs_ + CNC_RUN_MS) { return; }
  cncTs_ = now;

}


CncAxisEnum cncIncActiveAxis(int32_t steps) {
    int currentAxis = configCncCurrentAxis.get();
    int old = currentAxis;
    currentAxis += steps;
    if (currentAxis <= 0) { currentAxis = 0; }
    if (currentAxis >= configAxesCount.get()) { currentAxis = configAxesCount.get(); }
    if (currentAxis != old) {
        configCncCurrentAxis.set(currentAxis);
        DEBUG_printf(FST("Current Axis %d\n"), currentAxis);
        uiHighlightAxis(currentAxis - 1);
    }
    return (CncAxisEnum) currentAxis;
}

void cncIncJogFeed(int32_t steps) {
    size_t currentAxis = configCncCurrentAxis.get();
    if (!currentAxis || currentAxis > CNC_AXIS_MAX) { return; }
    currentAxis--;
    cncAxis[currentAxis].incJogFeed(steps);
    char buffer[32];
    sprintf(buffer, FST("%g"), cncAxis[currentAxis].jogFeed.get());   
    lv_label_set_text(uiPanelSettingsJogFeed.text, buffer);
}

void cncIncJogStep(int32_t steps) {
    size_t currentAxis = configCncCurrentAxis.get();
    if (!currentAxis || currentAxis > CNC_AXIS_MAX) { return; }
    currentAxis--;
    cncAxis[currentAxis].incJogStep(steps);
    char buffer[32];
    sprintf(buffer, FST("%g"), cncAxis[currentAxis].jogStep.get());   
    lv_label_set_text(uiPanelSettingsJogStep.text, buffer);
}

void cncJogAxis(int32_t steps) {
    size_t currentAxis = configCncCurrentAxis.get();
    if (!currentAxis || currentAxis > CNC_AXIS_MAX) { return; }
    currentAxis--;
    cncAxis[currentAxis].jog(steps);
}

/* ============================================== *\
 * Axis
\* ============================================== */

CncAxis::CncAxis(const char* groupName, CncAxisEnum axis_, char letter_, float defaultFeed, float defaultJogFeed, float defaultJogStep) :
    axis(axis_),
    letter(letter_),
    configGroup(groupName, &configGroupCNC), 
    feed(FST("Feed Rate"), defaultFeed, FST("Current feed rate"), 0, &configGroup),
    jogFeed(FST("Jog Feed"), defaultJogFeed, FST("Current jog feed rate"), 0, &configGroup),
    jogStep(FST("Jog Step"), defaultJogStep, FST("Current jog step distance"), 0, &configGroup),
    machinePos(FST("Machine Pos"), 0.0, FST("Current axis machine postion"), 0, &configGroup),
    workCoordinate(FST("Work Coord"), 0.0, FST("Current axis work coordiante"), 0, &configGroup)
    {}

float CncAxis::incFeed(int32_t steps) {
    float tmp = feed.get();
    tmp += 100.0 * steps; 
    if (tmp < 1.0) { tmp = 1.0; }
    if (tmp > 5000.0) { tmp = 5000.0; }
    feed.set(tmp);
    DEBUG_printf(FST("Axis %c feed: %g\n"), letter, tmp);
    return tmp;
}

float CncAxis::incJogFeed(int32_t steps) {
    float tmp = jogFeed.get();
    tmp += 100.0 * steps; 
    if (tmp < 1.0) { tmp = 1.0; }
    if (tmp > 5000.0) { tmp = 5000.0; }
    jogFeed.set(tmp);
    DEBUG_printf(FST("Axis %c jog feed: %g\n"), letter, tmp);
    return tmp;
}

float CncAxis::incJogStep(int32_t steps) {
    float tmp = jogStep.get();
    if (steps > 0 ) { tmp *= 10.0; }
    else if (steps < 0 ) { tmp /= 10.0; }
    if (tmp < 0.01) { tmp = 0.01; }
    if (tmp > 100.0) { tmp = 100.0; }
    jogStep.set(tmp);
    DEBUG_printf(FST("Axis %c jog step: %g\n"), letter, tmp);
    return tmp;
}

void cncIncFeed(int32_t steps) {
    size_t currentAxis = configCncCurrentAxis.get();
    if (!currentAxis || currentAxis > CNC_AXIS_MAX) { return; }
    currentAxis--;
    cncAxis[currentAxis].incFeed(steps);
    char buffer[32];
    sprintf(buffer, FST("%g"), cncAxis[currentAxis].feed.get());   
    lv_label_set_text(uiPanelSettingsFeed.text, buffer);
}

void CncAxis::jog(int steps) {
    cncSendCmdJog(axis, jogFeed.get(), jogStep.get() * steps);
}

void CncAxis::showWorkCoordinates() {
    if (axis == CNC_AXIS_NONE || axis > CNC_AXIS_A ) { return; }
    uiUpdateAxisValue(uiAxis[axis-1].work, workCoordinate.get());
}

void CncAxis::showMachineCoordinates() {
    if (axis == CNC_AXIS_NONE || axis > CNC_AXIS_A ) { return; }
    uiUpdateAxisValue(uiAxis[axis-1].machine, machinePos.get());
}
