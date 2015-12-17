  #include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

/*
 TO-DO
 ___________________
~CODE
  __________________
 */

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

class ServoInfo {
  
public:
  uint16_t pos; uint16_t num; char side;
  ServoInfo(uint16_t num, char side) {
    this->pos = 0;
    this->num = num;  
    this->side = side;}
  uint16_t getPosition() {
    return this->pos; }
  void setPosition(uint16_t newPos) {
    this->pos = newPos; } 
  uint16_t getNum() {
    return this->num;}  
};//end class


#define SERVOMIN 150 //min/max pulse length count: tweak according to servo
#define SERVOMAX 600

//declare all servoinfo's with initial position of 0 and uint16_t channel order
ServoInfo shouldR1(14, 'R'); ServoInfo elbowR1(15, 'R'); ServoInfo wristR1(13, 'R');
ServoInfo shouldR2(10, 'R'); ServoInfo elbowR2(11, 'R'); ServoInfo wristR2(9, 'R');
ServoInfo shouldR3(2, 'R'); ServoInfo elbowR3(3, 'R'); ServoInfo wristR3(1, 'R');
ServoInfo shouldL1(2, 'L'); ServoInfo elbowL1(3, 'L'); ServoInfo wristL1(1, 'L');
ServoInfo shouldL2(6, 'L'); ServoInfo elbowL2(7, 'L'); ServoInfo wristL2(5, 'L');
ServoInfo shouldL3(14, 'L'); ServoInfo elbowL3(15, 'L'); ServoInfo wristL3(13, 'L');

