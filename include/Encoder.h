#ifndef _ENCODER_H
#define _ENCODER_H

#include <arduino.h>

class Encoder {
public:
    Encoder() : counter(0), _oldAB(0), _state(0), _pinA(-1), _pinB(-1), _isIsrAttached(false) {}
    int32_t update(uint8_t inputAB);
    int getChagne() { int tmp = dcnt; dcnt = 0; return tmp; }
    void attachInterrupt(int pinA, int pinB);

    static void IRAM_ATTR encoderISR(void * arg);

    int32_t counter;      // Encoder value
    int delta;
    int dcnt;

private:
    uint8_t _oldAB;     // Lookup table index
    int8_t _state;
    int8_t _pinA;
    int8_t _pinB;
    bool _isIsrAttached;
};

#endif // _ENCODER_H
