#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

/*
 TO-DO
 ___________________
  ~Bring in wires
  ~Test code with servos
  ~Finish soldering other PWM driver
  ~Fix CAD model
  ~Reprint CAD parts
  __________________
 */

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

class ServoInfo {
  
public:
  uint16_t pos; uint16_t num;
  ServoInfo(uint16_t num) {
    this->pos = 0;
    this->num = num;
  }
  uint16_t getPosition() {
    return this->pos;
  }
  void setPosition(uint16_t newPos) {
    this->pos = newPos; 
  } 
  uint16_t getNum() {
    return this->num;
  }  
};


#define SERVOMIN 150 //min/max pulse length count: tweak according to servo
#define SERVOMAX 600


//declare all servoinfo's with initial position of 0 and uint16_t channel order
ServoInfo shoulder1(0);
ServoInfo shoulder2(1);
ServoInfo shoulder3(2);
ServoInfo shoulder4(3);
ServoInfo shoulder5(4);
ServoInfo shoulder6(5);
ServoInfo elbow1(6);
ServoInfo elbow2(7);
ServoInfo elbow3(8);
ServoInfo elbow4(9);
ServoInfo elbow5(10);
ServoInfo elbow6(11);
ServoInfo wrist1(12);
ServoInfo wrist2(13);
ServoInfo wrist3(14);
ServoInfo wrist4(15);
ServoInfo wrist5(16);
ServoInfo wrist6(17);

//arrays for easy access to ServoInfo objects
ServoInfo shoulders[] = {shoulder1, shoulder2, shoulder3, shoulder4, shoulder5, shoulder6};
ServoInfo elbows[] = {elbow1, elbow2, elbow3, elbow4, elbow5, elbow6};
ServoInfo wrists[] = {wrist1, wrist2, wrist3, wrist4, wrist5, wrist6};
ServoInfo leg1[] = {shoulder1, elbow1, wrist1};
ServoInfo leg2[] = {shoulder2, elbow2, wrist2};
ServoInfo leg3[] = {shoulder3, elbow3, wrist3};
ServoInfo leg4[] = {shoulder4, elbow4, wrist4};
ServoInfo leg5[] = {shoulder5, elbow5, wrist5};
ServoInfo leg6[] = {shoulder6, elbow6, wrist6};

double pie = (SERVOMAX - SERVOMIN)*360; 


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
      pwm.setPWM(servo.getNum(), 0, pulse);}
  }
}

//moves servo to a set position
void moveToPosition(ServoInfo servo, uint16_t newPos);
void moveToPosition(ServoInfo servo, uint16_t newPos) {
   uint16_t pos = servo.getPosition();
  if(newPos > 0) { //if positive
  for (uint16_t pulse = pos; pulse <= newPos; pulse++) { //may or may not work
    pwm.setPWM(servo.getNum(), 0, pulse);}
  }
  else { //if negative
    for (uint16_t pulse = pos; pulse >= newPos; pulse--) { //may or may not work
      pwm.setPWM(servo.getNum(), 0, pulse);} 
  }
}

void moveLeg(ServoInfo leg[], uint16_t spos, uint16_t epos, uint16_t wpos);
void moveLeg(ServoInfo leg[], uint16_t spos, uint16_t epos, uint16_t wpos) {
  moveServo(leg[0], spos);
  moveServo(leg[1], epos);
  moveServo(leg[2], wpos);
}

void moveLegToPosition(ServoInfo leg[], uint16_t spos, uint16_t epos, uint16_t wpos);
void moveLegToPosition(ServoInfo leg[], uint16_t spos, uint16_t epos, uint16_t wpos) {
  moveToPosition(leg[0], spos);
  moveToPosition(leg[1], epos);
  moveToPosition(leg[2], wpos);
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60); //60Hz updates
  Serial.println("Spyder Initiated");
}

void loop() {
  
  if(Serial.available()) {
    String cmd = Serial.readString();
    Serial.println("You said: " + cmd);
  }
  else {
    Serial.println("Awaiting User Command");
    while(!Serial.available()){
       delay(500);
    }
  }
}
  

