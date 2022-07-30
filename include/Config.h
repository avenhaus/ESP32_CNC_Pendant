#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/********************************************\
|*  Pin Definitions
\********************************************/

/*
(2) Left Joy
(2) Right Joy
(2) Main Rotary Dial

(3) Axis Encoder
(3) Speed Encoder
(3) Mode Encoder
(3) Menu Encoder
(1) Fast
(1) Slow
(1) Run
(1) Pause
(1) Repeat
(1) Zero
(1) Spindle
(1) Mist
 Lock

*/

/*
https://drive.google.com/file/d/1gbKM7DA7PI7s1-ne_VomcjOrb0bE2TPZ/view
---+------+----+-----+-----+-----------+---------------------------
No.| GPIO | IO | RTC | ADC | Default   | Function
---+------+----+-----+-----+-----------+---------------------------
25 |   0* | IO | R11 | 2_1 | Boot      | INPUT_CLK
35 |   1  | IO |     |     | UART0_TXD | USB Programming/Debug
24 |   2* | IO | R12 | 2_2 |           | LCD_DC
34 |   3  | IO |     |     | UART0_RXD | USB Programming/Debug
26 |   4* | IO | R10 | 2_0 |           | INPUT_LOAD
29 |   5* | IO |     |     | SPI0_SS   | SD_CS (LED)  
14 |  12* | IO | R15 | 2_5 |           | LCD_LED
16 |  13  | IO | R14 | 2_4 |           | INPUT_IN (or INPUT_CS)
13 |  14  | IO | R16 | 2_6 |           | TOUCH_CS
23 |  15* | IO | R13 | 2_3 |           | LCD_CS
27 |  16+ | IO |     |     | UART2_RXD | CNC_RXD
28 |  17+ | IO |     |     | UART2_TXD | CNC_TXD 
30 |  18  | IO |     |     | SPI0_SCK  | SCK LCD,Touch,SD
31 |  19  | IO |     |     | SPI0_MISO | MISO Touch, SD
33 |  21  | IO |     |     | I2C0_SDA  | Buzzer 
36 |  22  | IO |     |     | I2C0_SCL  | 
37 |  23  | IO |     |     | SPI0_MOSI | MOSI LCD, SD
10 |  25  | IO | R06 | 2_8 |DAC1/I2S-DT| ENCODER0_A
11 |  26  | IO | R07 | 2_9 |DAC2/I2S-WS| ENCODER0_B
12 |  27  | IO | R17 | 2_7 | I2S-BCK   | IN_A
8  |  32  | IO | R09 | 1_4 |           | IN_B
9  |  33  | IO | R08 | 1_5 |           | Battery
6  |  34  | I  | R04 | 1_6 |           | R_JOY_X
7  |  35  | I  | R05 | 1_7 |           | R_JOY_Y
4  |  36  | I  | R00 | 1_0 | SENSOR_VP | L_JOY_X
5  |  39  | I  | R03 | 1_3 | SENSOR_VN | L_JOY_Y
3  |  EN  | I  |     |     | RESET     | Reset LCD       
---+------+----+-----+-----+-----------+---------------------------
(IO6 to IO11 are used by the internal FLASH and are not useable)
22 x I/O  + 4 x input only = 26 usable pins 
GPIO_34 - GPIO_39 have no internal pullup / pulldown.
- ADC2 can not be used with WIFI/BT (easily)
+ GPIO 16 and 17 are not available on WROVER (PSRAM)
* Strapping pins: IO0, IO2, IO4, IO5 (HIGH), IO12 (LOW), IO15 (HIGH)
*/

#define LCD_SCK_PIN 18
#define LCD_MOSI_PIN 23
#define LCD_MISO_PIN 19
#define LCD_RST_PIN -1
#define LCD_DC_PIN 2
#define LCD_CS_PIN 15
#define LCD_LED_PIN 12
#define LCD_LED_PWM_CHANNEL 0

#define TOUCH_CS_PIN 14
#define TOUCH_CS_IRQ -1

#define SD_CS_PIN 5
#define LED_PIN 5

#define BUZZER_PIN 21

#define UART2_RXD 16
#define UART2_TXD 17

#define INPUT_SPI_CHANNEL VSPI
#define INPUT_SPI_SPEED 10000000
#define INPUT_SCK_PIN 0
#define INPUT_LOAD_PIN 4 // 74HC165 HIGH during shifting
#define INPUT_MISO_PIN 13

#define ENCODER0_A_PIN 25
#define ENCODER0_B_PIN 26

#define ADC_VREF 1100

#define BATTERY_PIN 34

#define L_JOY_X_PIN 32 
#define L_JOY_Y_PIN 33 
#define R_JOY_X_PIN 36 
#define R_JOY_Y_PIN 39 

/* ============================================== *\
 * Extended Inputs (from shift registers 74HC165)
\* ============================================== */

#define LEFT_ENCODER1_A_BIT 8
#define LEFT_ENCODER1_B_BIT 9
#define LEFT_ENCODER1_BUTTON_BIT 10

#define RIGHT_ENCODER1_A_BIT 14
#define RIGHT_ENCODER1_B_BIT 15
#define RIGHT_ENCODER1_BUTTON_BIT 13

#define SLOW_BUTTON_BIT 11
#define FAST_BUTTON_BIT 12

#define EX_INPUT(BIT) (BIT < 0 ? 0 : (extended_inputs >> BIT) & 1)


/* ============================================== *\
 * Constants
\* ============================================== */

#define ENABLE_DISPLAY 1

typedef enum JoyAxis {
    L_JOY_AXIS_X,
    L_JOY_AXIS_Y,
    R_JOY_AXIS_X,
    R_JOY_AXIS_Y,
    JOY_AXIS_SIZE
} JoyAxis;
extern float joyAxes[JOY_AXIS_SIZE];

#endif  // CONFIG_H
