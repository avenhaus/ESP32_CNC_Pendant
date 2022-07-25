#ifndef _VUEF_CONF_H_
#define _VUEF_CONF_H_

#include <Arduino.h>

/* ============================================== *\
 * Constants
\* ============================================== */

#define __HOSTNAME__ "cnc-pendant";
#define __PROJECT_NAME__ "ESP32 CNC Pendant";
#define __PROJECT_COMPILE__ "0.2";

#define SERIAL_DEBUG 1
#define SERIAL_SPEED 115200

#define HTTP_PORT 80
#define TELNET_PORT 23

#define COMMAND_BUFFER_SIZE 64

#define ENABLE_WIFI 0
#define ENABLE_OTA 1
#define ENABLE_TELNET 1
#define ENABLE_WEB_SERVER 1
#define WEBUI_TABS "control,settings,files,wizard"
#define WEBUI_START_TAB "settings"
#define ENABLE_EMBEDDED_WEB_UI 1
#define ENABLE_SPIFFS_WEB_UI 1
#define ENABLE_SPIFFS_WEB_CONTENT 0
#define ENABLE_MDNS 1
#define ENABLE_NTP 1
#define ENABLE_SPIFFS 1

#define ENABLE_STATE_REG 1
#define ENABLE_WS_STATE_MONITOR 1
// #define USE_NETWORK_TASK 1

// PST8PDT,M3.2.0,M11.1.0
#define TIME_ZONE "PST8PDT,M3.2.0/2:00:00,M11.1.0/2:00:00"
#define TIME_FORMAT "%H:%M:%S"
#define DATE_FORMAT "%d.%m.%Y"

// "time.nist.gov"
#define NTP_SERVER_1 "132.163.96.1"
// "time.windows.com"
#define NTP_SERVER_2 "13.65.245.138"


#endif // _VUEF_CONF_H_
