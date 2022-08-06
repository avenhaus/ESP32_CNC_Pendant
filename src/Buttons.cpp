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
  { "PENDANT_FILES", CBF_PENDANT_FILES },
  { "CNC_FILES", CBF_CNC_FILES },
  { "MENU", CBF_MENU },

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
ConfigEnum configButtonLY(FST("Left Yellow"), configButtonFunctionOptions, configButtonFunctionOptionsSize, 1, _cncButtonHelp, 0, &configGroupButtons);
ConfigEnum configButtonLB(FST("Left Blue"), configButtonFunctionOptions, configButtonFunctionOptionsSize, 1, _cncButtonHelp, 0, &configGroupButtons);
ConfigEnum configButtonLR(FST("Left Red"), configButtonFunctionOptions, configButtonFunctionOptionsSize, 1, _cncButtonHelp, 0, &configGroupButtons);
ConfigEnum configButtonLG(FST("Left Green"), configButtonFunctionOptions, configButtonFunctionOptionsSize, 1, _cncButtonHelp, 0, &configGroupButtons);

