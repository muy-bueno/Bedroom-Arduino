// C++ code
//
#include <Wire.h>
#define address 0x20
int perMode = 1;
int winMode = 1;
int patMode = 1;
int syncMode = 1;
int modeOnStart = digitalRead(12);
int step = 1;
int winStep = 1;
int patStep = 0;
int pinStates = 0b00000000;
int potVal = 0;
unsigned long stopTime;


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  powerOn();
}


void powerOn()
{ //startup animation
  for(int i=2; i<=7; i++){
    digitalWrite(i, HIGH);
  }
  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(1000);

  for(int i=2; i<=7; i++){
    digitalWrite(i, LOW);
  }
  Wire.beginTransmission(address);
  Wire.write(0xFF);
  Wire.endTransmission();
}



void perButton()
{	//handles perimiter button presses
  while(digitalRead(8) == HIGH){}//debounce
  perMode++;
  allOff(2);

  if(perMode == 5)
    perMode = 1;
}

void winButton()
{	//handles window button presses
  while(digitalRead(9) == HIGH){}//debounce
  winMode++;
  allOff(3);

  if(winMode == 4)
    winMode = 1;


}
void patButton()
{	//handles pattern button presses
  while(digitalRead(10) == HIGH){}//debounce
  patMode++;
  // allOff(4); NOT NEEDED, doesn't save previous state!

  if(patMode == 10){
    patMode = 1;
    patStep = 0; // change if more modes added
  }
  if(patMode == 5 || patMode == 6 || patMode == 7 || patMode == 8)
    patStep = 0;
}

void syncButton(){

  while(digitalRead(13) == HIGH){}//debounce

  syncMode++;
  allOff(1);

  if(syncMode == 2)
    syncMode = 1;


}



//turns various segments off
//mode 1: all off
//mode 2: perimiter off
//mode 3: window off
//mode 4: pattern off
void allOff(int mode)
{

  if(mode == 1){
    for(int i=2; i<=7; i++){
      digitalWrite(i, LOW);
    } 
  }

  else if(mode == 2){
    for(int i=2; i<=5; i++){
      digitalWrite(i, LOW);
    }
  }

  else if(mode == 3){

    for(int i=6; i<=7; i++){
      digitalWrite(i, LOW);
    } 
  }

  else if(mode == 4){

    Wire.beginTransmission(address);
    Wire.write(0xFF);
    Wire.endTransmission();
  }
}




//perimiter animations
//mode 1: increasing
//mode 2: decreasing
//mode 3: all on
//mode 4: all off
void perFunction(int sel)
{

  if(sel == 1){

    if(step == 1){
      digitalWrite(2, HIGH);
      digitalWrite(5, LOW); 
      step++;
    }
    else if(step == 2){
      digitalWrite(3, HIGH);
      digitalWrite(2, LOW);
      step++;
    }
    else if(step == 3){
      digitalWrite(4, HIGH);
      digitalWrite(3, LOW);
      step++;
    }
    else if(step == 4){
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      step = 1;
    }
  }//sel 1
  else if(sel == 2){

    if(step == 1){
      digitalWrite(5, HIGH);
      digitalWrite(2, LOW);
      step++;
    }
    else if(step == 2){
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      step++;
    }
    else if(step == 3){
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      step++;
    }
    else if(step == 4){
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      step = 1;
    }

  }//sel 2
  else if(sel == 3){

    for(int i=2; i<=5; i++){
      digitalWrite(i, HIGH);
    } 

  }//sel 3
  else if(sel == 4){
    //placeholder because lights turn off when mode switches

  }

}


//mode 1: alternating
//mode 2: all on
//mode 3: all off
void winFunction(int sel){

  if(sel == 1){

    if(winStep == 1){

      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);

    }

    else if(winStep == 2){

      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);

    }

    winStep++;
    if(winStep == 3)	winStep = 1;

  }

  else if(sel == 2){

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);

  }
  else if(sel == 3){

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);

  }

}



