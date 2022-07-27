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
extern uint32_t old_extended_inputs;

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
}

void loop() {
  uint32_t now = millis();
  vuefRun(now);
  cncRun(now);
  #if ENABLE_DISPLAY
  guiRun();
  #endif
 
  getExtendedInputs();  
  if (extended_inputs != old_extended_inputs) { inputSameCount = 0; }
  else { inputSameCount++; }
  if (inputSameCount == 5) { // Simple debounce

    encoderLeft.update((extended_inputs >> LEFT_ENCODER1_A_BIT) & 3);
    if (encoderLeft.delta) { cncIncJogStep(encoderLeft.delta); }
    //!EX_INPUT(LEFT_ENCODER1_BUTTON_BIT);
    encoderRight.update((extended_inputs >> RIGHT_ENCODER1_A_BIT) & 3);
    if (encoderRight.delta) { cncIncJogFeed(encoderRight.delta); }
    //!EX_INPUT(RIGHT_ENCODER1_BUTTON_BIT);

    // DEBUG_printf(FST("Inputs: %08X  L:%d  R:%d \n"), ~extended_inputs, encoderLeft.counter, encoderRight.counter);
    //DEBUG_printf(FST("Inputs: %04X\n"), extended_inputs);
  }

  if (now - analogReadTs >= 2) {
    analogReadTs = now;
    
    joyAxes[L_JOY_AXIS_X] = leftJoyX.read();
    joyAxes[L_JOY_AXIS_Y] = leftJoyY.read();

    joyAxes[R_JOY_AXIS_X] = rightJoyX.read();
    joyAxes[R_JOY_AXIS_Y] = rightJoyY.read();
    //DEBUG_printf(FST("Analog: LX %d  %.3f %.3f  LP %d  %.3f %.3f  LY %d   RX %d  RY %d  RP %d\n"), leftJoyX.raw, leftJoyX.value, leftJoyX.fvalue, leftPot1.raw, leftPot1.value, leftPot1.fvalue, leftJoyY.raw, rightJoyX.raw, rightJoyY.raw, rightPot1.raw);
    //DEBUG_printf(FST("Analog: LY %d  %.3f %.3f \n"), leftJoyY.raw, leftJoyY.value, leftJoyY.fvalue);
    //DEBUG_printf(FST("Analog: LX %4d  %.3f | LY %4d  %.3f | LP %4d  %.3f || RX %4d  %.3f | RY %4d  %.3f | RP %4d  %.3f\n"), leftJoyX.raw, leftJoyX.fvalue, leftJoyY.raw, leftJoyY.fvalue, leftPot1.raw, leftPot1.fvalue, rightJoyX.raw, rightJoyX.fvalue, rightJoyY.raw, rightJoyY.fvalue, rightPot1.raw, rightPot1.fvalue);

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

  encoderMain.update(digitalRead(ENCODER0_B_PIN)<<1 | digitalRead(ENCODER0_A_PIN));
  //int64_t val = encoderMainDial.getCount(); 
  if (encoderMain.delta) { cncJogAxis(encoderMain.delta); }
}