#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "Arduino.h"
#include "VUEF.h"


/* ============================================== *\
 * Enum
\* ============================================== */

typedef enum CncButtonFunctionEnum { 
CBF_NONE, 

CBF_ZERO_ALL,
CBF_ZERO_CURRENT,
CBF_ZERO_X,
CBF_ZERO_Y,
CBF_ZERO_Z,
CBF_ZERO_A,
CBF_ZERO_B,
CBF_ZERO_C,

CBF_HOME_ALL,
CBF_HOME_CURRENT,
CBF_HOME_X,
CBF_HOME_Y,
CBF_HOME_Z,
CBF_HOME_A,
CBF_HOME_B,
CBF_HOME_C,

CBF_CONTROL_START,
CBF_CONTROL_PAUSE,
CBF_CONTROL_RESET,
CBF_CONTROL_REPEAT,
CBF_ALARM_RESET,
CBF_CNC_REBOOT,
CBF_PENDANT_REBOOT,
CBF_PENDANT_LOCK,

CBF_MENU,
CBF_MENU_UP,
CBF_MENU_DOWN,
CBF_MENU_SELECT,
CBF_MENU_CANCEL,
CBF_PENDANT_FILES,
CBF_CNC_FILES,

CBF_MOVE_SLOW,
CBF_MOVE_FAST,
CBF_JOG_POSITIVE,
CBF_JOG_NEGATIVE,
CBF_AXIS_INC,
CBF_AXIS_DEC,
CBF_AXIS_RELATIVE_POS,
CBF_AXIS_WORK_POS,
CBF_AXIS_MACHINE_POS,
CBF_SETTING_NEXT,
CBF_SETTING_PREV,
CBF_SETTING_INC,
CBF_SETTING_DEC,
CBF_SETTING_GET,

CBF_SPINDLE,
CBF_MIST,
CBF_FLOOD,
CBF_VACUUM,
CBF_CLAMP,

CBF_PROBE,
CBF_TOOL,
CBF_MACRO_MENU,
CBF_MACRO_RECORD,
CBF_MACRO_SAVE,
CBF_MACRO_0,
CBF_MACRO_1,
CBF_MACRO_2,
CBF_MACRO_3,
CBF_MACRO_4,
CBF_MACRO_5,
CBF_MACRO_6,
CBF_MACRO_7,
CBF_MACRO_8,
CBF_MACRO_9,
} CncButtonFunctionEnum;


/*==========================================================*\
 * Functions
\*==========================================================*/

void buttonsInit();


#endif // _BUTTONS_H_