

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class ServoInfo {
  
public:
  uint16_t pos; uint16_t num; int minn; int maxx; 
  ServoInfo(uint16_t num, int minn, int maxx) {
    this->pos = 0; this->minn = minn; this->maxx = maxx;
    this->num = num;  }
  uint16_t getPosition() {
    return this->pos; }
  void setPosition(uint16_t newPos) {
    this->pos = newPos; } 
  uint16_t getNum() {
    return this->num;}  
};//end class

// called this way, it uses the default address 0x40, PWM1 = 0x41
Adafruit_PWMServoDriver pwmR = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwmL = Adafruit_PWMServoDriver(0x41);

/*
 SERVO MIN/MAX RELATIONSHIP TO VISUAL
 RIGHT SHOULDER: INVERSE
 RIGHT ELBOW: DIRECT
 RIGHT WRIST: INVERSE
 LEFT SHOULDER: DIRECT
 LEFT ELBOW: INVERSE
 LEFT WRIST: DIRECT
 */
#define R1WRISTMAX  555 //correct
#define R1WRISTMIN  200
#define R1SHOULDERMAX  430
#define R1SHOULDERMIN  171
#define R1ELBOWMAX  567
#define R1ELBOWMIN  155

#define R2WRISTMAX 555 
#define R2WRISTMIN  200
#define R2SHOULDERMAX  430
#define R2SHOULDERMIN  171
#define R2ELBOWMAX  567
#define R2ELBOWMIN  155

#define R3WRISTMAX  555
#define R3WRISTMIN  200
#define R3SHOULDERMAX 430 
#define R3SHOULDERMIN  171
#define R3ELBOWMAX  567
#define R3ELBOWMIN 155

#define L1WRISTMAX        490 //correct
#define L1WRISTMIN        100
#define L1SHOULDERMAX     350
#define L1SHOULDERMIN     100
#define L1ELBOWMAX        500//estimate
#define L1ELBOWMIN        200//estimate

#define L2WRISTMAX        490 //correct
#define L2WRISTMIN        100
#define L2SHOULDERMAX     350
#define L2SHOULDERMIN     100
#define L2ELBOWMAX        500//estimate
#define L2ELBOWMIN        200//estimate

#define L3WRISTMAX        490 //correct
#define L3WRISTMIN        100
#define L3SHOULDERMAX     350
#define L3SHOULDERMIN     100
#define L3ELBOWMAX        500//estimate
#define L3ELBOWMIN        200//estimate

//#define SERVOMIN  150 
//#define SERVOMAX  600 

ServoInfo shouldR1(14, R1SHOULDERMIN, R1SHOULDERMAX); ServoInfo elbowR1(15, R1ELBOWMIN, R1ELBOWMAX); ServoInfo wristR1(13, R1WRISTMIN, R1WRISTMAX);
ServoInfo shouldR2(10, R2SHOULDERMIN, R2SHOULDERMAX); ServoInfo elbowR2(11, R2ELBOWMIN, R2ELBOWMAX); ServoInfo wristR2(9, R2WRISTMIN, R2WRISTMAX);
ServoInfo shouldR3(2, R3SHOULDERMIN, R3SHOULDERMAX); ServoInfo elbowR3(3, R3ELBOWMIN, R3ELBOWMAX); ServoInfo wristR3(1, R3WRISTMIN, R3WRISTMAX);
ServoInfo shouldL1(2, L1SHOULDERMIN, L1SHOULDERMAX); ServoInfo elbowL1(3, L1ELBOWMIN, L1ELBOWMAX); ServoInfo wristL1(1, L1WRISTMIN, L1WRISTMAX);
ServoInfo shouldL2(6, L2SHOULDERMIN, L2SHOULDERMAX); ServoInfo elbowL2(7, L2ELBOWMIN, L2ELBOWMAX); ServoInfo wristL2(5, L2WRISTMIN, L2WRISTMAX);
ServoInfo shouldL3(14, L3SHOULDERMIN, L3SHOULDERMAX); ServoInfo elbowL3(15, L3ELBOWMIN, L3ELBOWMAX); ServoInfo wristL3(13, L3WRISTMIN, L3WRISTMAX);

ServoInfo shouldersR[] = {shouldR1, shouldR2, shouldR3};
ServoInfo shouldersL[] = {shouldL1, shouldL2, shouldL3};

ServoInfo elbowsR[] = {elbowR1, elbowR2, elbowR3};
ServoInfo elbowsL[] = {elbowL1, elbowL2, elbowL3};

ServoInfo wristsR[] = {wristR1, wristR2, wristR3}; 
ServoInfo wristsL[] = {wristL1, wristL2, wristL3}; 

