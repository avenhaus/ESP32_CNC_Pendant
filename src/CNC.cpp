#include <Arduino.h>
#include <string.h>
#include "Config.h"
#include "VUEF.h"
//#include "BluetoothSerial.h"
#include "ui.h"
#include "uiHelper.h"
#include "CNC.h"
#include <algorithm> 
// http://fluidnc.local/  192.168.0.184
// http://grblesp.local/


#if ENABLE_WIFI
#include "TcpServer.h"
TcpConnection cncTcpConnection;
static uint32_t _tcpConnectRetryTS = 0;
void tcpClientRun(uint32_t now);
bool _tcpConnectTaskStarted = false;
#endif

const char* _axisInfoCmd[] PROGMEM = {
    "max_rate_mm_per_min",
    "max_travel_mm",
    "homing/mpos_mm",
    "homing/positive_direction"
};
const int _AXIS_INFO_CMD_CNT = sizeof(_axisInfoCmd) / sizeof(char*);

void parseGrblResponse(const char* line);

//BluetoothSerial SerialBT;

Stream* cncStream = nullptr;
uint32_t _cncCmdSentTs = 0;
uint32_t _cncCmdResponseTs = 0;
uint32_t _cncStatusCheckTs = 0;
uint32_t _cncStatusResponseTs = 0;
uint32_t _joyJogNextTs = 0;
uint32_t _emergencyStopTs = 0;
bool _isJoyJogging = false;
bool _isStreamConnected = false;

uint32_t _cncGetConfigTs = 0;
uint32_t _cncGetConfigState = 0;
uint32_t _cncConfigResponseCount = 0;
bool _cncIsConfigResponse = false;

uint32_t cncCmdCnt = 0;
uint32_t cncResponseCnt = 0;
bool cncResponseError = false;

uint32_t _cncNextJogTs = 0;
int _cncBufferedJogSteps = 0;

#define CNC_STREAM_BUFFER_SIZE 256
char cncStreamReadBuffer[CNC_STREAM_BUFFER_SIZE];
size_t cncStreamReadBufferIndex = 0;

const char* PROGMEM CNC_HOST = "fluidnc.local";

RegGroup configGroupCNC(FST("CNC"));
ConfigUInt32 configUart2Speed(FST("Baud"), 115200, FST("UART2 Speed"), 0, &configGroupCNC);
ConfigUInt32 configCheckStatusInterval(FST("Status Check"), 1000, FST("Check CNC status interval"), 0, &configGroupCNC);
ConfigStr configCncHost(FST("Host"), 32, CNC_HOST, FST("CNC host name"), 0, &configGroupCNC);
ConfigUInt16 configCncPort(FST("Port"), 23, FST("CNC TCP port number"), 0, &configGroupCNC);
ConfigUInt8 configAxesCount(FST("Axes"), 4, FST("Number of axes"), 0, &configGroupCNC);

ConfigUInt32 configShowMessageToastMs(FST("Message Toast ms"), 5000, FST("How long a message is displayed on screen."), 0, &configGroupCNC);
ConfigUInt32 configShowErrorToastMs(FST("Error Toast ms"), 5000, FST("How long an error message is displayed on screen."), 0, &configGroupCNC);

const ConfigEnum::Option configCncConnectionTypeOptions[] PROGMEM = {
  { "None", CCT_NONE },
  { "WIFI", CCT_WIFI },
  // { "Bluetooth", CCT_BT },
  { "UART", CCT_UART },
  { "USB", CCT_USB },
  //{ "ESP-NOW", CCT_ESP_NOW }
};
const size_t configCncConnectionTypeOptionsSize = sizeof(configCncConnectionTypeOptions) / sizeof(ConfigEnum::Option);
ConfigEnum configCncConnectionType(FST("Connection Type"), configCncConnectionTypeOptions, configCncConnectionTypeOptionsSize, CCT_UART, FST("None/WIFI/BT/UART/USB"), 0, &configGroupCNC);