//arrays for easy access to ServoInfo objects
ServoInfo allServos[] = {
shouldR1, elbowR1, wristR1,
shouldR2, elbowR2, wristR2,
shouldR3, elbowR3, wristR3,
shouldL1, elbowL1, wristL1,
shouldL2, elbowL2, wristL2,
shouldL3, elbowL3, wristL3 };
uint16_t imprintPos[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

ServoInfo shoulders[] = {shouldR1, shouldR2, shouldR3, shouldL1, shouldL2, shouldL3};
ServoInfo elbows[] = {elbowR1, elbowR2, elbowR3, elbowL1, elbowL2, elbowL3};
ServoInfo wrists[] = {wristR1, wristR2, wristR3, wristL1, wristL2, wristL3};

ServoInfo legR1[] = {shouldR1, elbowR1, wristR1};
ServoInfo legR2[] = {shouldR2, elbowR2, wristR2};
ServoInfo legR3[] = {shouldR3, elbowR3, wristR3};
ServoInfo legL1[] = {shouldL1, elbowL1, wristL1};
ServoInfo legL2[] = {shouldL2, elbowL2, wristL2};
ServoInfo legL3[] = {shouldL3, elbowL3, wristL3};

Adafruit_PWMServoDriver pwmR = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwmL = Adafruit_PWMServoDriver(0x41);

double pie = 2*(SERVOMAX - SERVOMIN); 
String blank = ""; //c++ semantics

//moves servo a set of positions from where it is
void moveServo(ServoInfo servo, uint16_t addPos);
void moveServo(ServoInfo servo, uint16_t addPos) { 
  uint16_t pos = servo.getPosition();
  if(addPos > 0) { //if positive
  for (uint16_t pulse = pos; pulse <= pos + addPos; pulse++) { //may or may not work
    pwm.setPWM(servo.getNum(), 0, pulse);} 
  }
  else { //if negative
    for (uint16_t pulse = pos + addPos ; pulse >= pos; pulse--) { //may or may not work
      pwm.setPWM(servo.getNum(), 0, pulse);}  }//end else
}//emd void

//waits until user input provided
void waitDelay();
void waitDelay() {
  while(!Serial.available()) {
    delay(100);
  }
}
//prompts user and waits for input before proceeding
void prompt(String str);
void prompt(String str) {
  Serial.println(str);  
  waitDelay();
  }//end void

double convertToPosition(double degree);
double convertToPosition(double degree) {
  return (degree/360)*pie;
}

void setup() {
  Serial.begin(9600);
  pwmR.begin();
  pwmR.setPWMFreq(300); //300Hz updates
  pwmL.begin();
  pwmL.setPWMFreq(300); //300Hz updates
  Serial.println("Sypder Initiated");
  Serial.println("Possible commands: ");
  Serial.println("move + [side] + [part]/all[part} + [specific part]/[] + degree ");
}//end setup


void loop() {
  
  if(Serial.available()) {
   String cmd = Serial.readString();
   Serial.println("You said: " + cmd);
    if(cmd.equals("move")) { //move
      prompt("Side: ");
         char side = Serial.readString()[0];
      prompt("Servo: ");
        String part = Serial.readString();
      if(part.equals("servo")) { //servo
        prompt("Servo Number: ");
          int num = Serial.parseInt();
          ServoInfo thisServo = allServos[num];
        prompt("Degrees:");
          int amount = convertToPosition(Serial.parseInt());
          uint16_t newPos = thisServo.getPosition() + amount;
       if(side == 'R') {
          pwmR.setPWM(thisServo.getNum(), 0, newPos); }//endif
       else {
          pwmL.setPWM(thisServo.getNum(), 0, newPos);
       }
       thisServo.setPosition(newPos);
       Serial.println(blank + "Moved Servo " + num + " " + amount + " degrees"); 
      }
      if(part.equals("leg")) {//leg 
        prompt("Side: ");
         char side = Serial.readString()[0];
        prompt("Leg: ");
          int leg = Serial.parseInt();
          ServoInfo thisLeg[] = {shoulders[leg-1], wrists[leg-1], elbows[leg-1]};
        prompt("Degrees: ");
          int amount = convertToPosition(Serial.parseInt());
        for(int i = 0; i <= 2; i++) {
            ServoInfo thisServo = thisLeg[i]; 
            uint16_t newPos = thisServo.getPosition() + amount;
          pwm.setPWM(thisServo.getNum(), 0, newPos);
          thisServo.setPosition(newPos);
        }
      }
      ////MORE WORK HERE
      if(part.equals("all shoulders")) {//shoulders
        prompt("By how many degrees?");
        int amount = convertToPosition(Serial.parseInt());
        for(int i = 0; i <= 5; i++) {
            ServoInfo thisServo = shoulders[i]; 
            uint16_t newPos = thisServo.getPosition() + amount;
          pwm.setPWM(thisServo.getNum(), 0, newPos);
          thisServo.setPosition(newPos);
        }
      }
            ////MORE WORK HERE
      if(part.equals("all elbows")) {//elbows
        prompt("By how many degrees?");
        int amount = convertToPosition(Serial.parseInt());
        for(int i = 0; i <= 5; i++) {
            ServoInfo thisServo = elbows[i]; 
            uint16_t newPos = thisServo.getPosition() + amount;
          pwm.setPWM(thisServo.getNum(), 0, newPos);
          thisServo.setPosition(newPos);
        }
      }
            ////MORE WORK HERE
      if(part.equals("all wrists")) {//wrists
        prompt("By how many degrees?");
        int amount = convertToPosition(Serial.parseInt());
        for(int i = 0; i <= 5; i++) {
            ServoInfo thisServo = wrists[i]; 
            uint16_t newPos = thisServo.getPosition() + amount;
          pwm.setPWM(thisServo.getNum(), 0, newPos);
          thisServo.setPosition(newPos);
        }
      }
 
    }//end move
    else if(cmd.equals("wait")) { //wait
      prompt("Seconds: ");
      int duration = Serial.parseInt();
      delay(1000*duration); 
    }//end wait
    else if(cmd.equals("imprint")) { //remember this position
      for(int i = 0; i <= 17; i++) {
        imprintPos[i] = allServos[i].getPosition();
      }
      prompt("Imprint Saved");      ////MORE WORK HERE
    }
    else if(cmd.equals("reset")) { //reset to last imprint
      for(int i = 0; i <= 17; i++) {
        uint16_t change = imprintPos[i] - allServos[i].getPosition(); 
        pwm.setPWM(allServos[i].getNum(), 0, change);
      }
    }
    //---------------INSERT NEW COMMANDS HERE---------------
    else {//confuse
      prompt("I do not understand your command, master");
    }
  }//end if
  
  else {
    Serial.println("Awaiting Command");
    while(!Serial.available()){
       delay(500);
    }//end while
  }//end else
}//end loop
  

