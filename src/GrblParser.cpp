#include <Arduino.h>
#include <string.h>
#include "Config.h"
#include "VUEF.h"
#include "CNC.h"
#include "Ui.h"
#include "UiHelper.h"

char cncPinStates[32] = "-";
extern uint32_t _cncCmdResponseTs;
extern uint32_t _cncStatusResponseTs;
int getErrorCode = 0;

extern uint32_t _cncGetConfigTs;
extern uint32_t _cncGetConfigState;
extern bool _cncIsConfigResponse;

static const char* _grblStatusTerm = FST("<>|:, \n\r");
static const char* _grblInfoTerm = FST("/=\n\r");

size_t _readCncToken(const char* src, char* token, size_t max, const char* term) {
    size_t n = 0;
    while (n < max) {
        char c = src[n];
        if (c == 0) { break; }
        token[n++] = c;
        if (strchr(term, c)) { break; }
    }
    token[n] = '\0';
    return n;
}

void _cncSetState(const char* token) {
    if (!strcasecmp(token, FST("Idle"))) { cncSetState(CS_IDLE); }
    else if (!strcasecmp(token, FST("Alarm"))) { cncSetState(CS_ALARM); }
    else if (!strcasecmp(token, FST("Check"))) { cncSetState(CS_CHECK); }
    else if (!strcasecmp(token, FST("Homing"))) { cncSetState(CS_HOMING); }
    else if (!strcasecmp(token, FST("Run"))) { cncSetState(CS_RUN); }
    else if (!strcasecmp(token, FST("Jog"))) { cncSetState(CS_JOG); }
    else if (!strcasecmp(token, FST("Hold"))) { cncSetState(CS_HOLD); }
    else if (!strcasecmp(token, FST("Door"))) { cncSetState(CS_DOOR); }
    else if (!strcasecmp(token, FST("Sleep"))) { cncSetState(CS_SLEEP); }
    else {
        cncSetState(CS_UNKNOWN);
        DEBUG_printf(FST("Unknown CNC state: '%s'\n"), token);
    }
}

/*==========================================================*\
 * Handle incoming "$" config parameter line from GRBL controller
\*==========================================================*/

void _grblHandleConfigLine(const char* line) {
  // DEBUG_printf(FST("GRBL-in: %s\n"), line);
  size_t n = 0;
  if (line[0] == '$') {
    _cncIsConfigResponse = true;
    if (line[1] == '/') {
        n = 2;
        char token[64];
        n += _readCncToken(line+n, token, sizeof(token), _grblInfoTerm);
        if (!strcmp(token, FST("axes/"))) { 
            n += _readCncToken(line+n, token, sizeof(token), _grblInfoTerm);
            if (token[1] == '/') {
                char aLetter = token[0];
                int axis = -1;
                if (aLetter >= 'X' && aLetter <= 'Z') { axis = aLetter - 'X'; }
                else if (aLetter >= 'A' && aLetter <= 'C') { axis = aLetter - 'A' + 3; }
                else { DEBUG_printf(FST("Unknown axis letter: %c\n"), aLetter); }
                if (axis > 0) {
                    n += _readCncToken(line+n, token, sizeof(token), _grblInfoTerm);
                    if (!strcmp(token, FST("max_rate_mm_per_min="))) { cncAxis[axis].maxFeed.set(atof(line+n)); }
                    else if (!strcmp(token, FST("max_travel_mm="))) { cncAxis[axis].maxTravel.set(atof(line+n)); }
                    else if (!strcmp(token, FST("homing/"))) {
                        n += _readCncToken(line+n, token, sizeof(token), _grblInfoTerm);
                        DEBUG_printf(FST("Homing Setting for axis %d: %s (%s)\n"), axis, token, line+n);
                    } else { DEBUG_printf(FST("Setting for axis %d: %s (%s)\n"), axis, token, line+n); }
                }
            }            
        }
    } else if (line[1] >= '0' && line[1] <= '9') {
      int par = atoi(line+1);
      n = 1;
      while (line[n] && line[n] != '=') { n++; }
      if (line[n] == '=') { n++; }
      float val = atof(line+n);
      DEBUG_printf(FST("GRBL-parameter: %d : %f\n"), par, val);
      if (par >= 110 && par <= 110 + CNC_MAX_AXES) {
        cncAxis[par-110].maxFeed.set(val);
      }
      else if (par >= 120 && par <= 120 + CNC_MAX_AXES) {
        cncAxis[par-120].maxTravel.set(val);
      }
      if (par == 120) {
        cncSetCncMachineType(CMT_GRBL);
      }
    }
  }
}


/*==========================================================*\
 * Handle incoming "<" status line from GRBL controller
\*==========================================================*/