const StateEnum::Option stateCncConnectionStateOptions[] PROGMEM = {
  { "Unknown", CCS_UNKNOWN },
  { "Connecting", CCS_CONNECTING },
  { "Get Config", CCS_GET_CONFIG },
  { "Connected", CCS_CONNECTED },
  { "Timeout", CCS_TIMEOUT },
  { "Error", CCS_ERROR },
  { "Connection Lost", CCS_CONNECTION_LOST }
};
const size_t stateCncConnectionStateOptionsSize = sizeof(stateCncConnectionStateOptions) / sizeof(StateEnum::Option);
StateEnum stateCncConnectionState(FST("Connection State"), stateCncConnectionStateOptions, stateCncConnectionStateOptionsSize, CCS_UNKNOWN, 0, 0, &configGroupCNC);

int32_t cncConnectionStateColor[] = {
  0xD0D0D0, // CCS_UNKNOWN
  0x40FFFF, // CCS_CONNECTING
  0xFFFF40, // CCS_GET_CONFIG
  0x40FF40, // CCS_CONNECTED
  0xFF4040, // CCS_TIMEOUT
  0xFF4040, // CCS_ERROR
  0xFF4040, // CCS_CONNECTION_LOST
};

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
  0xD02020, // CS_TIMEOUT
  0x10B0B0, // CS_IDLE
  0xD02020, // CS_ALARM
  0xB0B010, // CS_CHECK
  0x10B050, // CS_HOMING
  0x10B010, // CS_RUN
  0x10B060, // CS_JOG
  0xA0A010, // CS_HOLD
  0xB07030, // CS_DOOR
  0x4040A0  // CS_SLEEP
};

ConfigFloat joyJogFeedMode(FST("JJ Mode"), 0.1, FST("Joystick Jog speed level"), 0, &configGroupCNC);
ConfigFloat joyJogAdjust(FST("JJ Adjust"), 0.92, FST("Adjust calculate jog distance values to match reality"), 0, &configGroupCNC);
ConfigFloat joyJogDt(FST("JJ DT"), 0.1, FST("Time interval between jog commands"), 0, &configGroupCNC);
ConfigFloat cncHoverZ(FST("Z Hover"), 10.0, FST("Z distance from work 0 for rapid moves"), 0, &configGroupCNC);

StateFloat stateCncFeed(FST("Feed"), 0.0, FST("Current feed rate"), 0, &configGroupCNC);
StateFloat stateCncFeedOverride(FST("Feed Override"), 0.0, FST("Feed rate override"), 0, &configGroupCNC);
StateFloat stateCncRapidsOverride(FST("Feed Override"), 0.0, FST("Feed rate override"), 0, &configGroupCNC);
StateFloat stateCncSpeed(FST("Speed"), 0.0, FST("Current spindle speed"), 0, &configGroupCNC);
StateFloat stateCncSpeedOverride(FST("Speed Override"), 0.0, FST("Spindle speed override"), 0, &configGroupCNC);

CncSettingsEncoderMode cncSettingsEncoderMode = CSEM_JOG_STEP;
typedef void (*CncEncoderCB)(int32_t steps);
CncEncoderCB cncEncoderCB[] = {
  nullptr,         // CSEM_NONE
  cncIncJogStep,   // CSEM_JOG_STEP
  cncIncJogFeed,   // CSEM_JOG_FEED
  nullptr,         // CSEM_FEED_OVERRIDE
  nullptr,         // CSEM_RAPIDS_OVERRIDE
  nullptr          // CSEM_SPEED_OVERRIDE
};

CncAxis cncAxis[CNC_MAX_AXES] = {
    CncAxis(FST("X Axis"), CNC_AXIS_X, 'X', 100.0, 1000.0, 10.0, 2000.0, 300.0),
    CncAxis(FST("Y Axis"), CNC_AXIS_Y, 'Y', 100.0, 1000.0, 10.0, 2000.0, 300.0),
    CncAxis(FST("Z Axis"), CNC_AXIS_Z, 'Z',  10.0,  100.0,  1.0,  500.0, 100.0),
    CncAxis(FST("A Axis"), CNC_AXIS_A, 'A',  10.0,  100.0,  1.0,  500.0, 100.0),
    CncAxis(FST("B Axis"), CNC_AXIS_B, 'B',  10.0,  100.0,  1.0,  500.0, 100.0),
    CncAxis(FST("C Axis"), CNC_AXIS_C, 'C',  10.0,  100.0,  1.0,  500.0, 100.0),
};