//pattern function
//mode 1: increasing (red => purple)
//mode 2: decreasing (purple => red)
//mode 3: all on
//mode 4: all off
//mode 5: increasing/decreasing, save state
//mode 6: middle to outside
//mode 7: 1 digit slide
//mode 8: 2 digit slide
//mode 9: 3 digit slide
void patFunction(int sel)
{


  if(sel == 1 || sel == 2){

    if(sel == 1 || patStep == 0){
      patStep++;
      if(patStep == 7)	patStep = 1;
    }
    else if(sel == 2){
      patStep--;
      if(patStep == 0)	patStep = 6;
    }
    if(patStep == 1){ 

      pinStates = 0b00000001;

    }
    else if(patStep == 2){

      pinStates = 0b00000010;

    }
    else if(patStep == 3){

      pinStates = 0b00000100;

    }
    else if(patStep == 4){

      pinStates = 0b00001000;

    }
    else if(patStep == 5){

      pinStates = 0b00010000;

    }
    else if(patStep == 6){

      pinStates = 0b00100000;

    }

  }//sel 1+2

  else if(sel == 3){

    pinStates = 0xFF;
  }//sel 3

  else if(sel == 4){

    pinStates = 0x00;

  }//sel 4

  else if(sel == 5){


    patStep++;
    if(patStep == 13)	patStep = 1;


    if(patStep == 1){ 

      pinStates = 0b00000001;

    }
    else if(patStep == 2){

      pinStates = 0b00000011;

    }
    else if(patStep == 3){

      pinStates = 0b00000111;

    }
    else if(patStep == 4){

      pinStates = 0b00001111;

    }
    else if(patStep == 5){

      pinStates = 0b00011111;

    }
    else if(patStep == 6){

      pinStates = 0b00111111;

    }
    else if(patStep == 7){ 

      pinStates = 0b00111110;

    }
    else if(patStep == 8){

      pinStates = 0b00111100;

    }
    else if(patStep == 9){

      pinStates = 0b00111000;

    }
    else if(patStep == 10){

      pinStates = 0b00110000;

    }
    else if(patStep == 11){

      pinStates = 0b00100000;

    }
    else if(patStep == 12){

      pinStates = 0b00000000;

    }


  }//sel 5
  else if(sel == 6){

    patStep++;
    if(patStep == 7)	patStep = 1;



    if(patStep == 1){ 

      pinStates = 0b00001100;

    }
    else if(patStep == 2){

      pinStates = 0b00011110;

    }
    else if(patStep == 3){

      pinStates = 0b00111111;

    }
    else if(patStep == 4){

      pinStates = 0b00110011;

    }
    else if(patStep == 5){

      pinStates = 0b00100001;

    }
    else if(patStep == 6){

      pinStates = 0b00000000;

    }


  }//sel 6

  else if(sel == 7){

    patStep++;
    if(patStep == 11)	patStep = 1;



    if(patStep == 1){ 

      pinStates = 0b00100000;

    }
    else if(patStep == 2){

      pinStates = 0b00010000;

    }
    else if(patStep == 3){

      pinStates = 0b00001000;

    }
    else if(patStep == 4){

      pinStates = 0b00000100;

    }
    else if(patStep == 5){

      pinStates = 0b00000010;

    }
    else if(patStep == 6){

      pinStates = 0b00000001;

    }
    else if(patStep == 7){ 

      pinStates = 0b00000010;

    }
    else if(patStep == 8){

      pinStates = 0b00000100;

    }
    else if(patStep == 9){

      pinStates = 0b00001000;

    }
    else if(patStep == 10){

      pinStates = 0b00010000;

    }

  }//sel 7
  else if(sel == 8){

    patStep++;
    if(patStep == 9)	patStep = 1;



    if(patStep == 1){ 

      pinStates = 0b00110000;

    }
    else if(patStep == 2){

      pinStates = 0b00011000;

    }
    else if(patStep == 3){

      pinStates = 0b00001100;

    }
    else if(patStep == 4){

      pinStates = 0b00000110;

    }
    else if(patStep == 5){

      pinStates = 0b00000011;

    }
    else if(patStep == 6){

      pinStates = 0b00000110;

    }
    else if(patStep == 7){

      pinStates = 0b00001100;

    }
    else if(patStep == 8){

      pinStates = 0b00011000;

    }


  }//sel 8


  else if(sel == 9){

    patStep++;
    if(patStep == 7)	patStep = 1;



    if(patStep == 1){ 

      pinStates = 0b00111000;

    }
    else if(patStep == 2){

      pinStates = 0b00011100;

    }
    else if(patStep == 3){

      pinStates = 0b00001110;

    }
    else if(patStep == 4){

      pinStates = 0b00000111;

    }
    else if(patStep == 5){

      pinStates = 0b00001110;

    }
    else if(patStep == 6){

      pinStates = 0b00011100;

    }

  }////sel 9
  Wire.beginTransmission(address);
  Wire.write(~pinStates);
  Wire.endTransmission();

  Serial.print(pinStates);
  Serial.print("   ");
  Serial.print(patStep);
  Serial.print("   ");
  Serial.print(sel);
  Serial.print("   ");
}




//custom wait function
//1 = check buttons for activity while waiting using input timing
//2 = check buttons and use pot for timing
void wait(int time, int function)
{

  if(function == 2){

    potVal = analogRead(A0);

  }
  if(function == 1)
    stopTime = millis() + time;
  else if(function == 2)
    stopTime = millis() + potVal;

    Serial.print(stopTime);
  Serial.print("   ");
  Serial.print(perMode);
  Serial.print("   ");
  Serial.print(millis());
  Serial.print("   ");
  Serial.println(potVal);

  if(function == 1 || function == 2){
    while (stopTime > millis()){
      if(digitalRead(8) == HIGH){
        perButton();
        break;
      }

      if(digitalRead(9) == HIGH){
        winButton();
        break;
      }

      if(digitalRead(10) == HIGH){
        patButton();
        break;
      }
      if(function == 2 && digitalRead(11) == HIGH)
        break;
      delay(10);//USE ONLY IN SIM
    }
  }

}



void loop()
{
  if(digitalRead(12) == modeOnStart){//handles synchro mode

    if(digitalRead(8) == HIGH)
      perButton();

    if(digitalRead(9) == HIGH)
      winButton();

    if(digitalRead(10) == HIGH)
      patButton();

    perFunction(perMode);
    winFunction(winMode);
    patFunction(patMode);

  }
  else{

    if(digitalRead(13) == HIGH)
      syncButton();

    //syncFunction(syncMode);
  }
  if(digitalRead(11) == HIGH)
    wait(100, 1);
  else
    wait(0, 2);

}