ServoInfo allLeft[] = { shouldL1, elbowL1, wristL1,shouldL2, elbowL2, wristL2,shouldL3, elbowL3, wristL3 };

ServoInfo allRight[] = { shouldR1, elbowR1, wristR1,shouldR2, elbowR2, wristR2,shouldR3, elbowR3, wristR3 };

String blank = " ";

uint16_t Rconvert(double dec, ServoInfo servo); //converts decimal between 1 and 0 to position
uint16_t Rconvert(double dec, ServoInfo servo) {//elbows are direct, others are inverse
  switch(servo.num) {
    case 15: case 11: case 3: {//elbows, direct
      if(dec == 0.00) {
        Serial.print("MIN: ");
        Serial.println(servo.minn);
        return servo.minn;
      }
      else if(dec == 1) {
        Serial.print("MAX: ");
        Serial.println(servo.maxx);
        return servo.maxx;
      }
      else { 
        Serial.print("NEWPOS: "); 
        uint16_t value = (servo.minn + (servo.maxx-servo.minn)*(dec));
        Serial.println(value);
        return value; }
    }
    default: {//shoulders or wrists, inverse
      if(dec == 0.00) {
        Serial.print("MIN: ");
        Serial.println(servo.maxx);
        return servo.maxx;
      }
      else if(dec == 1) {
        Serial.print("MAX: ");
        Serial.println(servo.minn);
        return servo.minn;
      }
      else { 
        uint16_t value = (servo.maxx - (servo.maxx-servo.minn)*(dec));
        Serial.print("NEWPOS: "); 
        Serial.println(value);
        return value; }
    }
  }
} //endvoid

uint16_t Lconvert(double dec, ServoInfo servo); //converts decimal between 1 and 0 to position
uint16_t Lconvert(double dec, ServoInfo servo) {//elbows are inverse, others are direct
  switch(servo.num) {
    case 15: case 7: case 3: {//elbows, inverse
      if(dec == 0) {
        Serial.print("MIN: ");
        Serial.println(servo.maxx);
        return servo.maxx;
      }
      else if(dec == 1) {
        Serial.print("MAX: ");
        Serial.println(servo.minn);
        return servo.minn;
      }
      else { 
        uint16_t value = (servo.maxx - (servo.maxx-servo.minn)*(dec));
        Serial.print("NEWPOS: ");
        Serial.println(value);
        return value; }
    }
    default: {//shoulders or wrists, inverse
      if(dec == 0) {
        Serial.print("MIN: ");
        Serial.println(servo.minn);
        return servo.minn;
      }
      else if(dec == 1) {
        Serial.print("MAX: ");
        Serial.println(servo.maxx);
        return servo.maxx;
      }
      else { 
        uint16_t value = (servo.minn + (servo.maxx-servo.minn)*(dec));
        Serial.print("NEWPOS: ");
        Serial.println(value);
        return value; }
    }
  }
} //endvoid

//moves specified servo specified amount
void drive(char side, ServoInfo servo, double pos);
void drive(char side, ServoInfo servo, double pos) {
  if(side == 'r') {
    pwmR.setPWM(servo.num, 1024, Rconvert(pos, servo)); 
  }
  else { //side == l
    pwmL.setPWM(servo.num, 1024, Lconvert(pos, servo));
  }
}

void prompt(String str);
void prompt(String str) {
  Serial.print(str);  
  while(!Serial.available()) {
    delay(10);
  }
}//end void


void oneServo(); //drives one at a time by command
void oneServo() { 
  prompt("r/l: ");
    char side = Serial.read(); 
    Serial.println(side);
  prompt("Servo: ");
    int servoNum = Serial.parseInt(); 
    Serial.println(blank + servoNum);
  prompt("Position: ");
    double pos = Serial.parseFloat();
    Serial.println(blank + pos); 
  if(side == 'r'){
    drive(side, allRight[servoNum], pos); 
  }
  else {drive(side, allLeft[servoNum], pos); }
  delay(500);
} 

void standUp() { //shoulders/elbows to 0.5, wrists to 0 
  for(int i = 0; i < 3; i++) {
    drive('r', shouldersR[i], 0.4); 
    drive('l', shouldersL[i], 0.4);
    drive('r', elbowsR[i], 0.5); 
    drive('l', elbowsL[i], 0.5);
    drive('r', wristsR[i], 0.7); 
    drive('l', wristsL[i], 0.7); 
  }
}

void setup() {
  Serial.begin(9600);
  pwmR.begin();
  pwmL.begin();
  
  pwmR.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwmL.setPWMFreq(60); 

  //standUp();

}

void loop() {
oneServo();

}//end loop