const ConfigEnum::Option configCncMachineTypeOptions[] PROGMEM = {
  { "Unknown", CMT_UNKNOWN },
  { "GRBL", CMT_GRBL },
  { "FluidNC", CMT_FLUIDNC }
};
const size_t configCncMachineTypeOptionsSize = sizeof(configCncMachineTypeOptions) / sizeof(ConfigEnum::Option);
ConfigEnum configCncMachineType(FST("CNC Type"), configCncMachineTypeOptions, configCncMachineTypeOptionsSize, CMT_UNKNOWN, FST("Unknown/GRBL/FluidNC"), 0, &configGroupCNC);



/*==========================================================*\
 * CNC Commands
\*==========================================================*/

const char* CNC_CMD_ALARM_RESET = FST("$X");
const char* CNC_CMD_CONTROL_START = FST("~");
const char* CNC_CMD_CONTROL_PAUSE = FST("!");
const char* CNC_CMD_CONTROL_RESET = FST("\x18");
const char* CNC_CMD_HOME_ALL = FST("$H");
const char* CNC_CMD_ZERO_ALL = FST("G10 L20 P0 X0 Y0 Z0 A0");
const char* CNC_CMD_ZERO_XYZ = FST("G10 L20 P0 X0 Y0 Z0");
const char* CNC_CMD_RESTART = FST("$Bye");
const char* CNC_CMD_GO_WCO_0_A = FST("G00 X0Y0\nG00 G21 Z10");
const char* CNC_CMD_GO_WCO_0_B = FST("G00 G21 Z10\nG00 X0Y0");

const char* GCODE_MOVE = FST("G0");
const char* GCODE_MOVE_RAPID = FST("G00");
const char* GCODE_ABSOLUTE = FST("G90");
const char* GCODE_RELATIVE = FST("G91");
const char* GCODE_IMPERIAL = FST("G20");
const char* GCODE_METRIC = FST("G21");



CncAxis* getCurrentAxis() {
    size_t currentAxis = configCncCurrentAxis.get();
    if (!currentAxis || currentAxis > CNC_AXIS_MAX) { return nullptr; }
    currentAxis--;
    return &cncAxis[currentAxis];
}


/*==========================================================*\
 * GRBL configuration
\*==========================================================*/

void cncSend(const char* text) {
    if (cncStream) { 
        cncStream->print(text); 
        cncStream->write('\n'); 
        if (text[0] != '?') { 
            cncCmdCnt++; 
            _cncCmdSentTs = millis();
        }
    }
    if (debugStream && cncStream != debugStream) { debugStream->println(text); }
}

void cncSendCmdJog(CncAxisEnum axis, float speed, float distance) {
    if (axis == CNC_AXIS_NONE) { return; }
    char buffer[64];
    size_t n = snprintf(buffer, sizeof(buffer)-2, FST("$J=G91 G21 F%g %c%g"), speed, CNC_AXIS_LETTER[axis], distance);
    buffer[n] = 0;
    return cncSend(buffer);
}


void cncSetConnectionState(CncConnectionStateEnum state) {
    stateCncConnectionState.set(state);
    DEBUG_printf(FST("Connection state: %s\n"), stateCncConnectionState.getText());
    uiUpdateLabel(uiStatusBarState, stateCncConnectionState.getText(), cncConnectionStateColor[state]);
}

void cncSetState(CncStateEnum state) {
    configCncState.set(state);
    uiUpdateLabel(uiCncStateLabel, configCncState.getText(), -1, cncStateColor[configCncState.get()]);
}

void cncSetCncMachineType(CncMachineTypeEnum type) {
    configCncMachineType.set(type);
    if (type != CMT_UNKNOWN) { cncSetConnectionState(CCS_CONNECTED); }
    DEBUG_printf(FST("Connected Machine Type: %d\n"), type);
}


/*==========================================================*\
 * Send jog commands to GRBL controller
\*==========================================================*/

