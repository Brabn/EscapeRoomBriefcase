#include <Wire.h> 
#include <LedControl.h>
#include <LiquidCrystal_I2C.h>
#include <PS2Keyboard.h>
const byte PIN_PS2_DATA=2;  // PS2 keyboard connection - data
const byte PIN_PS2_CLK=3;   // PS2 keyboard connection - clock signal
const byte PIN_SPI_DIN=4;   // Connecting a digital indicator - DIN
const byte PIN_SPI_CS=5;    // Connecting a digital indicator - CS
const byte PIN_SPI_CLK=6;   // Connecting a digital indicator - CLK
const byte PIN_CARD=7;      //PIN for card checking
const byte PIN_UNLOCK1=8;    // Lock #1 control PIN
const byte PIN_UNLOCK2=9;    // Lock #2 control PIN
const byte PIN_KEY1=A0;    // Key #1 control PIN
const byte PIN_KEY2=A1;    // Key #2 control PIN
const byte PIN_BUZZ = 12;   // Buzzer PIN
const byte PIN_KEYLED = 13;   //Connecting the simultaneous key rotation indicator


// Timings for buzzer signals
const int toneWrongPassTone=100;
const int toneWrongPassTime=200;
const int toneCorrectPassTone=2000;
const int toneCorrectPassTime=200;
const int toneWrongCodeTone=100;
const int toneWrongCodeTime=200;
const int toneCorrectCodeTone=2000;
const int toneCorrectCodeTime=200;
const int toneUnlockingTone=3000;
const int toneUnlockingTime=200;

// Keys states
bool Key1State=false;
bool Key1Prev=false; 
bool Key2State=false;
bool Key2Prev=false;
bool WrongKeysDisplayed=false;

bool Key1InitState=false;
bool Key2InitState=false;

long toneTimer;   // Buzzer timer
byte digitDisplayBright=8;	// Brightness of digit display

long Key1Timer; 
long Key2Timer;

int KeysPause=200;       // max pause between turning the keys for activation
long KeysPauseTimer;            // simultaneous key turn timer

int WrongKeysLength=1000;     //duration of the simultaneous turn message, ms
long WrongKeysTimer;          // simultaneous turn message timer

int BothKeysLength=1000;     //duration of the simultaneous turn message, ms
long BothKeysTimer;          // simultaneous turn message timer

long CursorBlinkTimer;         // cursor blink timer
int CursorBlinkLength=500;     // duration of cursor blinking, ms
long DigitBlinkTimer;         // digital indicator flashing timer
int DigitBlinkLength=250;     // blinking duration of a random set of numbers on a digital indicator, ms

long DigitCheckTimer;         // timer for digital indicator flashing when checking password
int DigitCheckLength=400;     // Duration of digital indicator blinking when checking a password, ms
bool DigitCheck=false;

long DigitSwitchTimer;         // digital indicator flashing timer
int DigitSwitchLength=80;     // duration of digital indicator blinking, ms
long CardInsertTimer;         // card insertion message timer
int CardInsertLength=1000;     // Duration of message about inserted card, ms
long WrongPassTimer;         // Wrong password message timer

int ShowCodeLength=6000;     // duration of message with correct code, ms
long ShowCodeTimer; 
int WrongPassLength=1200;     // Duration of message about incorrect password, ms
long CorrectPassTimer;         // timer for correct password notification
int CorrectPassLength=1200;     // duration of the message about the correct password, ms
long WrongCodeTimer;         //Wrong code message timer
int WrongCodeLength=1600;     // Duration of message about incorrect code, ms
long CorrectCodeTimer;         // correct code message timer
int CorrectCodeLength=1600;     // duration of the message about the correct code, ms

byte PassCursorPos=0;   //Current cursor in the entered password
byte CodeCursorPos=0;   //Current cursor in the input code
String lcd1text=String(16);     // Text to display in 1 line of LCD screen
String lcd2text=String(16);     // Text to display in 2 line of LCD screen
String lcd1now=String(16);      // Current text in 1 line of LCD screen
String lcd2now=String(16);      // Current text in 2 line of LCD screen
String UserPassword=String(16); // User entered password
String UserCode=String(16); // User entered digit code
String MainPassword=String(16); // Correct password
String MainCode=String(16); // Correct digit code

// Russian characters
uint8_t tochki[8] = {B0, B00000, B0, B0, B0, B0, B10101};
uint8_t bukva_P[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B10001};
uint8_t bukva_Ya[8] = {B01111, B10001, B10001, B01111, B00101, B01001, B10001};
uint8_t bukva_L[8] = {0x3, 0x7, 0x5, 0x5, 0xD, 0x9, 0x19};
uint8_t bukva_Lm[8] = {0, 0, B01111, B00101, B00101, B10101, B01001};
uint8_t bukva_Mz[8] = {0x10, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x1E};
uint8_t bukva_I[8] = {0x11, 0x13, 0x13, 0x15, 0x19, 0x19, 0x11};
uint8_t bukva_D[8] = {B01111, B00101, B00101, B01001, B10001, B11111, 0x11};
uint8_t bukva_G[8] = {B11111, B10001, B10000, B10000, B10000, B10000, B10000};
uint8_t bukva_IY[8] = {B01110, B00000, B10001, B10011, B10101, B11001, B10001};
uint8_t bukva_Z[8] = {B01110, B10001, B00001, B00010, B00001, B10001, B01110};
uint8_t bukva_ZH[8] = {B10101, B10101, B10101, B11111, B10101, B10101, B10101};
uint8_t bukva_U[8] = {B10001, B10001, B10001, B01010, B00100, B01000, B10000};
uint8_t bukva_B[8] = {B11110, B10000, B10000, B11110, B10001, B10001, B11110};
uint8_t bukva_CH[8] = {B10001, B10001, B10001, B01111, B00001, B00001, B00001};
uint8_t bukva_IYI[8] = {B10111, B10101, B10101, B11101, B10101, B10101, B10111};
uint8_t bukva_TS[8] = {B10010, B10010, B10010, B10010, B10010, B10010, B11111, B00001};
uint8_t bukva_MZN[8] = {B10000, B10000, B10000, B11110, B10001, B10001, B11110};
uint8_t bukva_T[8] = {B11111, B10101, B00100, B00100, B00100, B00100, B00100};
uint8_t bukva_6I[8] = {B10001,B10001, B10001, B11101, B10101, B10101, B11101};

