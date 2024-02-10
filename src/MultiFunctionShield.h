#ifndef MultiFunctionShield_h
  #define MultiFunctionShield_h

#ifndef __AVR_ATmega328P__  // Arduino UNO, NANO
  #error : Library only for Arduino Uno, Nano and so on with ATmega328
#endif

#include "Arduino.h"
#include <avr/interrupt.h>
//#define FAST_ISR
const uint8_t LED_1_PIN=13;
const uint8_t LED_2_PIN=12;
const uint8_t LED_3_PIN=11;
const uint8_t LED_4_PIN=10;
const uint8_t POT_PIN=0;
const uint8_t BUZZER_PIN=3;
const uint8_t BUTTON_1_PIN=A1;
const uint8_t BUTTON_2_PIN=A2;
const uint8_t BUTTON_3_PIN=A3;
const uint8_t LATCH_PIN=4;
const uint8_t CLK_PIN=7;
const uint8_t DATA_PIN=8;
const uint8_t LM35_PIN=A4;
/* 
*  The following constants are bit-coded. Do not change.
*/
const uint8_t DOT_NONE=0;  
const uint8_t DOT_POS0=1;
const uint8_t DOT_POS1=2;
const uint8_t DOT_POS2=4;
const uint8_t DOT_POS3=8;
const uint8_t DOT_ALL=DOT_POS0|DOT_POS1|DOT_POS2|DOT_POS3;
const uint8_t DOT_OFF=16;
const uint8_t DOT_ON=32;
const uint8_t DOT_KEEP=64;

class MultiFunctionShield
{
  public:
    /**
      @brief  Constructor
      @param  none
      @return none
    */
    MultiFunctionShield(void);

    /**
      @brief  Initialization
      @param  none
      @return none
    */
    void begin(void);

    /**
      @brief  Write a given int value on the display (in the next IRQ)
      @param  value
      @param  dot optional, sset DP(s) Defaults to DOT_NONE (no dot set)
                  can be set to:
                    DOT_NONE (default) to show no dot at all
                    DOT_ALL to lit all dots
                    DOT_POS0 to lit the rightmost dot up to
                    DOT_POS3 to lit the leftmost dot 
                    DOT_POSx values can be ORed to lit any combination of dots,
                    i. e. if set to DOT_POS0 | DOT_POS2 the second dot from left 
                    and the rightmost dot will be lit
      @return none
    */
    void Display (int16_t, uint8_t dot = DOT_NONE);

    /**
      @brief  Switch on the dots on the display ON or OFF
      @param  dot specifies the dot. 0 refers to right dot, up to 3 for left dot
      @param  show specifies if dot should be switched ON (if set to true) or OFF (if set to false) 
                   This parameter is optional and defaults to true if not given.
      @return none
    */
    void DisplayDot (uint8_t dot, bool show = true);

    /**
      @brief  Returns the status of the current dot
      @param  dot specifies the dot. 0 refers to right dot, up to 3 for left dot
      @return 128 if specified dot is on, or 0 if dot is off
    */
    uint8_t GetDot (uint8_t dot);

    /**
      @brief  Set (all/any or none of) the dots on the display 
      @param  dots specifies which dots are to be set
                    DOT_NONE (default) to show no dot at all
                    DOT_ALL to lit all dots
                    DOT_POS0 to lit the rightmost dot up to
                    DOT_POS3 to lit the leftmost dot 
                    DOT_POSx values can be ORed to lit any combination of dots,
                    i. e. if set to DOT_POS0 | DOT_POS2 the second dot from left 
                    and the rightmost dot will be lit
      @return none
    */
    void DisplayDots (uint8_t dots);

    /**
      @brief  Clears the display ("    ");
      @param  none
      @return none
    */
   void Clear(void);

    /**
      @brief  Displays a given bitmap on the display at given position
      @param  pos Position on display (0 == right to 3 == left)
      @param  bmap Bitmap to display
                  Each bit is mapped to a certain LED on the segment
                  b0 corresponds to LED A (top bar) to
                  b7 corresponding to the decimal point (DP)
                  Note that bits are interpreted as not inverted: if
                  a bit is set, the corresponding LED will be lit
      @return none
    */
    void DisplayBitmap (uint8_t pos, uint8_t bmap); 

