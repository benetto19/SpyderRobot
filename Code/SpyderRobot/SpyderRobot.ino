#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150 //min/max pulse length count: tweak according to servo
#define SERVOMAX 600

class ServoInfo {
  uint16_t pos; uint16_t num;
public:
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



void setup() {
  pwm.begin();
  pwm.setPWMFreq(60); //60Hz updates

}

void loop() {
  

}

//servoNum from 0 to 5, duration <= SERVOMAX - SERVOMIN
void moveServo(ServoInfo servo, uint16_t degrees) { 
  uint16_t pos = servo.getPosition();
  if(degrees > 0) { //if positive
  for (uint16_t pulse = pos; pulse <= pos + degrees; pulse++) { //may or may not work
    pwm.setPWM(servo.getNum(), 0, pulse);} //fix
  }
  else {
    
  }
}


  

