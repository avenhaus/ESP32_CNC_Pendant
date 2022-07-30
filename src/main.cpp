#include <Arduino.h>
#include "Config.h"
#include "VUEF.h"
#include "SPIFFS.h"
#include "Display.h"
#include "Analog.h"
#include "Battery.h"
#include "Encoder.h"
#include "CNC.h"

// #include <ESP32Encoder.h>


void extendedInputSetup();
uint32_t getExtendedInputs();
extern uint32_t extended_inputs;
uint32_t extended_inputs_last = 0;
uint32_t extended_inputs_debounce = 0;

// ESP32Encoder encoderMainDial;
Encoder encoderMain;
Encoder encoderLeft;
Encoder encoderRight;

Joystick leftJoyX(L_JOY_X_PIN);
Joystick leftJoyY(L_JOY_Y_PIN);
Joystick rightJoyX(R_JOY_X_PIN);
Joystick rightJoyY(R_JOY_Y_PIN);

float joyAxes[JOY_AXIS_SIZE] = {0};

uint32_t analogReadTs = 0;

uint32_t inputSameCount = 0;

void _inputTask(void* pvParameters) {
    while(true) {
        encoderMain.update(digitalRead(ENCODER0_B_PIN)<<1 | digitalRead(ENCODER0_A_PIN));
        getExtendedInputs();
        encoderLeft.update((extended_inputs >> LEFT_ENCODER1_A_BIT) & 3);
        encoderRight.update((extended_inputs >> RIGHT_ENCODER1_A_BIT) & 3);
        vTaskDelay(1);
    }
 
}

void setup() {
adcInit();
  adc2RegSave(); // Save ADC2 registers before WiFi

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  

  
  vuefInit();

  // This must be executed before Display initialization.
  // Otherwise SPI gets messed up for some reason.
  extendedInputSetup();

  #if ENABLE_DISPLAY
  displaySetup();
  displayBootScreen();  
  DEBUG_print(F("Screen Done\n"));
  #endif
  
  // delay(5000);

  #if ENABLE_DISPLAY
  mainScreen();
  #endif

  #if 1  
  pinMode(ENCODER0_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER0_B_PIN, INPUT_PULLUP);
  #else
  ESP32Encoder::useInternalWeakPullResistors=UP;
  //	encoderMainDial.attachFullQuad(ENCODER0_A_PIN, ENCODER0_B_PIN);
  //	encoderMainDial.attachHalfQuad(ENCODER0_A_PIN, ENCODER0_B_PIN);
  encoderMainDial.attachSingleEdge(ENCODER0_A_PIN, ENCODER0_B_PIN);
  //encoderMainDial.setFilter(1024);
  encoderMainDial.clearCount();
  #endif

  cncInit();

  xTaskCreatePinnedToCore(
    _inputTask,     // Task function
    "net",          // String with name of task
    2048,           // Stack size in bytes
    NULL,           // Parameter passed as input of the task
    1,              // Priority of the task.
    NULL,           // Task handle.
    1);             // Core
}

void loop() {
  uint32_t now = millis();
  vuefRun(now);
  cncRun(now);
  #if ENABLE_DISPLAY
  guiRun(now);
  #endif

  int steps = encoderMain.getChagne();
  if (steps) { cncJogAxis(steps); }
  steps = encoderLeft.getChagne();
  if (steps) { 
    if (EX_INPUT(LEFT_ENCODER1_BUTTON_BIT)) { cncIncSettingsEncoder(steps); }
    else { cncChangeSettingsEncoderMode(steps); }
  }
  steps = encoderRight.getChagne();
  if (steps) { cncIncActiveAxis(steps); }
  
  if (extended_inputs != extended_inputs_debounce) { 
    inputSameCount = 0;
    extended_inputs_debounce = extended_inputs;
  }
  else { inputSameCount++; }
  if (inputSameCount == 5) { // Simple debounce
    uint32_t justPressed = (extended_inputs ^ extended_inputs_last) & ~extended_inputs;
    if (justPressed) {
      DEBUG_printf(FST("Pressed: %04X\n"), justPressed);
      if (justPressed & (1 << RIGHT_ENCODER1_BUTTON_BIT)) { cncAxisEncoderPress(); }
    }

    float jjMode = !EX_INPUT(SLOW_BUTTON_BIT) ? 0.01 : 0.1;
    if (!EX_INPUT(FAST_BUTTON_BIT)) { jjMode *= 10.0; }
    if (jjMode != joyJogFeedMode.get()) {
        DEBUG_printf(FST("JJ Feed Mode: %0.3f\n"), jjMode);
        joyJogFeedMode.set(jjMode);
    }

    extended_inputs_last = extended_inputs;
  }

  if (now - analogReadTs >= 2) {
    analogReadTs = now;
    
    joyAxes[L_JOY_AXIS_X] = leftJoyX.read();
    joyAxes[L_JOY_AXIS_Y] = leftJoyY.read();
    joyAxes[R_JOY_AXIS_X] = rightJoyX.read();
    joyAxes[R_JOY_AXIS_Y] = rightJoyY.read();

    cncAxis[0].throttle.set(joyAxes[L_JOY_AXIS_X]);
    cncAxis[1].throttle.set(joyAxes[L_JOY_AXIS_Y]);
    cncAxis[2].throttle.set(joyAxes[R_JOY_AXIS_Y]);

    //DEBUG_printf(FST("Analog: LX %d  %.3f %.3f  LP %d  %.3f %.3f  LY %d   RX %d  RY %d  RP %d\n"), leftJoyX.raw, leftJoyX.value, leftJoyX.fvalue, leftPot1.raw, leftPot1.value, leftPot1.fvalue, leftJoyY.raw, rightJoyX.raw, rightJoyY.raw, rightPot1.raw);
    //DEBUG_printf(FST("Analog: LY %d  %.3f %.3f \n"), leftJoyY.raw, leftJoyY.value, leftJoyY.fvalue);
    // DEBUG_printf(FST("Analog: LX %4d  %.3f | LY %4d  %.3f || RX %4d  %.3f | RY %4d  %.3f |\n"), leftJoyX.raw, leftJoyX.fvalue, leftJoyY.raw, leftJoyY.fvalue, rightJoyX.raw, rightJoyX.fvalue, rightJoyY.raw, rightJoyY.fvalue);

  }
 
  #if BATTERY_PIN >= 0 
  batteryRun(now);
  if (batteryChargeLevel <= 2 && (batteryVoltage > -1.0)) {
    DEBUG_println(FST("Battery is low!"));
    //shutdown();
    Serial.flush(); 
    esp_deep_sleep_start();
  }
  #endif
  vTaskDelay(1);
}