    /**
      @brief  Displays char on display at given position
      @param  pos Position on display (0 == right to 3 == left)
      @param  c Character to display. Displayable characters are:
                - digits '0' to '9'
                - letters 'a' to 'z' (not case sensitive)
                - minus sign '-' 
                - underline '_'
      @param  dot Bool, if true, show dot. Default is false (no dot)
      @return none
    */
    void DisplayChar (uint8_t pos, char c, uint8_t dotMode = DOT_OFF); 

    /**
      @brief  Displays text on display at given position
      @param  s Text to display
                - maximum the first 4 characters of the text will be displayed
                - if length is shorter than 4 blanks will be shown on left
      @param  dots optional, specifies which dots are to be set
                    DOT_NONE to show no dot at all (default)
                    DOT_ALL to lit all dots
                    DOT_POS0 to lit the rightmost dot up to
                    DOT_POS3 to lit the leftmost dot 
                    DOT_POSx values can be ORed to lit any combination of dots,
                    i. e. if set to DOT_POS0 | DOT_POS2 the second dot from left 
                    and the rightmost dot will be lit
      @return none
    */
    void DisplayText (const char* s, uint8_t dotMode = DOT_OFF); 

    /**
      @brief  Write a given 16-bit value on the display as hexadecimal number
              Display is right aligned, left padding with blanks
      @param  value 16 bit value to be displayed as hex
      @param  dot optional parameter to set DP(s) (defaults to DOT_NONE)
                  can be set to:
                    DOT_NONE (default) to show no dot at all
                    DOT_ALL to lit all dots
                    DOT_POS0 to lit the rightmost dot up to
                    DOT_POS3 to lit the leftmost dot 
                    DOT_POSx values can be ORed to lit any combination of dots,
                    i. e. if set to DOT_POS0 | DOT_POS2 the second dot from left 
                    and the rightmost dot will be lit
      @return none
    */
    void DisplayHexValue(uint16_t value, uint8_t dotMode = DOT_OFF);

    /**
      @brief  Write a given 16-bit value on the display as hexadecimal number
              Display is right aligned, left padding with Zeros
      @param  value 16 bit value to be displayed as hex
      @param  dot optional parameter to set DP(s) (defaults to DOT_NONE)
                  can be set to:
                    DOT_NONE (default) to show no dot at all
                    DOT_ALL to lit all dots
                    DOT_POS0 to lit the rightmost dot up to
                    DOT_POS3 to lit the leftmost dot 
                    DOT_POSx values can be ORed to lit any combination of dots,
                    i. e. if set to DOT_POS0 | DOT_POS2 the second dot from left 
                    and the rightmost dot will be lit
      @return none
    */
    void DisplayHexValue0(uint16_t value, uint8_t dotMode = DOT_OFF);

#if !defined(FAST_ISR)
    /**
      @brief  Called by ISR. Do not use
      @param  none
      @return none
    */
    void ISRFunc(void);

  private:
    /**
      @brief  Writes the value for one segment to the display. The value is saved in a lokal variable
      @param  Display position (0..3, 0=left)
      @return none
    */
     void WriteNumberToSegment(uint8_t);

    /**
      @brief  Write a given 16-bit value on the display as hexadecimal number
      @param  value 16 bit value to be displayed as hex
      @param  dot set DP(s)
                  can be set to:
                    DOT_NONE (default) to show no dot at all
                    DOT_ALL to lit all dots
                    DOT_POS0 to lit the rightmost dot up to
                    DOT_POS3 to lit the leftmost dot 
                    DOT_POSx values can be ORed to lit any combination of dots,
                    i. e. if set to DOT_POS0 | DOT_POS2 the second dot from left 
                    and the rightmost dot will be lit
      @param  padc  character to be used for left padding (should be either '0' 
                    or ' ' normally)
      @return none
    */
#endif
  private:
     void DisplayHexValue(uint16_t value, uint8_t dots, char padc);


};

#endif
