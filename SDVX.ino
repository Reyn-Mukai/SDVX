#include <Keyboard.h>
#include <Mouse.h>

//A Mega32u4 based SDVX/K-Shoot Mania Controller

//Define "PROMICRO" or "MICRO" depending on which board you are using
#define PROMICRO

//Arduino Pro Micro pin declarations
#ifdef PROMICRO
#define E1A 0
#define E1B 1
#define E2A 2
#define E2B 7
#define BS1 21 //A3
#define BS2 20 //A2
#define BS3 19 //A1
#define BS4 18 //A0
#define BS5 15
#define BW1 14
#define BW2 16
#endif

//Arduino Micro pin declarations
#ifdef MICRO
#define ACTIVELED 10
#define PROGSW 4
#endif

//Button keybindings
#define BS1char 0x32
#define BS2char 0x33
#define BS3char 0x34
#define BS4char 0x35
#define BS5char 0xB0
#define BW1char 0x31
#define BW2char 0x36

volatile int encoder1Pos = 0;
volatile int encoder2Pos = 0;
int active = 0;
int bs1flag, bs2flag, bs3flag, bs4flag, bs5flag, bw1flag, bw2flag = 0;
int bs1deb, bs2deb, bs3deb, bs4deb, bs5deb, bw1deb, bw2deb = 0;

void setup(){
  #ifdef PROMICRO
  pinMode(E1A, INPUT_PULLUP);
  pinMode(E1B, INPUT_PULLUP);
  pinMode(E2A, INPUT_PULLUP);
  pinMode(E2B, INPUT_PULLUP);
  pinMode(BS1, INPUT_PULLUP);
  pinMode(BS2, INPUT_PULLUP);
  pinMode(BS3, INPUT_PULLUP);
  pinMode(BS4, INPUT_PULLUP);
  pinMode(BS5, INPUT_PULLUP);
  pinMode(BW1, INPUT_PULLUP);
  pinMode(BW2, INPUT_PULLUP);
  #endif

  #ifdef MICRO
  pinMode(E1A, INPUT_PULLUP);
  pinMode(E1B, INPUT_PULLUP);
  pinMode(E2A, INPUT_PULLUP);
  pinMode(E2B, INPUT_PULLUP);
  pinMode(BS1, INPUT_PULLUP);
  pinMode(BS2, INPUT_PULLUP);
  pinMode(BS3, INPUT_PULLUP);
  pinMode(BS4, INPUT_PULLUP);
  pinMode(BS5, INPUT_PULLUP);
  pinMode(BW1, INPUT_PULLUP);
  pinMode(BW2, INPUT_PULLUP);
  pinMode(ACTIVELED, OUTPUT);
  pinMode(PROGSW, INPUT_PULLUP);
  #endif

  attachInterrupt(digitalPinToInterrupt(E1B), Encoder1A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E1A), Encoder1B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E2A), Encoder2A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E2B), Encoder2B, CHANGE);

  Serial.begin(115200);
  Serial.println("Pins initialized.");
}

void loop(){
  posOverflow(); //Checks/fixes overflow
  #ifdef PROMICRO  
  libInitPro();
  #endif
    
  #ifdef MICRO
  libInit();
  libTerm();
  #endif

  #ifdef PROMICRO
  while(active == true){
    posOverflow();
    kbPress(BS1, &bs1flag, BS1char);
    kbPress(BS2, &bs2flag, BS2char);
    kbPress(BS3, &bs3flag, BS3char);
    kbPress(BS4, &bs4flag, BS4char);
    kbPress(BS5, &bs5flag, BS5char);
    kbPress(BW1, &bw1flag, BW1char);
    kbPress(BW2, &bw2flag, BW2char);
  }
  #endif
  
  #ifdef MICRO
  while(digitalRead(PROGSW) == LOW){
    posOverflow();
    kbPress(BS1, &bs1flag, BS1char);
    kbPress(BS2, &bs2flag, BS2char);
    kbPress(BS3, &bs3flag, BS3char);
    kbPress(BS4, &bs4flag, BS4char);
    kbPress(BS5, &bs5flag, BS5char);
    kbPress(BW1, &bw1flag, BW1char);
    kbPress(BW2, &bw2flag, BW2char);
  }
  #endif

  kbDeb(BS1, &bs1deb); //Serial debug
  kbDeb(BS2, &bs2deb);
  kbDeb(BS3, &bs3deb);
  kbDeb(BS4, &bs4deb);
  kbDeb(BS5, &bs5deb);
  kbDeb(BW1, &bw1deb);
  kbDeb(BW2, &bw2deb);
}

/*********************************************Micro Functions*********************************************/
#ifdef MICRO

void libInit(){ //Initializes KB/mouse libs when switched to gamepad mode
  if(digitalRead(PROGSW) == LOW && active == false){
    active = true;
    digitalWrite(ACTIVELED, HIGH);
    Keyboard.begin();
    Mouse.begin();
    debReset();
    Serial.println("Mouse & KB lib active");
  }
}

void libTerm(){ //Terminates KB/mouse libs when switched to debug mode
  if(digitalRead(PROGSW) == HIGH && active == true){
    active = false;
    digitalWrite(ACTIVELED, LOW);
    Keyboard.releaseAll();
    Keyboard.end();
    Mouse.end();
    Serial.println("Mouse & KB lib inactive");
    flagReset();
  }
}

void flagReset(){ //Resets key press flags to 0
  if(digitalRead(PROGSW) == HIGH && active == false){ 
    bs1flag=bs2flag=bs3flag=bs4flag=bs5flag=bw1flag=bw2flag=0;
    Serial.println("Flags reset");
  }
}

