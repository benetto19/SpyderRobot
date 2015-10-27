#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150 //min/max pulse length count: tweak according to servo
#define SERVOMAX 600

uint16_t shoulders[] = {0, 1, 2, 3, 4, 5};
uint16_t elbows[] = {6, 7, 8, 9, 10, 11};
uint16_t wrists[] = {12, 13, 14, 15, 16, 17};



void setup() {
  pwm.begin();
  pwm.setPWMFreq(60); //60Hz updates

}

void loop() {
  

}

//servoNum from 0 to 5, duration <= SERVOMAX - SERVOMIN
void moveServo(String part, int servoNum, int duration) { 
  uint16_t shoulderServo = shoulders[servoNum]; 
  for (uint16_t pulse = 1; pulse <= duration; pulse++) { //may or may not work
    pwm.setPWM(0, 0, pulse);} //fix
}


  
class ServoInfo {
  int pos; String type; int number;
public:
  ServoInfo(int pos, String type, int number) {
    this->pos = pos;
    this->type = type;
    this->number = number;
  }
  int getPosition() {
    return this->pos;
  }
  void setPosition(int newPos) {
    this->pos = newPos; 
  }

 
  
};
