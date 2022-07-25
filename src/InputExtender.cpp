#include <arduino.h>
#include <spi.h>
#include "Config.h"

uint32_t extended_inputs = 0;
uint32_t old_extended_inputs = 0;

#ifdef INPUT_SPI_CHANNEL
SPIClass INPUT_SPI(INPUT_SPI_CHANNEL);

void extendedInputSetup() {
    pinMode(INPUT_LOAD_PIN, OUTPUT);
    digitalWrite(INPUT_LOAD_PIN, LOW);  
    INPUT_SPI.begin(INPUT_SCK_PIN, INPUT_MISO_PIN, -1);
    INPUT_SPI.setFrequency(INPUT_SPI_SPEED);
    INPUT_SPI.setDataMode(SPI_MODE2);
}

uint32_t getExtendedInputs() {
    digitalWrite(INPUT_LOAD_PIN, HIGH);
    old_extended_inputs = extended_inputs;
    extended_inputs = INPUT_SPI.transfer16(0);
    digitalWrite(INPUT_LOAD_PIN, LOW);  
    return extended_inputs;
}
#endif