char cursorSymbol='_';
String msgNoCard="Insert card... ";
String msgCardInserted="OK, inserted ";
String msgNeedPass="нг";


int CurrSwitchDigit=0; // current digit for quick change
bool CardInserted=false;
bool Unlocking=false;

// Modes
bool ModeNoCard=true;           // Card insertion requirement mode
bool ModeCardInserted=false;    //Mode immediately after card insertion

bool ModeNeedKeys=false;        // Key rotation request mode
bool ModeBothKeys=false;        // Keys turned simultaneously
bool ModeWrongKeys=false;       // Keys turned nonsimultaneously


bool ModeNeedPass=false;        // Password Request Mode
bool ModeWrongPass=false;       // Mode after entering the wrong password
bool ModeCorrectPass=false;     // Mode after entering the correct password
bool ModeNeedCode=false;        // Code request mode
bool ModeShowCode=false;        // Code display mode
bool ModeWrongCode=false;       // Mode after entering an incorrect code
bool ModeCorrectCode=false;     // Mode after entering an correct code
bool ModeUnlocked=false;        // Open lock mode
bool ModeCodeBlinking=false;    // Flashing mode of random sets of numbers on the digital indicator
bool ModeDigitSwitching=false;  // Mode for quickly changing random numbers on a digital indicator
bool ModeDigitChecking=false;   // Flashing mode of the entered code on the digital indicator
bool ModeCursorBlinking=false;  // Cursor blinking mode
bool CursorBlink=false;

 
LedControl digitDisplay = LedControl(PIN_SPI_DIN, PIN_SPI_CLK, PIN_SPI_CS,1);    // Connecting a 7 segment indicator via SPI
LiquidCrystal_I2C lcdDisplay(0x3F,16,2); 
PS2Keyboard keyboard;
int note=0;

int count=1;          // current bit counter of 11-bit code from keyboard
int m=0;              // scan code current bit counter
uint8_t scan_code ;  // scan code of the key pressed
bool btUp=false;
void CheckPassword()
{
  ModeNeedPass=false;
  if (UserPassword==MainPassword)
          {
          ModeCorrectPass=true;
          ModeCodeBlinking=false;     // Flashing mode of random sets of numbers on the digital indicator
          ModeDigitSwitching=false;    // Mode for quickly changing random numbers on a digital indicator
          ModeDigitChecking=false;    // Flashing mode of the entered code on the digital indicator
          DigitCheck=false;
          
          CorrectPassTimer=millis();
          tone(PIN_BUZZ,toneCorrectPassTone,toneCorrectPassTime);
          }
          else
          {
          ModeWrongPass=true;
          WrongPassTimer=millis();
          tone(PIN_BUZZ,toneWrongPassTone,toneWrongPassTime);
          
          }
}
void CheckCode()
{
  ModeNeedCode=false;
  ModeCodeBlinking=false;     // Flashing mode of random sets of numbers on the digital indicator
  ModeDigitSwitching=false;    // Mode for quickly changing random numbers on a digital indicator
  ModeDigitChecking=true;    // Flashing mode of the entered code on the digital indicator
  DigitCheckTimer=millis();
  if (UserCode==MainCode)
          {
          ModeCorrectCode=true;
          CorrectCodeTimer=millis();
          tone(PIN_BUZZ,toneCorrectCodeTone,toneCorrectCodeTime);
          }
          else
          {
          ModeWrongCode=true;
          WrongCodeTimer=millis();
          tone(PIN_BUZZ,toneWrongCodeTone,toneWrongCodeTime);
          }
}

