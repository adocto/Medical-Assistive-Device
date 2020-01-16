#include <Stepper.h>
#include <Servo.h>              // Add library
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_MotorShield.h>

Servo flipservo;               // Define any servo name
Servo pushservo;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(200, 2);

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

#define TURN_TIME 460
#define TURN_TIMEPUSH 500

int angle = 0;
int servo_position = 0 ;
void setup() {
 //code for servo  
   flipservo.attach(10);
    // Initially the servo must be stopped 
   flipservo.write(90);

    pushservo.attach(9);
    // Initially the servo must be stopped 
    pushservo.write(90);

      Serial.begin(9600);
 //code for color sensor 
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);

    //code for stepper
 Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor1->setSpeed(1000);  // 10 rpm     
  }  
}

void loop()
{
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");

  delay(2000);


  if(c>3400 && c<4000)//yellow
  {
    convstepper();

    //flip1();
    
    servoflip();

    
    servopush();

   //flip2();
  }
  else if(c<2100 && c>2200)//blue
  {
    convstepper();
   
    servopush();
  }
  
  else //black
  {
  convstepper();
  }
}

void servopush()
{
      //delay(2000);
    // Start turning clockwise
    pushservo.write(0);
    // Go on turning for the right duration
    delay(TURN_TIMEPUSH);
    // Stop turning
    pushservo.write(90);

    //delay(3000);

    pushservo.write(180);
    delay(TURN_TIMEPUSH);
    // Stop turning
    pushservo.write(90);

    //delay(3000);
   

    // Wait for 12s
    //delay(1000);
}

void servoflip()
{
    // Start turning clockwise
    flipservo.write(0);
    // Go on turning for the right duration
    delay(TURN_TIME);
    // Stop turning
    flipservo.write(90);

    // Wait for 12s
   // delay(8000);
}

void convstepper()
{
    Serial.println("Single coil steps");
  //myMotor->step(100, FORWARD, SINGLE); 
  myMotor1->step(144, BACKWARD, DOUBLE);
}

void flip1() 
{ 
  for(angle = 0; angle <= 200; angle += 5)    // command to move from 0 degrees to 180 degrees 
  {                                  
    flipservo.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);                       
  } 
 
  delay(8000);

}  
void flip2(){
    for(angle = 200; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
  {                                
    flipservo.write(angle);              //command to rotate the servo to the specified angle
    delay(15);                       
  }
  delay(10000);  
}


  
