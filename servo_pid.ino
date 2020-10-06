/*
Servo Controller with PID

by R.Pelayo

V1 - 10/06/2020
*/


#include <PID_v1.h>
#include <SoftwareSerial.h>
#include <Servo.h>

//pid settings and gains
#define OUTPUT_MIN 0
#define OUTPUT_MAX 180
#define KP 2
#define KI 5
#define KD 1

#define ALARM_PIN 6

Servo myservo;  // create servo object to control a servo

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

double camera_pos, setPoint, outputVal, servo_pos, correction;

//input/output variables passed by reference, so they are updated automatically
PID myPID(&camera_pos, &outputVal, &setPoint, KP, KI, KD, DIRECT);

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);  // center servo motor at start
  servo_pos = 90;
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    double serialData = (double)inputString.toInt();
    camera_pos = map(serialData,0,320,0,180);
    //servo_pos = myservo.read();
    Serial.print("Input: ");
    Serial.println(camera_pos);
    setPoint = servo_pos;
    myPID.Compute();
    correction = map(outputVal,0,255,0,180);
    Serial.print("Output: ");
    Serial.println(correction);
    //outputVal = camera_pos++;
    //myservo.write(outputVal);
    //delay(50);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}



/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}