bool _cncJoyJog() {
    char buffer[64];
    buffer[0] = 0;
    float d[CNC_MAX_AXES];
    float total_feed = 0.0;
    for (size_t a=0; a<CNC_MAX_AXES; a++) {
        d[a] = 0.0;
        float throttle = cncAxis[a].throttle.get();
        if (abs(throttle) > 0.002) {
            float feed = abs(throttle) * cncAxis[a].maxFeed.get() * joyJogFeedMode.get();
            d[a] = feed * joyJogDt.get() / 60.0 * joyJogAdjust.get();
            if (throttle < 0) { d[a] = -d[a]; }
            if (feed) { total_feed = sqrt((total_feed * total_feed) + (feed * feed)); }
        }
    }
    if (total_feed) {
        size_t n = 0;
        if (total_feed > 1000.0) { n = sprintf(buffer, FST("$J=G91 G21 F%d"), (int) round(total_feed)); }
        else { n = sprintf(buffer, FST("$J=G91 G21 F%0.4g"), total_feed); }    
        for (size_t a=0; a<CNC_MAX_AXES; a++) {
            if (d[a]) { n += sprintf(buffer+n, FST(" %c%0.4g"), cncAxis[a].letter, d[a]); }
        }
        buffer[n] = '\0';
        cncSend(buffer);
        _isJoyJogging = true;
    } else if (_isJoyJogging) {
        buffer[0] = 0x85;  // Abort Jog command
        buffer[2] = 0;
        cncSend(buffer);
        _isJoyJogging = false;
    }
    return total_feed;
}

bool cncIsBelowHover() {
    return cncAxis[2].machinePos.get() < cncHoverZ.get();
}

void moveAxisCB(float value, bool isRelative, UiAxisCoord* uiAxisCoord) {
    CncAxis& axis = cncAxis[uiAxisCoord->axis];
    DEBUG_printf(FST("Move %d %s rel:%d val:%g\n"), uiAxisCoord->axis, lv_label_get_text(uiAxisCoord->label), isRelative, value);
    const char* gMode = isRelative ? GCODE_RELATIVE : GCODE_ABSOLUTE;
    const char* gMove = cncIsBelowHover() ? GCODE_MOVE : GCODE_MOVE_RAPID;
    if (!isRelative && lv_label_get_text(uiAxisCoord->label)[0] == 'm') {
        value = value - axis.workCoordinate;
    }

    char buffer[32];
    sprintf(buffer, FST("%s %s %c%g"), gMode, gMove, axis.letter, value);
    cncSend(buffer);
}

#if ENABLE_WIFI
void _tcpConnectTask(void* pvParameters) {
    while(true) {
        if (_isStreamConnected && !cncTcpConnection.isConnected()) {
            cncSetConnectionState(CCS_CONNECTION_LOST);
            _isStreamConnected = false;
        }
        uint32_t now = millis();
        if (now > _tcpConnectRetryTS && !cncTcpConnection.isConnected()) {
            _tcpConnectRetryTS = now + 4000;
            DEBUG_printf(FST("Connecting to %s:%d\n"), configCncHost.get(), configCncPort.get());
            if (cncTcpConnection.connect(configCncHost.get(), configCncPort.get(), 3000) != TcpConnection::Accepted) {
                DEBUG_printf(FST("Could not connect to CNC %s:%d\n"), configCncHost.get(), configCncPort.get());
                _isStreamConnected = false;
            } else {
                DEBUG_println(FST("CNC connected via TCP"));
                _isStreamConnected = true;
                CncConnectionStateEnum state = (CncConnectionStateEnum) stateCncConnectionState.get();
                if (state != CCS_CONNECTING && state != CCS_GET_CONFIG) {
                    cncSetConnectionState(CCS_CONNECTED);
                }
            }
        }
        vTaskDelay(100);
    }
}
#endif