void debReset(){ //Resets debug flags to 0
  if(digitalRead(PROGSW) == LOW && active == true){
    bs1deb=bs2deb=bs3deb=bs4deb=bs5deb=bw1deb=bw2deb=0;
    Serial.println("Deb reset");
  }
}

#endif
/*******************************************Pro Micro Functions*******************************************/
#ifdef PROMICRO

void libInitPro(){
  if(digitalRead(BS5) == LOW && digitalRead(BW1) == LOW && digitalRead(BW2) == LOW){
    active = true;
    Keyboard.begin();
    Mouse.begin();
    Serial.println("Mouse & KB lib active");
    for(int i = 0;i < 3;i++){
      Serial.println("Entered LED blink for loop");
    }
  }
}

#endif
/********************************************Shared Functions*********************************************/

void kbPress(int pin, int *flag, int key){ //KB input function
  if(digitalRead(pin) == LOW && *flag == 0){
    Keyboard.press(key);
    *flag = 1;
  }
  if(digitalRead(pin) == HIGH && *flag == 1){
    Keyboard.release(key);
    *flag = 0;
  }
}

void kbDeb(int pin, int *deb){ //KB debug function
  if(digitalRead(pin) == LOW && *deb == 0){
    Serial.print("Pin: ");
    Serial.print(pin);
    Serial.println(" is LOW");
    *deb = 1;
  }
  if(digitalRead(pin) == HIGH && *deb == 1){
    Serial.print("Pin: ");
    Serial.print(pin);
    Serial.println(" is HIGH");
    *deb = 0;
  }
}

void posOverflow(){ //Resets encoder position (unused)
  if(encoder1Pos >= 32700 || encoder1Pos <= -32700){
    encoder1Pos = 0;
    Serial.println("encoder1Pos controlled reset");
  }
  if(encoder2Pos >= 32700 || encoder2Pos <= -32700){
    encoder2Pos = 0;
    Serial.println("encoder2Pos controlled reset");
  }
}

//X knob (left) CW -> Right
//Y knob (right) CW -> Up

void Encoder1A(){
  // look for a low-to-high on channel A
  if(digitalRead(E1A) == HIGH){ 
    // check channel B to see which way encoder is turning
    if(digitalRead(E1B) == LOW){  
      encoder1Pos = encoder1Pos + 1;         // CW
      if(active == true){
        Mouse.move(1,0,0);
      }
    } 
    else{
      encoder1Pos = encoder1Pos - 1;         // CCW
      if(active == true){
        Mouse.move(-1,0,0);
      }
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(E1B) == HIGH) {   
      encoder1Pos = encoder1Pos + 1;          // CW
      if(active == true){
        Mouse.move(1,0,0);
      }
    } 
    else {
      encoder1Pos = encoder1Pos - 1;          // CCW
      if(active == true){
        Mouse.move(-1,0,0);
      }
    }
  }
}

void Encoder1B(){
  // look for a low-to-high on channel B
  if(digitalRead(E1B) == HIGH){   
   // check channel A to see which way encoder is turning
    if(digitalRead(E1A) == HIGH){  
      encoder1Pos = encoder1Pos + 1;         // CW
      if(active == true){
        Mouse.move(1,0,0);
      }
    } 
    else{
      encoder1Pos = encoder1Pos - 1;         // CCW
      if(active == true){
        Mouse.move(-1,0,0);
      }
    }
  }
  // Look for a high-to-low on channel B
  else{ 
    // check channel B to see which way encoder is turning  
    if(digitalRead(E1A) == LOW){   
      encoder1Pos = encoder1Pos + 1;          // CW
      if(active == true){
        Mouse.move(1,0,0);
      }
    } 
    else{
      encoder1Pos = encoder1Pos - 1;          // CCW
      if(active == true){
        Mouse.move(-1,0,0);
      }
    }
  }
}
void Encoder2A(){
  // look for a low-to-high on channel A
  if(digitalRead(E2A) == HIGH){ 
    // check channel B to see which way encoder is turning
    if(digitalRead(E2B) == LOW){  
      encoder2Pos = encoder2Pos + 1;         // CW
      if(active == true){
        Mouse.move(0,1,0);
      }
    } 
    else{
      encoder2Pos = encoder2Pos - 1;         // CCW
      if(active == true){
        Mouse.move(0,-1,0);
      }
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(E2B) == HIGH) {   
      encoder2Pos = encoder2Pos + 1;          // CW
      if(active == true){
        Mouse.move(0,1,0);
      }
    } 
    else {
      encoder2Pos = encoder2Pos - 1;          // CCW
      if(active == true){
        Mouse.move(0,-1,0);
      }
    }
  }
}

void Encoder2B(){
  // look for a low-to-high on channel B
  if(digitalRead(E2B) == HIGH){   
   // check channel A to see which way encoder is turning
    if(digitalRead(E2A) == HIGH){  
      encoder2Pos = encoder2Pos + 1;         // CW
      if(active == true){
        Mouse.move(0,1,0);
      }
    } 
    else{
      encoder2Pos = encoder2Pos - 1;         // CCW
      if(active == true){
        Mouse.move(0,-1,0);
      }
    }
  }
  // Look for a high-to-low on channel B
  else{ 
    // check channel B to see which way encoder is turning  
    if(digitalRead(E2A) == LOW){   
      encoder2Pos = encoder2Pos + 1;          // CW
      if(active == true){
        Mouse.move(0,1,0);
      }
    } 
    else{
      encoder2Pos = encoder2Pos - 1;          // CCW
      if(active == true){
        Mouse.move(0,-1,0);
      }
    }
  }
}
