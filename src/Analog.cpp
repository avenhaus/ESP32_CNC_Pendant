#include "Analog.h"

/*=====================================================================*\
 | ESP32 measurable input voltage range:
 | -----------------+-----------------
 | ADC_ATTEN_DB_0   | 100 mV ~  950 mV
 | ADC_ATTEN_DB_2_5 | 100 mV ~ 1250 mV
 | ADC_ATTEN_DB_6   | 150 mV ~ 1750 mV
 | ADC_ATTEN_DB_11  | 150 mV ~ 2450 mV
 | -----------------+-----------------
 |
 | Thumb Joystick is 5K (10K each?). It has a fairly big dead zone on
 | the outsides so small actual analog movement range.
 |
 | * Experimental: Drop across joystick should be  3.15mV - 150mV = 3.0V
 | * Experimental: R1 (GND) is 180 Ohm and R2 (+3.3V) is 220 Ohm
 | 
 | # Theory:
 | # * Drop across joystick should be  2450mV - 150mV = 2.3V
 | # => For 3.3V Vcc: R1 + R2 = 5K / 2.3 = 2174 Ohm
 | # => R1 = 326 Ohm | R2 = 1848 Ohm
 | # => 326 + 5000 + 1848 = 7174 => 0.15V + 2.3V + 0.85V = 3.3V
 | 
 | * 10K Pot: R1+R2 = 10K / 2.3 = 4.348K; R1 = 652; R2 = 3.696
 *=====================================================================*
 | ADC2 is used by WIFI and needs some extra logic to deal with conflict.
 | ADC2 GPIO: 0, 2, 4, 12, 13, 14, 15, 25, 26, 27
\*=====================================================================*/

Joystick::Joystick(int8_t pin_, uint16_t deadBand_, uint16_t centerVal_, uint16_t minVal_, uint16_t maxVal_, float fc_) :
        pin(pin_), deadBand(deadBand_), centerVal(centerVal_), minVal(minVal_), maxVal(maxVal_), fc(fc_) {
    fvalue = 0.0;
}

// Returns values between -1.0 and +1.0
float Joystick::read() {
    if (pin < 0) { return 0.0; }
    if (centerVal == 0xFFFF) { calibrate(); }
    raw = analogRead(pin);
    if (raw > centerVal + deadBand) {
        value = (float) (raw - (centerVal + deadBand)) / float(maxVal - (centerVal + deadBand));
    } else if (raw < centerVal - deadBand) {
        value = (float) -(centerVal - deadBand - raw) / float(centerVal - deadBand - minVal);
    } else {
        value = 0.0;
    }
    value = CLAMP(value, -1.0, 1.0);
    if (fc == 0.0) { return fvalue = value; }
    else if (fc > 0.0 && fc < 0.5) { fvalue = fvalue * (1.0 - fc) + value * fc; }
    else { fvalue = acc.avg((float) value); }
    return fvalue;
}

void Joystick::calibrate() {
    if (pin < 0) { return; }
    uint16_t acc = 0;
    for (int i=0; i<16; i++) { 
        acc += analogRead(pin);
        vTaskDelay(1);
    }
    centerVal = acc >> 4;
}

Potentiometer::Potentiometer(int8_t pin_, uint16_t minVal_, uint16_t maxVal_, float fc_) :
        pin(pin_), minVal(minVal_), maxVal(maxVal_), fc(fc_)  {
    fvalue = 0.0;
}

// Returns values between 0.0 and +1.0
float Potentiometer::read() {
    if (pin < 0) { return 0.0; }
    raw = analogRead(pin);
    value = (float) (raw - minVal) / float(maxVal - minVal);
    if (fc == 0.0) { return fvalue = value; }
    else if (fc > 0.0 && fc < 0.5) { fvalue = fvalue * (1.0 - fc) + value * fc; }
    else { fvalue = acc.avg((float) value); }
    return fvalue;
}

// Kludge to fix ADC2 + WiFi
#include "driver/adc.h"
#include "soc/sens_reg.h"    // needed for manipulating ADC2 control register
uint64_t adc2Reg1Pre_, adc2Reg1Post_;
uint64_t adc2Reg2Pre_, adc2Reg2Post_;
uint64_t adc2Reg3Pre_, adc2Reg3Post_;

int safeAnalogRead(uint8_t pin) {
  if (pin < 32) { adc2RegPrepare(); }
  int raw = analogRead(pin); 
  // int raw = -1;
  // esp_err_t e = adc2_get_raw(ADC2_CHANNEL_8, ADC_WIDTH_BIT_12, &raw);
  if (pin < 32) { adc2RegRestore(); }
  return raw;
}

// Saves ADC2 registers prior to WiFi. Must be called before WiFi is intialized
void adc2RegSave() {
  adc2Reg1Pre_ = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
  adc2Reg2Pre_ = READ_PERI_REG(SENS_SAR_START_FORCE_REG);
  adc2Reg3Pre_ = READ_PERI_REG(SENS_SAR_MEAS_START2_REG);
}

void adc2RegPrepare() {
  adc2Reg1Post_ = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, adc2Reg1Pre_);
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);

  adc2Reg2Post_ = READ_PERI_REG(SENS_SAR_START_FORCE_REG);
  WRITE_PERI_REG(SENS_SAR_START_FORCE_REG, adc2Reg2Pre_);

  adc2Reg3Post_ = READ_PERI_REG(SENS_SAR_MEAS_START2_REG);
  WRITE_PERI_REG(SENS_SAR_MEAS_START2_REG, adc2Reg3Pre_);
}

void adc2RegRestore() {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, adc2Reg1Post_);
  WRITE_PERI_REG(SENS_SAR_START_FORCE_REG, adc2Reg2Post_);
  WRITE_PERI_REG(SENS_SAR_MEAS_START2_REG, adc2Reg3Post_);
}
