#include <Arduino.h>
#include "Encoder.h"

// Encoder Lookup table
static const int8_t ENCODER_STATES[] PROGMEM = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; 

// KY-040 generates pulses of about 5ms and as low as 2ms.
// KY-040 generates a lot of noise. The lookup table helps to deal with that.
int32_t Encoder::update(uint8_t inputAB) {
  _oldAB <<=2;        // Remember previous state
  _oldAB |= inputAB & 3;  // Update with new state

  _state += ENCODER_STATES[( _oldAB & 0x0F )];

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( _state > 3 ) {        // Four steps forward
    counter++;              // Increase counter
    delta = 1;
    _state = 0;
  } else if( _state < -3 ) {  // Four steps backwards
    counter--;               // Decrease counter
    delta = -1; 
    _state = 0;
  } else { delta = 0; }
  return counter;
}