void _readCncStream() {
  if (!cncStream || !_isStreamConnected) { return; }
  int available = 0;  
  while (available = cncStream->available() > 0) {
    char c = cncStream->read();
    if (c == '\n' || c == '\r') { 
        if (cncStreamReadBufferIndex > 0) {
            cncStreamReadBuffer[cncStreamReadBufferIndex] = '\0';
            cncStreamReadBufferIndex = 0;
            DEBUG_print(FST("CNC: "));
            DEBUG_println(cncStreamReadBuffer);
            if (!strcasecmp(FST("ok"), cncStreamReadBuffer)) {
                // DEBUG_printf(FST("CMD RT: %d\n"), millis() - _cncCmdSentTs);
                _cncCmdResponseTs = millis();
                if (cncResponseCnt < cncCmdCnt) { cncResponseCnt++; }
                cncResponseError = false;
                if (_cncIsConfigResponse) {
                    _cncIsConfigResponse = false;
                    _cncConfigResponseCount++;
                }
            } else { parseGrblResponse(cncStreamReadBuffer); }
        }
    } else {
        cncStreamReadBuffer[cncStreamReadBufferIndex++] = c;
    }
    if (cncStreamReadBufferIndex > sizeof(cncStreamReadBuffer)-1) {
      cncStreamReadBufferIndex = sizeof(cncStreamReadBuffer)-1;
    }
  }
  if (available < 0 && stateCncConnectionState.get() != CCS_ERROR) {
    cncSetConnectionState(CCS_ERROR);
  }
}

void cncSetAzisZero(int axis) {
    if (axis < 0 || axis > configAxesCount.get()) { return; }
    DEBUG_printf(FST("Set %c Zero\n"), cncAxis[axis].letter);
    char buffer[64];
    sprintf(buffer, FST("G10 L20 P0 %c0"), cncAxis[axis].letter);    
    cncSend(buffer);
}

void cncAxisEncoderPress() {
    static uint32_t lastPressTs = 0;
    uint32_t now = millis();
    if (lastPressTs > now - 500) {
        DEBUG_println(FST("Zero XYZ axes"));
        cncSend(CNC_CMD_ZERO_XYZ);
    } else { cncSetAzisZero(configCncCurrentAxis.get()-1); }
    lastPressTs = now;
}

static void _cncResetAlarm(lv_event_t * e) {
    DEBUG_println(FST("Reset Alarms"));
    showMessageToast(FST("Reset Alarms"), 3000);
    cncSend(CNC_CMD_ALARM_RESET);
}

static void _cncSetAxisZeroEvent(lv_event_t * e) {
    int axis = (int) lv_event_get_user_data(e);
    cncSetAzisZero(axis);
}


void _showCurrentAxisSettings() {
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    char buffer[32];
    sprintf(buffer, FST("%g"), axis->jogStep.get());   
    lv_label_set_text(uiPanelSettingsJogStep.text, buffer);
    sprintf(buffer, FST("%g"), axis->jogFeed.get());   
    lv_label_set_text(uiPanelSettingsJogFeed.text, buffer);
}

void setCncConnection() {
    DEBUG_printf(FST("CNC connection type: %s\n"), configCncConnectionType.getText());
    switch((CncConnectionTypeEnum) configCncConnectionType.get()) {
        case CCT_NONE:
            cncStream = nullptr;
            _isStreamConnected = false;
            break;

        case CCT_USB: 
            cncStream = &Serial;
            _isStreamConnected = true;
            break;

        case CCT_UART: 
            Serial2.begin(configUart2Speed.get(), SERIAL_8N1, UART2_RXD, UART2_TXD);
            cncStream = &Serial2;
            _isStreamConnected = true;
            break;

#if ENABLE_WIFI
        case CCT_WIFI:
            cncStream = &cncTcpConnection;
            _isStreamConnected = cncTcpConnection.isConnected();
            if (!_tcpConnectTaskStarted) {
                xTaskCreate(
                    _tcpConnectTask, // Task function
                    FST("connect"),  // String with name of task
                    2048,            // Stack size in bytes
                    NULL,            // Parameter passed as input of the task
                    1,               // Priority of the task.
                    NULL);           // Task handle.
                _tcpConnectTaskStarted = true;
            }
            break;
#endif

        /*case CCT_BT:
            SerialBT.begin(FST("ESP32Pendant"));
            cncStream = &SerialBT;
            break;*/

        case CCT_ESP_NOW:

        default:
            DEBUG_printf(FST("Unknown connection type: %d\n"), configCncConnectionType.get());
            cncStream = nullptr;
    }
}