void LcdLineInsertCard()  // Display the line "Insert card"
{
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("  INSERT CARD   ");
  lcdDisplay.createChar(0, bukva_T);
  lcdDisplay.createChar(1, bukva_MZN);
  lcdDisplay.createChar(2, bukva_U);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print('B');
  lcdDisplay.print('C');
  lcdDisplay.write(0);//T  
  lcdDisplay.print('A');
  lcdDisplay.print('B');
  lcdDisplay.write(1);//Ь
  lcdDisplay.write(0);//T  
  lcdDisplay.print('E');
  lcdDisplay.print(' ');
  lcdDisplay.print('K');
  lcdDisplay.print('A');
  lcdDisplay.print('P');
  lcdDisplay.write(0);//T
  lcdDisplay.write(2);//У
}  
void LcdLineCardInserted()  // Display the line "Card inserted"
{
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print(" CARD INSERTED! ");
  lcdDisplay.createChar(0, bukva_T);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print('K');
  lcdDisplay.print('A');
  lcdDisplay.print('P');
  lcdDisplay.write(0);//T
  lcdDisplay.print('A');
  lcdDisplay.print(' ');
  lcdDisplay.print('B');
  lcdDisplay.print('C');
  lcdDisplay.write(0);//T  
  lcdDisplay.print('A');
  lcdDisplay.print('B');
  lcdDisplay.write(1);//Л
  lcdDisplay.print('E');
  lcdDisplay.print('H');  
  lcdDisplay.print('A');
  lcdDisplay.print('!');
}
void LcdLineReadCode()   // Display the line "Remember code"
{
    lcdDisplay.setCursor(0,0);
  lcdDisplay.print(" REMEMBER CODE:");
lcdDisplay.createChar(0, bukva_D);
    lcdDisplay.createChar(1, bukva_P);
    lcdDisplay.createChar(2, bukva_I);
    lcdDisplay.createChar(3, bukva_T);
    lcdDisplay.setCursor(0,1);
    lcdDisplay.print(' ');
    lcdDisplay.print('3');
    lcdDisplay.print('A');
    lcdDisplay.write(1);//П
    lcdDisplay.print('O');
    lcdDisplay.print('M');
    lcdDisplay.print('H');
    lcdDisplay.write(2);//И
    lcdDisplay.write(3);//Т
    lcdDisplay.print('E');
    lcdDisplay.print(' ');
    lcdDisplay.print('K');
    lcdDisplay.print('O');
    lcdDisplay.write(0);//Д
    lcdDisplay.print(':');
    lcdDisplay.print(' ');
}
void LcdLineSwitchKeys()  //  Display the line "Turn the keys"
{
      lcdDisplay.setCursor(0,0);
  lcdDisplay.print(" TURN THE KEYS   ");
    lcdDisplay.createChar(0, bukva_P);
    lcdDisplay.createChar(1, bukva_I);
    lcdDisplay.createChar(2, bukva_T);
    lcdDisplay.createChar(3, bukva_L);
    lcdDisplay.createChar(4, bukva_IYI);
    lcdDisplay.createChar(5, bukva_CH);
    lcdDisplay.setCursor(0,1);
    lcdDisplay.write(0);//П
    lcdDisplay.print('O');
    lcdDisplay.print('B');
    lcdDisplay.print('E');
    lcdDisplay.print('P');
    lcdDisplay.print('H');
    lcdDisplay.write(1);//И
    lcdDisplay.write(2);//T
    lcdDisplay.print('E');
    lcdDisplay.print(' ');
    lcdDisplay.print('K');
    lcdDisplay.write(3);//Л   
    lcdDisplay.write(4);//Ю  
    lcdDisplay.write(5);//Ч
    lcdDisplay.write(1);//И 
}
void LcdLineInsertCode() //  Display the line "Insert code"
{
      lcdDisplay.setCursor(0,0);
  lcdDisplay.print(" INSERT CODE   ");
  lcdDisplay.createChar(0, bukva_D);
  lcdDisplay.createChar(1, bukva_I);
  lcdDisplay.createChar(2, bukva_T);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print(' ');
  lcdDisplay.print('B');  
  lcdDisplay.print('B');  
  lcdDisplay.print('E');
  lcdDisplay.write(0);//Д
  lcdDisplay.write(1);//И
  lcdDisplay.write(2);//Т
  lcdDisplay.print('E');
  lcdDisplay.print(' ');
  lcdDisplay.print('K');
  lcdDisplay.print('O');
  lcdDisplay.write(0);//Д
}
void LcdLineInsertPass()	//  Display the line "Password:"
{
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("PASSWORD:   ");
  lcdDisplay.createChar(0, bukva_P);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_MZN);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.write(0);
  lcdDisplay.print("A");
  lcdDisplay.print("P"); 
  lcdDisplay.print("O");  
  lcdDisplay.write(1);
  lcdDisplay.write(2);
  lcdDisplay.print(":"); 
}
void LcdLineWrongCode()	//  Display the line "Wrong!"
{
      lcdDisplay.setCursor(0,0);
  lcdDisplay.print("    WRONG !    ");
 lcdDisplay.createChar(0, bukva_P);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_MZN);
  lcdDisplay.createChar(3, bukva_I);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print(' ');
  lcdDisplay.print('H');  
  lcdDisplay.print('E'); 
  lcdDisplay.write(0);//П
  lcdDisplay.print('P');
  lcdDisplay.print('A');
  lcdDisplay.print('B');
  lcdDisplay.write(3);//И 
  lcdDisplay.write(1);//Л
  lcdDisplay.write(2);//Ь 
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.print('!');
  lcdDisplay.print("    ");
}
void LcdLineWrongPass()	//  Display the line "Wrong!"
{
        lcdDisplay.setCursor(0,0);
  lcdDisplay.print("    WRONG !    ");
 lcdDisplay.createChar(0, bukva_P);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_MZN);
  lcdDisplay.createChar(3, bukva_I);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print(' ');
  lcdDisplay.print('H');  
  lcdDisplay.print('E'); 
  lcdDisplay.write(0);//П
  lcdDisplay.print('P');
  lcdDisplay.print('A');
  lcdDisplay.print('B');
  lcdDisplay.write(3);//И 
  lcdDisplay.write(1);//Л
  lcdDisplay.write(2);//Ь 
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.print('!');
  lcdDisplay.print("    ");
}  
void LcdLineCorrectCode()		//  Display the line "Correct!"
 {
        lcdDisplay.setCursor(0,0);
  lcdDisplay.print("    CORRECT !   ");
  lcdDisplay.createChar(0, bukva_P);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_MZN);
  lcdDisplay.createChar(3, bukva_I);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print(' ');
  lcdDisplay.print(' ');  
  lcdDisplay.print(' '); 
  lcdDisplay.write(0);//П
  lcdDisplay.print('P');
  lcdDisplay.print('A');
  lcdDisplay.print('B');
  lcdDisplay.write(3);//И 
  lcdDisplay.write(1);//Л
  lcdDisplay.write(2);//Ь 
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.print('!');
  lcdDisplay.print("    ");
  }
    
