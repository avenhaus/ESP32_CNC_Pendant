#ifndef _ANALOG_H
#define _ANALOG_H

#include <Arduino.h>
#include <Helper.h>
// #include <SimpleKalmanFilter.h>


void adc2RegSave();
void adc2RegPrepare();
void adc2RegRestore();
int safeAnalogRead(uint8_t pin);

class Joystick {
public:
    Joystick(int8_t pin_,
        uint16_t deadBand_ = 32,
        uint16_t centerVal_ = 0xFFFF,
        uint16_t minVal_ = 30,
        uint16_t maxVal_ = 4095,
        float fc_ = -1.0);

    void calibrate();
    float read();

    int8_t pin;
    uint16_t deadBand;
    uint16_t centerVal;
    uint16_t minVal;
    uint16_t maxVal;
    int16_t raw;
    float value;
    float fc;
    float fvalue;
    RolingAcc acc;
    //SimpleKalmanFilter kalmanFilter;
    
};


class Potentiometer {
public:
    Potentiometer(int8_t pin_,
        uint16_t minVal_ = 0,
        uint16_t maxVal_ = 4095,
        float fc_ = -1.0);

    float read();

    int8_t pin;
    uint16_t minVal;
    uint16_t maxVal;
    int16_t raw;
    float value;
    float fc;
    float fvalue;
    RolingAcc acc;
    //SimpleKalmanFilter kalmanFilter;
};


#endif // _ANALOG_H