void cncInit() {

    setCncConnection();

    uiHighlightAxis(configCncCurrentAxis.get() - 1);
    uiHighlightCncSetting((int)cncSettingsEncoderMode - 1);

    _showCurrentAxisSettings();

    lv_obj_add_event_cb(uiCncStateLabel, _cncResetAlarm, LV_EVENT_RELEASED, nullptr);
    for (int i=0; i<4; i++) {
        lv_obj_add_event_cb(uiAxis[i].wZeroButton, _cncSetAxisZeroEvent, LV_EVENT_CLICKED, (void*) i);
    }

    cncSetConnectionState(CCS_UNKNOWN);
    _joyJogNextTs = millis() + joyJogDt.get() * 1000;

    cncSetConnectionState(CCS_CONNECTING);
    _cncGetConfigTs = 0;
    _cncGetConfigState = 0;
    _cncConfigResponseCount = 0;
    _cncIsConfigResponse = false;

    uiMoveAxisCB = &moveAxisCB;
    uiPanelSettingsJogFeed.cb = cncSetJogFeed;
    uiPanelSettingsJogStep.cb = cncSetJogStep;
}

void cncRun(uint32_t now) {
    if (now == 0) { now = millis(); }

    if (cncStream && _isStreamConnected) {
        
#if EMERGENCY_STOP_BIT >= 0
        extern uint32_t extended_inputs;
        if (extended_inputs & (1<<EMERGENCY_STOP_BIT) && now > _emergencyStopTs) {
            cncSend(CNC_CMD_CONTROL_RESET);
            _emergencyStopTs = now + 500;
        }
#endif        
        _readCncStream();

        static uint32_t _getConfigRetryTs = 0;
        CncConnectionStateEnum state = (CncConnectionStateEnum) stateCncConnectionState.get();
        if (state == CCS_CONNECTING || state == CCS_GET_CONFIG) {
            if (now > _getConfigRetryTs) {
                _joyJogNextTs = now + joyJogDt.get() * 1000;
                if (_cncConfigResponseCount == 0) {
                    cncSend(FST("$$")); // Try read GRBL config
                    cncSetConnectionState(CCS_GET_CONFIG);
                    _getConfigRetryTs = now + 2000; // Retry every 2 sec
                    if(!_cncGetConfigTs) { _cncGetConfigTs = now; }
                    cncResponseCnt = cncCmdCnt - 1;
                }
            }
            if (cncResponseCnt == cncCmdCnt) {
                if (_cncConfigResponseCount > 0 && configCncMachineType.get() == CMT_UNKNOWN) { 
                    char buffer[64];
                    int n = _cncConfigResponseCount - 1;  // First one is GRBL config $$
                    if (n < _AXIS_INFO_CMD_CNT * 4) {
                        char axis = cncAxis[n/_AXIS_INFO_CMD_CNT].letter;
                        n = n % _AXIS_INFO_CMD_CNT;
                        sprintf(buffer, FST("$/axes/%c/%s"), axis, _axisInfoCmd[n]);
                        cncSend(buffer);
                    } else { cncSetCncMachineType(CMT_FLUIDNC); }
                }
            }
            if (now > _cncGetConfigTs + 5000) {
                DEBUG_println(FST("Timed out trying to get CNC config"));
                cncSetConnectionState(CCS_CONNECTED);
            }
        }

        state = (CncConnectionStateEnum) stateCncConnectionState.get();
        if (state == CCS_CONNECTED) {
            if (now >= _joyJogNextTs) {
                _cncJoyJog();
                _joyJogNextTs += joyJogDt.get() * 1000;
            }

            int i = configCheckStatusInterval.get();
            if (i > 0 && now >= _cncStatusCheckTs + i) {
                _cncStatusCheckTs = now;
                cncStream->write('?');
                // DEBUG_println(FST("Check Status"));
            }
            
            /*if (_cncCmdSentTs > _cncCmdResponseTs && now > _cncCmdSentTs + CNC_CMD_TIMEOUT_MS) {
                DEBUG_println(FST("Timed out waiting for CMD response"));
                cncSetConnectionState(CCS_TIMEOUT);
                cncSetState(CS_TIMEOUT);
            } */

            if (_cncStatusCheckTs > _cncStatusResponseTs && now > _cncStatusCheckTs + CNC_STATUS_TIMEOUT_MS) {
                DEBUG_println(FST("Timed out waiting for Status response"));
                cncSetConnectionState(CCS_TIMEOUT);
                cncSetState(CS_TIMEOUT);
            }
        }
        if (state == CCS_TIMEOUT) {
            int i = configCheckStatusInterval.get();
            if (i > 0 && now >= _cncStatusCheckTs + i) {
                _cncStatusCheckTs = now;
                cncStream->write('?');
                // DEBUG_println(FST("Check Status"));
            }
        }
  }

  if (_cncBufferedJogSteps && now > (_cncNextJogTs + CNC_MAX_JOG_RATE_MS)) {
    CncAxis* axis = getCurrentAxis();
    if (axis) { 
        // DEBUG_printf(FST("Flush buffered jog steps: %d\n"), _cncBufferedJogSteps); 
        axis->jog(_cncBufferedJogSteps);
    }
    _cncNextJogTs = now + CNC_MAX_JOG_RATE_MS;       
    _cncBufferedJogSteps = 0;
  }

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
        _showCurrentAxisSettings();
    }
    return (CncAxisEnum) currentAxis;
}

