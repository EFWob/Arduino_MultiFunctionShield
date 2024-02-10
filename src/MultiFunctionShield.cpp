/*
 * Florian Schäffer
 * https://www.blafusel.de
 * 
 * https://github.com/coderfls/Arduino_MultiFunctionShield
 * 
 * https://www.heise.de/select/make/2018/2/1524540425550478
 * https://heise.de/-4031506
 */

#include <MultiFunctionShield.h>

const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};    // Segmente, die leuchten sollen pro Zahlwert (Low-Aktiv), & 0x7F Verknüpfen fuer Dezimalpunkt
const uint8_t SEGMENT_BLANK = 0xFF;
const uint8_t SEGMENT_MINUS = 0xBF;
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};                               // Ziffernposition (gemeinsame Anode, LSB)
volatile uint8_t ActDigit = 0;
volatile uint8_t SEGMENT_VALUE[4];
const uint8_t SEGMENT_MAP_LETTERS[] = {
    95, //a
    124,  //b
    88, //c
    94,  //d
    121,     //e
    113, //f
    61,   //g
    116,     //h
    17,         //i
    13,         //j  
    117,        //k
    56,    //L
    85,    //M
    84,        //n
    92,        //o
    115,     //p
    103,     //q
    80,         //r
    45,      //s
    120,        //t
    28,      //u
    42,       //v
    106,    //w
    20,         //x
    110,   //y
    27          //z
};

#if !defined(FAST_ISR)
static MultiFunctionShield *instance;
#else
static uint8_t latchBit;
static uint8_t* latchOut;
static uint8_t clockBit;
static uint8_t* clockOut;
static uint8_t dataBit;
static uint8_t* dataOut;
#endif

MultiFunctionShield::MultiFunctionShield(void)  //constructor
{
#if !defined(FAST_ISR)  
  instance = this;
#endif
}

void MultiFunctionShield::begin(void)
{
  digitalWrite(BUZZER_PIN, HIGH);   // first! else short sound
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LATCH_PIN,OUTPUT);
  pinMode(CLK_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
#if defined(FAST_ISR)
  latchBit = digitalPinToBitMask(LATCH_PIN);
  latchOut = portOutputRegister(digitalPinToPort(LATCH_PIN));
  clockBit = digitalPinToBitMask(CLK_PIN);
  clockOut = portOutputRegister(digitalPinToPort(CLK_PIN));
  dataBit = digitalPinToBitMask(DATA_PIN);
  dataOut = portOutputRegister(digitalPinToPort(DATA_PIN));
#endif    
  TCCR1A = 0;                                           // Register loeschen
  OCR1A = 1000;                                         // Vergleichswert x = (CPU / (2 x Teiler x f)) - 1
  TCCR1B |= (1 << CS10) | (1 << CS11) | (1 << WGM12);   // CTC-Mode, Teiler = 64
  TIMSK1 |= (1 << OCIE1A);                              // Output Compare A Match Interrupt Enable
  sei();                                                // IRQ Enable

  Clear();
}

void MultiFunctionShield::Display (int16_t value, uint8_t dotMode)
{
  if ((value > 9999) || (value < -999))   // out of range
  {
    DisplayText("----", dotMode);
  }
  else    // possible range
  {
    uint8_t digits = 4;
    if (value < 0)
    {
      value = -value;
      digits--;
      DisplayChar(3, '-', dotMode);
    }
    for(int i = 0;i < digits;i++)
    {
      char c = value % 10;
      if ((0 != value) || (0 == i))
        c = c + '0';
      DisplayChar(i, c, dotMode);
      value = value / 10;
    }
  }
  if (dotMode != 0)
    if (dotMode < 16) 
      DisplayDots(dotMode);
}

void MultiFunctionShield::DisplayDots(uint8_t dots)
{
  uint8_t dotMask = 1;
  for(int i=0;i < 4;i++)
    {
      if ((dots & dotMask) != 0) 
        SEGMENT_VALUE[3 - i] &= 0x7f;
      else
        SEGMENT_VALUE[3 - i] |= 0x80;
      dotMask = dotMask << 1;
    }
}

void MultiFunctionShield::DisplayDot(uint8_t dot, bool show)
{
  if (dot < 4)
    if (show)
      SEGMENT_VALUE[3 - dot] &= 0x7f;
    else
      SEGMENT_VALUE[3 - dot] |= 0x80;
}

