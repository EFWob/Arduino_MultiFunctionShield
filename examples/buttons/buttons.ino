/*
 * Show button state on display (right segments)
 * In addition, set the dot to position 0 to 3 to indicate how many buttons are currently pressed
 */

#include <MultiFunctionShield.h>
MultiFunctionShield MFS;

void setup ()
{
  MFS.begin();
  pinMode (BUTTON_1_PIN, INPUT);    // insert J2 on board for pull-ups!
  pinMode (BUTTON_2_PIN, INPUT);    // insert J2 on board for pull-ups!
  pinMode (BUTTON_3_PIN, INPUT);    // insert J2 on board for pull-ups!
}

uint8_t oldCount = 10;

void loop()
{
  uint8_t count = 0;
  if (digitalRead(BUTTON_1_PIN)) 
    MFS.DisplayBitmap(2, 0x54 | MFS.GetDot(2));
  else
  {
    count++;
    MFS.DisplayBitmap(2, 0x8 | MFS.GetDot(2));
  }
  if (digitalRead(BUTTON_2_PIN))
    MFS.DisplayBitmap(1, 0x54 | MFS.GetDot(1));
  else
  {
    count++;
    MFS.DisplayBitmap(1, 0x8 | MFS.GetDot(1));
  }
  if (digitalRead(BUTTON_3_PIN))
    MFS.DisplayChar(0, 'n', DOT_KEEP);
  else
  {
    count++;
    MFS.DisplayChar(0, '_', DOT_KEEP);
  }
  if (count != oldCount) 
  {
    MFS.DisplayDot(oldCount, false);
    MFS.DisplayDot(count);
    oldCount = count;
  }
  
}