void cncShowJogFeed(CncAxis* axis) {
    if (!axis) { return; }
    char buffer[32];
    sprintf(buffer, FST("%g"), axis->jogFeed.get());   
    lv_label_set_text(uiPanelSettingsJogFeed.text, buffer);
}


void cncSetJogFeed(float value, void* cbData) {
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    axis->setJogFeed(value);
    cncShowJogFeed(axis);
}

void cncIncJogFeed(int32_t steps) {
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    axis->incJogFeed(steps);
    cncShowJogFeed(axis);
}

void cncShowJogStep(CncAxis* axis) {
    if (!axis) { return; }
    char buffer[32];
    sprintf(buffer, FST("%g"), axis->jogStep.get());   
    lv_label_set_text(uiPanelSettingsJogStep.text, buffer);
}

void cncSetJogStep(float value, void* cbData) {
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    axis->setJogStep(value);
    cncShowJogStep(axis);
}

void cncIncJogStep(int32_t steps) {
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    axis->incJogStep(steps);
    cncShowJogStep(axis);
}

void cncIncFeed(int32_t steps) {
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    axis->incFeed(steps);
    char buffer[32];
    sprintf(buffer, FST("%g"), axis->feed.get());   
    lv_label_set_text(uiPanelSettingsFeed.text, buffer);
}

void cncJogAxis(int32_t steps) {
    uint32_t now = millis();
    if (_cncNextJogTs > now) {
        _cncBufferedJogSteps += steps;
        return;
    }
    CncAxis* axis = getCurrentAxis();
    if (!axis) { return; }
    axis->jog(steps + _cncBufferedJogSteps);
    _cncNextJogTs = now + CNC_MAX_JOG_RATE_MS;
    _cncBufferedJogSteps = 0;
}

void cncIncSettingsEncoder(int32_t steps) {
    CncEncoderCB cb = cncEncoderCB[cncSettingsEncoderMode];
    if (cb) { cb(steps); }
}

void cncChangeSettingsEncoderMode(int32_t steps) {
    int newMode = (((int)cncSettingsEncoderMode) + steps) % (int)CSEM_MAX;
    if (newMode < 0) { newMode += (int)CSEM_MAX; }
    cncSettingsEncoderMode = (CncSettingsEncoderMode)newMode;
    DEBUG_printf(FST("New CNC Settings Mode: %d\n"), newMode);
    uiHighlightCncSetting(newMode - 1);
}

/* ============================================== *\
 * Axis
\* ============================================== */

CncAxis::CncAxis(const char* groupName, CncAxisEnum axis_, char letter_, float defaultFeed, float defaultJogFeed, float defaultJogStep, float defaultMaxFeed, float defaultMaxTravel) :
    axis(axis_),
    letter(letter_),
    configGroup(groupName, &configGroupCNC), 
    feed(FST("Feed Rate"), defaultFeed, FST("Current feed rate"), 0, &configGroup),
    jogFeed(FST("Jog Feed"), defaultJogFeed, FST("Current jog feed rate"), 0, &configGroup),
    jogStep(FST("Jog Step"), defaultJogStep, FST("Current jog step distance"), 0, &configGroup),
    maxFeed(FST("Feed Max"), defaultMaxFeed, FST("Maximum feed rate for the axis"), 0, &configGroup),
    maxTravel(FST("Max Pos"), defaultMaxTravel, FST("Maximum axis machine coordinate"), 0, &configGroup),
    throttle(FST("Throttle"), 0.0, FST("Analog jog throttle (joystick"), 0, &configGroup),
    machinePos(FST("Machine Pos"), 0.0, FST("Current axis machine postion"), 0, &configGroup),
    workPos(FST("Work Pos"), 0.0, FST("Current axis work position"), 0, &configGroup),
    workCoordinate(0.0)
    {}

