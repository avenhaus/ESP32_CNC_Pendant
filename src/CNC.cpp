#include <Arduino.h>
#include "Config.h"
#include "VUEF.h"
#include "ui.h"
#include "CNC.h"

Stream* cncStream = &Serial;
uint32_t cncTs_ = 0;


float jogSpeed[CNC_AXIS_MAX] = {0.0, 1000.0, 1000.0, 100.0, 10.0};
float jogDistance[CNC_AXIS_MAX] = {0.0, 10.0, 10.0, 10.0, 10.0};
CncAxis currentAxis = CNC_AXIS_X;
const char CNC_AXIS_LETTER[] PROGMEM = "-XYZR";

void cncSend(const char* text) {
    if (cncStream) { cncStream->println(text); }
    if (debugStream && cncStream != debugStream) { debugStream->println(text); }
}

void cncSendCmdJog(CncAxis axis, float speed, float distance) {
    if (axis == CNC_AXIS_NONE) { return; }
    char buffer[64];
    size_t n = snprintf(buffer, sizeof(buffer)-2, FST("$J=G91 G21 F%g %c%g"), speed, CNC_AXIS_LETTER[axis], distance);
    buffer[n] = 0;
    return cncSend(buffer);
}


void cncInit() {

}

void cncRun(uint32_t now) {
  if (now == 0) { now = millis(); }
  if (now < cncTs_ + CNC_RUN_MS) { return; }
  cncTs_ = now;

}


void cncJogCurrentAxis(int32_t steps) {
    cncSendCmdJog(currentAxis, jogSpeed[currentAxis], jogDistance[currentAxis] * steps);
}

void cncChangeStep(int32_t steps) {
    if (steps > 0 ) { jogDistance[currentAxis] *= 10.0; }
    else if (steps < 0 ) { jogDistance[currentAxis] /= 10.0; }
    DEBUG_printf(FST("Axis %c step size: %g\n"), CNC_AXIS_LETTER[currentAxis], jogDistance[currentAxis]);

   char buffer[32];
   sprintf(buffer, FST("%g"), jogDistance[currentAxis]);   
   lv_label_set_text(uiPanelJogDistance.text, buffer);
}

void cncChangeSpeed(int32_t steps) {
    if (steps > 0 ) { jogSpeed[currentAxis] *= 10.0; }
    else if (steps < 0 ) { jogSpeed[currentAxis] /= 10.0; }
    DEBUG_printf(FST("Axis %c speed: %g\n"), CNC_AXIS_LETTER[currentAxis], jogSpeed[currentAxis]);

   char buffer[32];
   sprintf(buffer, FST("%g"), jogSpeed[currentAxis]);   
   lv_label_set_text(uiPanelJogSpeed.text, buffer);
}