void LcdLineCorrectPass()		//  Display the line "Correct!"
 {
          lcdDisplay.setCursor(0,0);
  lcdDisplay.print("    CORRECT !   ");
  lcdDisplay.createChar(0, bukva_P);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_MZN);
  lcdDisplay.createChar(3, bukva_I);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print(' ');
  lcdDisplay.print(' ');  
  lcdDisplay.print(' '); 
  lcdDisplay.write(0);//П
  lcdDisplay.print('P');
  lcdDisplay.print('A');
  lcdDisplay.print('B');
  lcdDisplay.write(3);//И 
  lcdDisplay.write(1);//Л
  lcdDisplay.write(2);//Ь 
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.print('!');
  lcdDisplay.print("    ");
  }
  
void LcdLineUnlocked()		//  Display the line "UNLOCKED!"
 {
          lcdDisplay.setCursor(0,0);
  lcdDisplay.print("   UNLOCKED !");
  lcdDisplay.createChar(0, bukva_B);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_I);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print('P');
  lcdDisplay.print('A');
  lcdDisplay.print('3');
  lcdDisplay.write(0);//Б 
  lcdDisplay.write(1);//Л
  lcdDisplay.print('O');
  lcdDisplay.print('K');
  lcdDisplay.write(2);//И  
  lcdDisplay.print('P');
  lcdDisplay.print('O');
  lcdDisplay.print('B');
  lcdDisplay.print('A');
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.print('!');
 }

void LcdLineWrongKeys()  //  Display the line "Need two keys!"
 {
          lcdDisplay.setCursor(0,0);
  lcdDisplay.print(" NEED TWO KEYS!");
  lcdDisplay.createChar(0, bukva_U);
  lcdDisplay.createChar(1, bukva_ZH);
  lcdDisplay.createChar(2, bukva_D);
  lcdDisplay.createChar(3, bukva_L);
  lcdDisplay.createChar(4, bukva_IYI);
  lcdDisplay.createChar(5, bukva_CH);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print('H');
  lcdDisplay.write(0);//У
  lcdDisplay.write(1);//Ж 
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.write(2);//Д
  lcdDisplay.print('B');
  lcdDisplay.print('A');
  lcdDisplay.print(' ');
  lcdDisplay.print('K');
  lcdDisplay.write(3);//Л   
  lcdDisplay.write(4);//Ю  
  lcdDisplay.write(5);//Ч
  lcdDisplay.print('A');
  lcdDisplay.print('!');
 }
void LcdLineBothKeys() //  Display the line "UNLOCKED!"
 {
            lcdDisplay.setCursor(0,0);
  lcdDisplay.print("    UNLOCKED !");
  lcdDisplay.createChar(0, bukva_B);
  lcdDisplay.createChar(1, bukva_L);
  lcdDisplay.createChar(2, bukva_I);
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print('P');
  lcdDisplay.print('A');
  lcdDisplay.print('3');
  lcdDisplay.write(0);//Б 
  lcdDisplay.write(1);//Л
  lcdDisplay.print('O');
  lcdDisplay.print('K');
  lcdDisplay.write(2);//И  
  lcdDisplay.print('P');
  lcdDisplay.print('O');
  lcdDisplay.print('B');
  lcdDisplay.print('A');
  lcdDisplay.print('H');
  lcdDisplay.print('O');
  lcdDisplay.print(' ');
  lcdDisplay.print('!');
 }
 void(* resetFunc) (void) = 0; // Reset MC function

bool first;
void setup() {
  MainPassword="X2AHK37T";
  MainCode="23705543";
  UserPassword="";
  UserCode="";
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("PassCase v1.5"));
  pinMode(PIN_UNLOCK1, OUTPUT);     //Lock 1 control PIN as output
  pinMode(PIN_UNLOCK2, OUTPUT);     // Lock 2 control PIN as output
  pinMode(PIN_KEYLED, OUTPUT);
   
  pinMode(PIN_PS2_DATA, INPUT);     //configure pins from the keyboard as inputs
  pinMode(PIN_PS2_CLK, INPUT);

  keyboard.begin(PIN_PS2_DATA, PIN_PS2_CLK);
  Serial.println(F("keyboard init"));
  pinMode(PIN_PS2_CLK, INPUT);

  digitalWrite(PIN_CARD,HIGH);            //turn on the internal pull-up resistor for the card reader pin
  digitalWrite(PIN_KEY1,HIGH);            //turn on the internal pull-up resistor for the key 1 pin
  digitalWrite(PIN_KEY2,HIGH);            //turn on the internal pull-up resistor for the key 1 pin

  digitalWrite(PIN_UNLOCK1,LOW);
  digitalWrite(PIN_UNLOCK2,LOW);

  delay(1000); 
  //attachInterrupt(1, ps2interrupt, FALLING);  // Keyboard clock interrupt

  lcdDisplay.init();                      // initialize the lcd 
  lcdDisplay.backlight();
  lcdDisplay.noCursor();
  lcdDisplay.begin(16, 2); 
  Serial.println(F("lcd init"));
  digitDisplay.shutdown(0, true);  // turns on display
  digitDisplay.setIntensity(0, 10); // 15 = brightest
  digitDisplay.setDigit(0, 0, 0, false);
  digitDisplay.setDigit(0, 1, 8, false);
  digitDisplay.setDigit(0, 2, 7, false);
  digitDisplay.setDigit(0, 3, 6, false);
  digitDisplay.setDigit(0, 4, 5, false);
  digitDisplay.setDigit(0, 5, 4, false);
  digitDisplay.setDigit(0, 6, 3, false);
  digitDisplay.setDigit(0, 7, 2, false);