float CncAxis::incFeed(int32_t steps) {
    float tmp = feed.get();
    tmp += 100.0 * steps; 
    if (tmp < 1.0) { tmp = 1.0; }
    if (tmp > maxFeed.get()) { tmp = maxFeed.get(); }
    feed.set(tmp);
    if (axis == CNC_AXIS_X) { cncAxis[CNC_AXIS_Y-1].feed.set(tmp); }
    if (axis == CNC_AXIS_Y) { cncAxis[CNC_AXIS_X-1].feed.set(tmp); }
    DEBUG_printf(FST("Axis %c feed: %g\n"), letter, tmp);
    return tmp;
}

void CncAxis::setJogFeed(float value) {
    if (value < 1.0) { value = 1.0; }
    if (value > maxFeed.get()) { value = maxFeed.get(); }
    jogFeed.set(value);
    if (axis == CNC_AXIS_X) { cncAxis[CNC_AXIS_Y-1].jogFeed.set(value); }
    if (axis == CNC_AXIS_Y) { cncAxis[CNC_AXIS_X-1].jogFeed.set(value); }
    DEBUG_printf(FST("Axis %c jog feed: %g\n"), letter, value);
}

float CncAxis::incJogFeed(int32_t steps) {
    float tmp = jogFeed.get();
    tmp += 100.0 * steps; 
    if (tmp < 1.0) { tmp = 1.0; }
    if (tmp > maxFeed.get()) { tmp = maxFeed.get(); }
    jogFeed.set(tmp);
    if (axis == CNC_AXIS_X) { cncAxis[CNC_AXIS_Y-1].jogFeed.set(tmp); }
    if (axis == CNC_AXIS_Y) { cncAxis[CNC_AXIS_X-1].jogFeed.set(tmp); }
    DEBUG_printf(FST("Axis %c jog feed: %g\n"), letter, tmp);
    return tmp;
}

void CncAxis::setJogStep(float value) {
    if (value < 0.001) { value = 0.001; }
    if (value > 100.0) { value = 100.0; }
    jogStep.set(value);
    if (axis == CNC_AXIS_X) { cncAxis[CNC_AXIS_Y-1].jogStep.set(value); }
    if (axis == CNC_AXIS_Y) { cncAxis[CNC_AXIS_X-1].jogStep.set(value); }
    DEBUG_printf(FST("Axis %c jog step: %g\n"), letter, value);
}

float CncAxis::incJogStep(int32_t steps) {
    float tmp = jogStep.get();
    if (steps > 0 ) { tmp *= 10.0; }
    else if (steps < 0 ) { tmp /= 10.0; }
    if (tmp < 0.001) { tmp = 0.001; }
    if (tmp > 100.0) { tmp = 100.0; }
    jogStep.set(tmp);
    if (axis == CNC_AXIS_X) { cncAxis[CNC_AXIS_Y-1].jogStep.set(tmp); }
    if (axis == CNC_AXIS_Y) { cncAxis[CNC_AXIS_X-1].jogStep.set(tmp); }
    DEBUG_printf(FST("Axis %c jog step: %g\n"), letter, tmp);
    return tmp;
}

void CncAxis::jog(int steps) {
    cncSendCmdJog(axis, jogFeed.get(), jogStep.get() * steps);
}

void CncAxis::showCoordinates() {
    if (axis == CNC_AXIS_NONE || axis > CNC_AXIS_A ) { return; }
    uiUpdateAxisValue(uiAxis[axis-1].work, workPos.get() );
    uiUpdateAxisValue(uiAxis[axis-1].machine, machinePos.get());
}