uint8_t MultiFunctionShield::GetDot(uint8_t dot)
{
uint8_t ret = 0;
  if (dot < 4)
    if (0 == (SEGMENT_VALUE[3 - dot] & 0x80))
      ret = 128;
  return ret;
}

void MultiFunctionShield::DisplayBitmap(uint8_t pos, uint8_t bmap) 
{
  if (pos < 4) 
  {
    SEGMENT_VALUE[3 - pos] = ~bmap;
  }
}

void MultiFunctionShield::DisplayChar(uint8_t pos, char c, uint8_t dotMode)
{
  uint8_t bmap = 0;
  if (pos < 4)
  {
    if (isDigit(c))
      bmap = ~SEGMENT_MAP[c - '0']; 
    else if (isalpha(c)) 
    {
      c = tolower(c);
      if ((c >= 'a') && (c <= 'z'))
        bmap = SEGMENT_MAP_LETTERS[c - 'a'];
    }
    else if ('-' == c)
      bmap = 64;
    else if ('_' == c)
      bmap = 8;
    DisplayBitmap(pos, bmap | (dotMode==DOT_KEEP?GetDot(pos):(dotMode == DOT_ON?0x80:0)));
  }
}

void MultiFunctionShield::DisplayText(const char *s, uint8_t dotMode)
{
  for(int i = 0;i < 4;i++)
  {
    char c;
    if (4 - i < strlen(s))
      c = 0;
    else
      c = *(s++);
    DisplayChar(3 - i, c, dotMode);
  }
  if (dotMode)
    if (dotMode < 16)
      DisplayDots(dotMode);
}

void MultiFunctionShield::DisplayHexValue(uint16_t value, uint8_t dotMode, char padc)
{
  for(int i = 0;i < 4;i++)
  {
    char c;
    uint8_t x = value & 0xf;
    if ((0 == value) && (0 < i))
      c = padc;
    else
      c = x < 10?('0' + x):('a' + x - 10);
    value >>= 4;
    DisplayChar(i, c);
  }
  if (dotMode)
    if (dotMode < 16)
      DisplayDots(dotMode);
}

void MultiFunctionShield::DisplayHexValue(uint16_t value, uint8_t dotMode)
{
  DisplayHexValue(value, dotMode, ' ');
}

void MultiFunctionShield::DisplayHexValue0(uint16_t value, uint8_t dotMode)
{
  DisplayHexValue(value, dotMode, '0');
}

void MultiFunctionShield::Clear(void) 
{
  SEGMENT_VALUE[0] = SEGMENT_BLANK;
  SEGMENT_VALUE[1] = SEGMENT_BLANK;
  SEGMENT_VALUE[2] = SEGMENT_BLANK;
  SEGMENT_VALUE[3] = SEGMENT_BLANK;
}



#if defined(FAST_ISR)
ISR(TIMER1_COMPA_vect)          // interrupt service routine 
{
  uint16_t val;
  *latchOut &= ~latchBit;
  val  = ((uint16_t)(SEGMENT_VALUE[ActDigit]))<<8 | SEGMENT_SELECT[ActDigit];
  for (uint8_t i = 0; i < 16; i++)  
  {
    *clockOut &= ~clockBit;
    if (val & 0x8000)
      *dataOut |= dataBit;
    else
      *dataOut &= ~dataBit;
    *clockOut |= clockBit;
    val <<= 1;
  }

  ActDigit = (ActDigit + 1) & 0x3;
  *latchOut |= latchBit;
} 
#else
void MultiFunctionShield::WriteNumberToSegment(uint8_t digit)
{
  digitalWrite(LATCH_PIN,LOW);                                      // Uebernahme-Takt: Ausgang Aus
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, SEGMENT_VALUE[digit]);      // Segmente passend zum Zahlwert rausschieben
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, SEGMENT_SELECT[digit]);     // Welche Stelle leuchten soll hinterher schieben
  digitalWrite(LATCH_PIN,HIGH);                                     // Uebernahme-Takt: Ausgang Ein
}

void MultiFunctionShield::ISRFunc(void) 
{
  switch (++ActDigit)
  {
    case 1 : WriteNumberToSegment(0); break;     
    case 2 : WriteNumberToSegment(1); break; 
    case 3 : WriteNumberToSegment(2); break; 
    case 4 : WriteNumberToSegment(3); ActDigit = 0; break; 
  }
}


ISR(TIMER1_COMPA_vect)          // interrupt service routine 
{
  instance->ISRFunc();
} 
#endif