Serial.println(F("digit init"));

   if (analogRead(PIN_KEY1)>500)  Key1InitState=false,Key1Prev=false; else Key1InitState=true,Key1Prev=true;
   if (analogRead(PIN_KEY2)>500)  Key2InitState=false,Key2Prev=false; else Key2InitState=true,Key2Prev=true;
  //delay(2000);
  
}
int intens=0;
int digit=0;
int i=0;
void setLcdText(String text1,String text2)
{
  lcd1text=text1;
  lcd2text=text2;
}

bool msgNoCardShow=false;


void ResetPassword()
{
          ModeNeedCode=false;
          ModeShowCode=false;
          ModeWrongCode=false;
          ModeCorrectCode=false;
          ModeWrongPass=false;
          ModeCorrectPass=false;
          ModeNeedPass=true;
          ModeCodeBlinking=true;     // Режим мигания случайных наборов чисел на цифровом индикаторе
          ModeDigitSwitching=false;  // Режим быстрой смены случайных чисел на цифровом индикаторе
          ModeDigitChecking=false;   // Режим мигания введенного кода на цифровом индикаторе
          UserPassword="";
          UserCode="";
          PassCursorPos=0;
          CodeCursorPos=0;
          Unlocking=false;
}

void loop() {
  
  
  if (ModeNoCard)         // Режим требования вставить карту
   {
    if (!msgNoCardShow)
    {
    lcdDisplay.clear();
    LcdLineInsertCard();  // Вывестистроку "Вставьте карту"
    msgNoCardShow=true;
    }
  }
 
if (ModeCardInserted)    // Режим сразу после вставки карты   
 {
 LcdLineCardInserted();  // Вывести строку "Карта вставлена"
  if ((millis()-CardInsertTimer)>CardInsertLength)
  {
    digitDisplay.shutdown(0, false);  // turns on display
    ModeCardInserted=false;
    ModeNeedKeys=true;
    lcdDisplay.clear();
  }
 }
 int KeyMinPause=100;
 unsigned long Key1offTimer;
 unsigned long Key2offTimer;
 int Key1abs=0;
 int Key2abs=0;
 if (ModeNeedKeys) 
 {
  digitDisplay.shutdown(0, false);
  LcdLineSwitchKeys();
  ModeCodeBlinking=true;
  /*ModeNeedKeys=false;
          ModeBothKeys=true;
          BothKeysTimer=millis();*/
     //----- Обработка поворота ключей
/*
if (keyboard.available()) 
  {
    char c = keyboard.read();
    if (c == 'z')
    {
      Key1State=true;
    }
    else if (c == 'x')
    {
      Key2State=true;
    }
  }   */
 
     
    if (analogRead(PIN_KEY1)>500)  Key1State=false; else Key1State=true;
     if (analogRead(PIN_KEY2)>500)  Key2State=false; else Key2State=true;
    Key1abs=analogRead(PIN_KEY1);
    Key2abs=analogRead(PIN_KEY2);
    /*Serial.print("Key1 ");
     Serial.print(Key1abs);
     Serial.print(" Key2 ");
     Serial.println(Key2abs);*/
     
     if ((Key1State==true)&&(Key1Prev==false)&&(!Key1InitState))    // Ключ 1 повернут
     {
        Serial.println("Key1 ON");
        delay(20);
        Key1Timer=millis();
        Key1offTimer=0;
        Key1Prev=true;
     } else
     if ((Key1State==false)&&(Key1Prev==true))    // Ключ 1 выключен
     {
       if (Key1InitState) Key1InitState=false;
        Serial.println("Key1 OFF");
        delay(20);
        WrongKeysDisplayed=false;
        Key1Timer=0;
        Key1offTimer=millis();
        Key1Prev=false;
     }
      if ((Key2State==true)&&(Key2Prev==false)&&(!Key2InitState))    // Ключ 2 повернут
     {
        Serial.println("Key2 ON");
        delay(20);
        Key2Timer=millis();
        Key2offTimer=0;
        Key2Prev=true;
     } else
     if ((Key2State==false)&&(Key2Prev==true))    // Ключ 2 выключен
     {
      if (Key2InitState) Key2InitState=false;
        Serial.println("Key2 OFF");
        delay(20);
        WrongKeysDisplayed=false;
        Key2Timer=0;
        Key2offTimer=millis();
        Key2Prev=false;
     }
     
     
     if ((Key1State)&&(Key2State))
     {
        Serial.print("KeyPause =");
        Serial.println(abs(Key1Timer-Key2Timer));
        if ((Key1Timer!=0)&&(Key2Timer!=0))
        if ((abs(Key1Timer-Key2Timer)>KeysPause))
        {
          if (!WrongKeysDisplayed)
          {  
            //Serial.println("WRONG");       
            ModeNeedKeys=false;
            ModeWrongKeys=true;
            WrongKeysTimer=millis();
            Key1Timer=0;
            Key2Timer=0;
            Key1offTimer=0;
            Key2offTimer=0;
            tone(PIN_BUZZ,toneWrongPassTone,toneWrongPassTime);
            
          }
        }
        else
        {
          Serial.println("RIGHT"); 
          tone(PIN_BUZZ,toneCorrectPassTone,toneCorrectPassTime);
          delay(500);
          Key1Timer=0;
            Key2Timer=0;
            Key1offTimer=0;
            Key2offTimer=0;
          ModeNeedKeys=false;
          ModeBothKeys=true;
          BothKeysTimer=millis();
          
        }
       
     }
         /* ModeNeedKeys=false;
          ModeBothKeys=true;
          BothKeysTimer=millis();
          tone(PIN_BUZZ,toneCorrectPassTone,toneCorrectPassTime);
     */
     //BothKeysTimer=timer();
     
 }
 if (ModeWrongKeys)   // Режим после неодновременного поворота ключей
 {
  tone(PIN_BUZZ,toneWrongPassTone,toneWrongPassTime);
  LcdLineWrongKeys();  // Вывести строку "Нужно два ключа"
  if ((millis()-WrongKeysTimer)>WrongKeysLength)
  {
    //noTone(PIN_BUZZ);
    ModeWrongKeys=false;
    ModeNeedKeys=true;
    lcdDisplay.clear();
    WrongKeysDisplayed=true;
  }
 }
 
 if (ModeBothKeys)   // Режим после одновременного поворота ключей
 {
  tone(PIN_BUZZ,toneCorrectPassTone,toneCorrectPassTime);
  digitalWrite(PIN_KEYLED,HIGH);
  LcdLineBothKeys();  // Вывести строку "Разблокировано"
  if ((millis()-BothKeysTimer)>BothKeysLength)
  {
    
    //noTone(PIN_BUZZ);
    digitalWrite(PIN_KEYLED,LOW);
    digitalWrite(PIN_UNLOCK1,HIGH);
    delay(2000);
    digitalWrite(PIN_UNLOCK1,LOW);
    ModeBothKeys=false;
    ModeWrongKeys=false;
    ModeNeedKeys=false;
    
    ModeNeedPass=true;
    lcdDisplay.clear();
    //digitDisplay.shutdown(0, false);  // turns on display
    lcdDisplay.clear();
    ModeCodeBlinking=true;
  }
 }

 
/*
 if (ModeBothKeys)   // Режим сразу после поворота ключей    
 {
 LcdLineBothKeys();  // Вывести строку "Карта вставлена"
  if ((millis()-CardInsertTimer)>CardInsertLength)
  {
    digitDisplay.shutdown(0, false);  // turns on display
    ModeCardInserted=false;
    ModeNeedPass=true;
    lcdDisplay.clear();
    ModeCodeBlinking=true;
  }
 }
 
 */
 if (ModeNeedPass)        // Режим запроса пароля
 {
if (keyboard.available()) 
  {
    char c = keyboard.read();
    if (c == PS2_ENTER)
    {
      CheckPassword();
    }
    else if (c == PS2_ESC)
    {
      ResetPassword();
    }
    else if (c == PS2_DELETE) 
    {
      Serial.print("[Backspase]");
         if (PassCursorPos!=0)
         {
            UserPassword.remove(PassCursorPos-1);
            PassCursorPos-=1;
         } 
    }
    else 
    {
      if (c=='a') c='A';
      if (c=='b') c='B';
      if (c=='c') c='C';
      if (c=='d') c='D';
      if (c=='e') c='E';
      if (c=='f') c='F';
      if (c=='g') c='G';
      if (c=='h') c='H';
      if (c=='j') c='J';
      if (c=='i') c='I';
      if (c=='k') c='K';
      if (c=='l') c='L';
      if (c=='m') c='M';
      if (c=='n') c='N';
      if (c=='o') c='O';
      if (c=='p') c='P';
      if (c=='q') c='Q';
      if (c=='r') c='R';
      if (c=='s') c='S';
      if (c=='t') c='T';
      if (c=='u') c='U';
      if (c=='v') c='V';
      if (c=='w') c='W';
      if (c=='x') c='X';
      if (c=='y') c='Y';
      if (c=='z') c='Z';
      Serial.print(c);
          UserPassword+=c;
          PassCursorPos+=1;
    }
 }
 
 //noTone(PIN_BUZZ);
 /*lcdDisplay.setCursor(0,0);
 lcdDisplay.print((int)scancode);
  lcdDisplay.print("   ");*/
  LcdLineInsertPass();  //Вывести строку "Пароль:"
  
  ModeCodeBlinking=true;
  ModeCursorBlinking=true;
  if (UserPassword!="") 
  {
    
    for (int i=0; i<PassCursorPos; i++)
    {
      //Serial.println(UserPassword[i]);
      lcdDisplay.print(UserPassword[i]);
    }
  }
  if (CursorBlink) 
  {
    lcdDisplay.print(cursorSymbol);
  }
  else
  {
    lcdDisplay.print(' ');
  }
  lcdDisplay.print("           ");
//  else lcd2text=CurrString;
 /* if (CursorBlink) Serial.println("blink");
  else Serial.println("zero");*/
  /*if (CursorBlink) 
  {
      lcd2text.setCharAt(PassCursorPos+9,cursorSymbol);
  }
  else lcd2text.setCharAt(PassCursorPos+9,' ');*/
  

 }
 if (ModeWrongPass)       // Режим после ввода неправильного пароля 
 {
  /* ModeNeedPass=false;
   if ((millis()-WrongPassTimer)>=toneWrongPassTime)
   {
   noTone(PIN_BUZZ);
   }*/
  //setLcdText(UserPassword+"/"+MainPassword,msgWrongPass);
  LcdLineWrongPass();
  ModeCodeBlinking=false;
  digitDisplay.clearDisplay(0);
  //tone(PIN_BUZZ,toneWrongPassTone,toneWrongPassTime);
  if ((millis()-WrongPassTimer)>WrongPassLength)
  {
    //noTone(PIN_BUZZ);
    lcdDisplay.clear();
    ModeWrongPass=false;
    ModeNeedPass=true;
    UserPassword="";
    PassCursorPos=0;
    WrongPassTimer=0;
  }
 }


 if (ModeCorrectPass)     // Режим после ввода правильного пароля
 {
   ModeNeedPass=false;
   //tone(PIN_BUZZ,toneCorrectPassTone,toneCorrectPassTime);
  //setLcdText(UserPassword+"="+MainPassword,msgCorrectPass);
  LcdLineCorrectPass();
  
  ModeCodeBlinking=false;
  ModeDigitSwitching=false;
  digitDisplay.clearDisplay(0);
  if ((millis()-CorrectPassTimer)>CorrectPassLength)
  {
    //noTone(PIN_BUZZ);
    lcdDisplay.clear();
    ModeCorrectPass=false;
    ModeWrongPass=false;
    ModeNeedPass=false;
    CorrectPassTimer=0;
    ModeDigitChecking=true;    // Режим мигания введенного кода на цифровом индикаторе
    ModeShowCode=true;
    ShowCodeTimer=millis();
    UserPassword="";
    PassCursorPos=0;
    UserCode="";
    CodeCursorPos=0;
  }
 }
 if (ModeShowCode)       // Режим после ввода неправильного кода
 {
    LcdLineReadCode();
    //lcdDisplay.print(MainCode);
      ModeCodeBlinking=false;     // Режим мигания случайных наборов чисел на цифровом индикаторе
      ModeDigitSwitching=false;    // Режим быстрой смены случайных чисел на цифровом индикаторе
      ModeDigitChecking=false;    // Режим мигания введенного кода на цифровом индикаторе
      DigitCheck=true;
      int MainCodeLen=MainCode.length();
      for ( i=0; i<=7;i++)
      {
         int shownInt=(int)(MainCode.charAt(i))-48;
         digitDisplay.setChar(0, 7-i, (DigitCheck)?MainCode.charAt(i):' ',false);
      }
      
    if ((millis()-ShowCodeTimer)>=ShowCodeLength)
    {
      ModeShowCode=false;
      ModeNeedCode=true;
      ModeCodeBlinking=false;     // Режим мигания случайных наборов чисел на цифровом индикаторе
      ModeDigitSwitching=true;    // Режим быстрой смены случайных чисел на цифровом индикаторе
      ModeDigitChecking=false;    // Режим мигания введенного кода на цифровом индикаторе
      //DigitChecking=false;
      CodeCursorPos=0;
      UserCode="";
      lcdDisplay.clear();
    }
 }
 if (ModeNeedCode)        // Режим запроса кода
 {
  if (keyboard.available()) 
  {
    char c = keyboard.read();
    if (c == PS2_ENTER)
    {
      CheckCode();
    }
    else if (c == PS2_ESC)
    {
      ResetPassword();
    }
    else if (c == PS2_DELETE) 
    {
      Serial.print("[Backspase]");
         if (CodeCursorPos!=0)
         {
            UserCode.remove(CodeCursorPos-1);
            CodeCursorPos-=1;
         } 
    }
    else 
    {
      if ((c=='0')||(c=='1')||(c=='2')||(c=='3')||(c=='4')||(c=='5')||(c=='6')||(c=='7')||(c=='8')||(c=='9'))
      {
      Serial.print(c);
          UserCode+=c;
          CodeCursorPos+=1;
      }
    }
 } 
    //setLcdText("",msgNeedCode);
LcdLineInsertCode();
  //ModeCodeBlinking=false;
  // ModeDigitSwitching=true;
 }

 if (ModeWrongCode)       // Режим после ввода неправильного кода
 {
   ModeNeedCode=false;
  //setLcdText(UserPassword+"/"+MainPassword,msgWrongPass);

LcdLineWrongCode();
  //ModeCodeBlinking=false;
  //digitDisplay.setDigit(0, 0, 5, false);
  digitDisplay.clearDisplay(0);
  if (ModeDigitChecking)
  {
      int UserCodeLen=UserCode.length();
      for ( i=0; i<=7;i++)
      {
        if (i<UserCodeLen)  digitDisplay.setChar(0, 7-i, (!DigitCheck)?UserCode.charAt(i):' ', false);
        else                digitDisplay.setChar(0, 7-i, ' ', false);
        
      }
  }
  if ((millis()-WrongCodeTimer)>WrongCodeLength)
  {
    //noTone(PIN_BUZZ);
    lcdDisplay.clear();
    ModeWrongCode=false;
    ModeNeedCode=true;
    UserCode="";
    CodeCursorPos=0;
    ModeCodeBlinking=false;     // Режим мигания случайных наборов чисел на цифровом индикаторе
    ModeDigitSwitching=true;    // Режим быстрой смены случайных чисел на цифровом индикаторе
    ModeDigitChecking=false;    // Режим мигания введенного кода на цифровом индикаторе
  }
 }
 if (ModeCorrectCode)     // Режим после ввода правильного кода
 {
LcdLineCorrectCode();
if (ModeDigitChecking)
{
      int UserCodeLen=UserCode.length();
      for ( i=0; i<=7;i++)
      {
        if (i<UserCodeLen)  digitDisplay.setChar(0, 7-i, (!DigitCheck)?UserCode.charAt(i):' ', false);
        else                digitDisplay.setChar(0, 7-i, ' ', false);
        
      }
}
  if ((millis()-CorrectCodeTimer)>CorrectCodeLength)
  {
    //noTone(PIN_BUZZ);
    lcdDisplay.clear();
    ModeCorrectCode=false;
    ModeUnlocked=true;
    UserCode="";
    CodeCursorPos=0;
    ModeCodeBlinking=false;     // Режим мигания случайных наборов чисел на цифровом индикаторе
    ModeDigitSwitching=false;    // Режим быстрой смены случайных чисел на цифровом индикаторе
    ModeDigitChecking=false;    // Режим мигания введенного кода на цифровом индикаторе
  }
  
 }
 if (ModeUnlocked)
 {
  LcdLineUnlocked();
   if (!Unlocking)
   {
    digitalWrite(PIN_UNLOCK2,HIGH);
    delay(2000);
    digitalWrite(PIN_UNLOCK2,LOW);
    Unlocking=true;
   }
 }
 if (ModeCodeBlinking)    // Режим мигания случайных чисел на цифровом индикаторе
 {
    if ((millis()-DigitBlinkTimer)>=DigitBlinkLength)
    {
      for ( i=0; i<=7;i++)
      {
         digitDisplay.setDigit(0, i, random(9), false);
      }
      DigitBlinkTimer=millis();
    }
 }
 if (ModeDigitSwitching)    // Режим мигания случайных чисел на цифровом индикаторе
 {
    
    if ((millis()-DigitSwitchTimer)>=DigitSwitchLength)
    {
      int UserCodeLen=UserCode.length();
      if (UserCodeLen==8) CheckCode();
      for ( i=0; i<=7;i++)
      {
        
        if (i<UserCodeLen)
        {
        digitDisplay.setChar(0, 7-i, UserCode.charAt(i), false);
        }
        else
        {
           digitDisplay.setDigit(0, 7-i, CurrSwitchDigit, false);
        }
         CurrSwitchDigit++;
         if (CurrSwitchDigit>=10) CurrSwitchDigit=0;
      }
      DigitSwitchTimer=millis();
    }
 }
if (ModeCursorBlinking)  // Режим мигания случайных чисел на цифровом индикаторе
  {
    if ((millis()-CursorBlinkTimer)>CursorBlinkLength) 
    {
      CursorBlink=!CursorBlink;
      CursorBlinkTimer=millis();
    }
   
  }
  if (ModeDigitChecking)  // Режим мигания случайных чисел на цифровом индикаторе
  {
    if ((millis()-DigitCheckTimer)>DigitCheckLength) 
    {
      DigitCheck=!DigitCheck;
      DigitCheckTimer=millis();
    }
   
  }
  if (lcd1text!=lcd1now)
  {
    
    lcdDisplay.setCursor(0,0);
    lcdDisplay.print("                ");
    lcdDisplay.setCursor(0,0);
    lcdDisplay.print(lcd1text);
    lcd1now=lcd1text;
  }
  if (lcd2text!=lcd2now)
  {
    
    lcdDisplay.setCursor(0,1);
    lcdDisplay.print("                ");
    lcdDisplay.setCursor(0,1);
    lcdDisplay.print(lcd2text);
    lcd2now=lcd2text;
  }
  if (digitalRead(PIN_CARD))        CardInserted=false;  // карта вставлена
  else                              CardInserted=true;  // карта вынута
  
  /*if (Unlocking)                    digitalWrite(PIN_UNLOCK2, HIGH);  // замок разблокирован
  else                              digitalWrite(PIN_UNLOCK2, LOW);  // замок заблокирован
  */
  if ((CardInserted)&&(ModeNoCard)) 
  {
    ModeNoCard=false;
    ModeCardInserted=true;
    //ModeNeedKeys=true;
    CardInsertTimer=millis();    
  }
  if ((!CardInserted)&&(!ModeNoCard))   // Карта вынута - обнуляем все 
  {
    msgNoCardShow=false;
    ModeNoCard=true; 
    ModeNeedKeys=false;
    ModeCardInserted=false;
    ModeCardInserted=false;   
    ModeNeedPass=false;
    ModeWrongPass=false; 
    ModeCorrectPass=false;
    ModeShowCode=false; 
    ModeNeedCode=false;        
    ModeWrongCode=false;      
    ModeCorrectCode=false;
    ModeUnlocked=false;
    ModeCodeBlinking=false;
    ModeCodeBlinking=true;     // Режим мигания случайных наборов чисел на цифровом индикаторе
    ModeDigitSwitching=false;  // Режим быстрой смены случайных чисел на цифровом индикаторе
    ModeDigitChecking=false;   // Режим мигания введенного кода на цифровом индикаторе
    digitDisplay.shutdown(0, true);  // turns on display
    //ModeCursorBlinking=false;
    UserPassword="";
    UserCode="";
    PassCursorPos=0;  
    CodeCursorPos=0; 
    Unlocking=false;
  }

}