/*
XYZABC Limit switches
P Probe Switch
R Reset Switch
S Cycle start switch
F Feed hold switch
D Door switch
0123 Macro switch status
*/
void _grblHandleStatusLine(const char* line) {
    // DEBUG_printf(FST("RT: %d\n"), millis() - _cncStatusCheckTs);
    _cncStatusResponseTs = millis();
    size_t n = 0;
    size_t i = 0;
    typedef enum CncReadStateEnum {PS_START, PS_STATE, PS_STATE_DONE, PS_MPOS, PS_FS, PS_OV, PS_WCO, PS_PN, PS_DONE } CncReadStateEnum;
    CncReadStateEnum state = PS_START;
    size_t p = 0;
    bool gotPinStates = false;
    int maxAxis = -1;
    char token[64];
    
    do {
        i = _readCncToken(line+n, token, sizeof(token), _grblStatusTerm);
        char sep = token[i-1];
        if (i > 1) { token[i-1] = '\0'; }
        if (!strcmp(token, FST("<"))) { state = PS_STATE; }
        else if (state == PS_STATE) { 
            _cncSetState(token);
            if (sep == ':') { i += _readCncToken(line+n+i, token, sizeof(token), _grblStatusTerm); }
            state = PS_STATE_DONE;
        }
        else if (sep == ':') {
            if (!strcmp(token, FST("MPos"))) { state = PS_MPOS; }
            else if (!strcmp(token, FST("WCO"))) { state = PS_WCO; }
            else if (!strcmp(token, FST("FS"))) { state = PS_FS; }
            else if (!strcmp(token, FST("Ov"))) { state = PS_OV; }
            else if (!strcmp(token, FST("Pn"))) { state = PS_PN; }
            else { DEBUG_printf(FST("Unknown CNC info token: %s\n"), token); }
        }
        else if (state == PS_MPOS) {
            if (p < CNC_AXIS_MAX-1) { 
                cncAxis[p].machinePos.set(atof(token));
                maxAxis = std::max(maxAxis, (int)p);
            }
        }
        else if (state == PS_WCO) {
            if (p < CNC_AXIS_MAX-1) { 
                cncAxis[p].workCoordinate.set(atof(token)); 
                maxAxis = std::max(maxAxis, (int)p);
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
            if (p == 0) { 
                stateCncSpeedOverride.set(atof(token)); 
                uiUpdateSettingValue(uiPanelSettingsSpeedOverride, stateCncSpeedOverride.get());
            }
            else if (p == 1) { 
                stateCncRapidsOverride.set(atof(token)); 
                uiUpdateSettingValue(uiPanelSettingsRapidsOverride, stateCncRapidsOverride.get());
            }
            else if (p == 2) { 
                stateCncFeedOverride.set(atof(token)); 
                uiUpdateSettingValue(uiPanelSettingsFeedOverride, stateCncFeedOverride.get());
            }
        }
        else if (state == PS_PN) {
            gotPinStates = true;
            if (!strcmp(token, cncPinStates)) { 
                lv_label_set_text(uiCncPinLabel, token); 
                lv_obj_set_style_text_color(uiCncPinLabel, lv_color_hex(0xFFFF20), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(uiCncPinLabel, lv_color_hex(0x4040D0), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            strncpy(cncPinStates, token, sizeof(cncPinStates)-1);
            cncPinStates[sizeof(cncPinStates)-1] = '\0';
        }
        else { DEBUG_printf(FST("Unknown CNC state token:%s sep=%c state=%d\n"), token, sep, state); }
        if (sep == ',') { p++; }
        else { p = 0; }

        // DEBUG_print(token); DEBUG_print(" "); DEBUG_println(term);
        n += i;
    } while (i && line[n]);
    if (!gotPinStates) {
        if (cncPinStates[0] != '-') {
            cncPinStates[0] = '-';
            cncPinStates[1] = '\0';
            lv_label_set_text(uiCncPinLabel, cncPinStates);
            lv_obj_set_style_text_color(uiCncPinLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(uiCncPinLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    for (int a=0; a<=maxAxis; a++) { cncAxis[a].showCoordinates(); }
    if (stateCncConnectionState.get() == CCS_TIMEOUT) { cncSetConnectionState(CCS_CONNECTED); }
}

void _grblHandleMessageLine(const char* line) {
    showMessageToast(line, configShowMessageToastMs.get());
}

void _grblHandleErrorLine(const char* line) {
    _cncCmdResponseTs = millis();
    showErrorToast(line, configShowErrorToastMs.get());
    getErrorCode=atoi(line);
    char buffer[32];
    snprintf(buffer, sizeof(buffer)-1, FST("$E=%s"), line);
    cncSend(buffer);
}

void parseGrblResponse(const char* line) {
    if (line[0] == '<') { _grblHandleStatusLine(line); }
    else if (line[0] == '$') { _grblHandleConfigLine(line); }
    else if (line[0] == '[') { _grblHandleMessageLine(line); }
    else if (getErrorCode && line[0]>='0' && line[0]<='9') {
        /*
        char token[64];
        size_t n = _readCncToken(line, token, sizeof(token));
        n = _readCncToken(line+n, token, sizeof(token));
        */
        showErrorToast(line, configShowErrorToastMs.get());
        getErrorCode = 0;
    }
    else {
        char token[32];
        size_t n = _readCncToken(line, token, sizeof(token), _grblStatusTerm);
        if (!strcasecmp(token, FST("error:"))) {
            if (cncResponseCnt < cncCmdCnt) { cncResponseCnt++; }
            cncResponseError = true;
            _readCncToken(line+n, token, sizeof(token), _grblStatusTerm);
            _grblHandleErrorLine(token);
        }
    }
}
