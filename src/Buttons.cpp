#include "Config.h"
#include "CNC.h"
#include "Ui.h"
#include "Buttons.h"

RegGroup configGroupButtons(FST("Buttons"));

const ConfigEnum::Option configButtonFunctionOptions[] PROGMEM = {
  { "None", CBF_NONE },

  { "ZERO_ALL", CBF_ZERO_ALL },
  { "ZERO_CURRENT", CBF_ZERO_CURRENT },
  { "ZERO_X", CBF_ZERO_X },
  { "ZERO_Y", CBF_ZERO_Y },
  { "ZERO_Z", CBF_ZERO_Z },
  { "ZERO_A", CBF_ZERO_A },
  { "ZERO_B", CBF_ZERO_B },
  { "ZERO_C", CBF_ZERO_C },

  { "HOME_ALL", CBF_HOME_ALL },
  { "HOME_CURRENT", CBF_HOME_CURRENT },
  { "HOME_X", CBF_HOME_X },
  { "HOME_Y", CBF_HOME_Y },
  { "HOME_Z", CBF_HOME_Z },
  { "HOME_A", CBF_HOME_A },
  { "HOME_B", CBF_HOME_B },
  { "HOME_C", CBF_HOME_C },

  { "CONTROL_START", CBF_CONTROL_START },
  { "CONTROL_PAUSE", CBF_CONTROL_PAUSE },
  { "CONTROL_RESET", CBF_CONTROL_RESET },
  { "CONTROL_REPEAT", CBF_CONTROL_REPEAT },
  { "ALARM_RESET", CBF_ALARM_RESET },
  { "CNC_REBOOT", CBF_CNC_REBOOT },
  { "PENDANT_REBOOT", CBF_PENDANT_REBOOT },
  { "PENDANT_LOCK", CBF_PENDANT_LOCK },
  
  { "MENU", CBF_MENU },
  { "MENU_UP", CBF_MENU_UP },
  { "MENU_DOWN", CBF_MENU_DOWN },
  { "MENU_SELECT", CBF_MENU_SELECT },
  { "MENU_CANCEL", CBF_MENU_CANCEL },
  { "PENDANT_FILES", CBF_PENDANT_FILES },
  { "CNC_FILES", CBF_CNC_FILES },
 

  { "MOVE_SLOW", CBF_MOVE_SLOW },
  { "MOVE_FAST", CBF_MOVE_FAST },
  { "JOG_POSITIVE", CBF_JOG_POSITIVE },
  { "JOG_NEGATIVE", CBF_JOG_NEGATIVE },
  { "AXIS_INC", CBF_AXIS_INC },
  { "AXIS_DEC", CBF_AXIS_DEC },
  { "AXIS_RELATIVE_POS", CBF_AXIS_RELATIVE_POS },
  { "AXIS_WORK_POS", CBF_AXIS_WORK_POS },
  { "AXIS_MACHINE_POS", CBF_AXIS_MACHINE_POS },
  { "SETTING_NEXT", CBF_SETTING_NEXT },
  { "SETTING_PREV", CBF_SETTING_PREV },
  { "SETTING_INC", CBF_SETTING_INC },
  { "SETTING_DEC", CBF_SETTING_DEC },
  { "SETTING_GET", CBF_SETTING_GET },

  { "SPINDLE", CBF_SPINDLE },
  { "MIST", CBF_MIST },
  { "FLOOD", CBF_FLOOD },
  { "VACUUM", CBF_VACUUM },
  { "CLAMP", CBF_CLAMP },

  { "PROBE", CBF_PROBE },
  { "TOOL", CBF_TOOL },
  { "MACRO_MENU", CBF_MACRO_MENU },
  { "MACRO_RECORD", CBF_MACRO_RECORD },
  { "MACRO_SAVE", CBF_MACRO_SAVE },
  { "MACRO_0", CBF_MACRO_0 },
  { "MACRO_1", CBF_MACRO_1 },
  { "MACRO_2", CBF_MACRO_2 },
  { "MACRO_3", CBF_MACRO_3 },
  { "MACRO_4", CBF_MACRO_4 },
  { "MACRO_5", CBF_MACRO_5 },
  { "MACRO_6", CBF_MACRO_6 },
  { "MACRO_7", CBF_MACRO_7 },
  { "MACRO_8", CBF_MACRO_8 },
  { "MACRO_9", CBF_MACRO_9 },
};
const size_t configButtonFunctionOptionsSize = sizeof(configButtonFunctionOptions) / sizeof(ConfigEnum::Option);
const char* _cncButtonHelp = FST("Function that is executed when this button is pressed.");
ConfigEnum configButtonLY(FST("Left Yellow"), configButtonFunctionOptions, configButtonFunctionOptionsSize, CBF_NONE, _cncButtonHelp, 0, &configGroupButtons);
ConfigEnum configButtonLB(FST("Left Blue"), configButtonFunctionOptions, configButtonFunctionOptionsSize, CBF_NONE, _cncButtonHelp, 0, &configGroupButtons);
ConfigEnum configButtonLR(FST("Left Red"), configButtonFunctionOptions, configButtonFunctionOptionsSize, CBF_NONE, _cncButtonHelp, 0, &configGroupButtons);
ConfigEnum configButtonLG(FST("Left Green"), configButtonFunctionOptions, configButtonFunctionOptionsSize, CBF_NONE, _cncButtonHelp, 0, &configGroupButtons);

const char * _uiButtonCmd[] = {
    CNC_CMD_HOME_ALL, // UIB_HOME
    CNC_CMD_ZERO_ALL, // UIB_ZERO
    nullptr, // UIB_FILES
    nullptr, // UIB_SETTINGS
    nullptr, // UIB_HELP
    CNC_CMD_CONTROL_START, // UIB_PLAY
    CNC_CMD_CONTROL_PAUSE, // UIB_PAUSE
    CNC_CMD_CONTROL_RESET, // UIB_STOP
    nullptr, // UIB_SPINDLE
    nullptr, // UIB_FLOOD
    nullptr, // UIB_MIST 
    nullptr, // UIB_PROBE
    nullptr, // UIB_MACRO
    CNC_CMD_RESTART, // UIB_RESTART
};


/* ============================================== *\
 * Event Callbacks
\* ============================================== */

static void _uiButtonPressed(lv_event_t* e) {
    lv_event_code_t event = lv_event_get_code(e);
    if(event != LV_EVENT_RELEASED) { return; }
    lv_obj_t* ta = lv_event_get_target(e);
    uint32_t uib = (uint32_t) lv_event_get_user_data(e);
    DEBUG_printf(FST("UI Button: %d\n"), uib);
    if (_uiButtonCmd[uib]) { cncSend(_uiButtonCmd[uib]); }
}

/*==========================================================*\
 * Functions
\*==========================================================*/

void buttonsInit() {
    for (int i=0; i<UIB_MAX; i++) {
        if (uiButton[i]) { lv_obj_add_event_cb(uiButton[i], _uiButtonPressed, LV_EVENT_RELEASED, (void*) i); }
    }
}