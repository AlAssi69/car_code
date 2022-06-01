//* Project Name: car_code
//* Car Name: Falcon 10
//* Description: Controls the movement of the car by using 2 ultrasonic sensors, 2 DC motors and LM298N
//* Principle used: PID
//* Made by Students in Damascus University, CAE Department: Ashraf AlAssi, Ibrahim AlSabagh, Amir AlAwa and Mahmoud AlHariri

//==========================================

// https://playground.arduino.cc/Code/PIDLibrary/
#include <PID_v1.h>
// https://github.com/Martinsos/arduino-lib-hc-sr04
#include <HCSR04.h>
#include <DCMotor.h>
#include <Helper.h>

// PINs used
#define PIN_VCC_FRONT 2
#define PIN_TRIGGER_FRONT 3
#define PIN_ECHO_FRONT 4

#define PIN_VCC_REAR 5
#define PIN_TRIGGER_REAR 6
#define PIN_ECHO_REAR 7

#define PIN_IN1 8
#define PIN_IN2 9

#define PIN_EN 11

#define PIN_IN3 12
#define PIN_IN4 13

// PIDs
double SetpointFront; // distance in cm
double InputFront, OutputFront;
double KpFront = 2, KiFront = 1e-3, KdFront = 0;
PID pidFront(&InputFront, &OutputFront, &SetpointFront, KpFront, KiFront, KdFront, DIRECT);

double SetpointRear; // distance in cm
double InputRear, OutputRear;
double KpRear = 2, KiRear = 1e-3, KdRear = 0;
PID pidRear(&InputRear, &OutputRear, &SetpointRear, KpRear, KiRear, KdRear, DIRECT);

// Ultrasonic sensors
UltraSonicDistanceSensor frontSensor(PIN_TRIGGER_FRONT, PIN_ECHO_FRONT);
UltraSonicDistanceSensor rearSensor(PIN_TRIGGER_REAR, PIN_ECHO_REAR);

// DC Motors
bool isMovingForward = true;
DCMotor rightMotor(PIN_IN1, PIN_IN2, PIN_EN, &isMovingForward);
DCMotor leftMotor(PIN_IN3, PIN_IN4, PIN_EN, &isMovingForward);

// Helper class
Helper helper;

void setup()
{
  // Set baud rate
  Serial.begin(9600);

  // Initialize the inputs
  InputFront = 0;
  InputRear = 0;

  // Setting the preferred distance in cm
  SetpointFront = 15;
  SetpointRear = 15;

  // Turning the PIDs on
  pidFront.SetOutputLimits(-255, 255);
  pidFront.SetSampleTime(50);
  pidFront.SetMode(AUTOMATIC);

  pidRear.SetOutputLimits(-255, 255);
  pidRear.SetSampleTime(50);
  pidRear.SetMode(AUTOMATIC);

  // Initializing sensors Vcc pins
  uint8_t sensorsVccPins[] = {PIN_VCC_FRONT, PIN_VCC_REAR};
  helper.pinModes(sensorsVccPins, OUTPUT);
  helper.digitalWrites(sensorsVccPins, HIGH);

  // This delay is just for safety so the car won't start immediately
  delay(5 * 1000);
}

// Declaring some variables to take an average for the measured distance
double avrFront = 0;
double avrRear = 0;
const int sampleNum = 10;
int counter = 0;

// A unified output
double Output = 0;

void loop()
{
  // Calculates the distance in cm at a given temperature (19.307°C by default)
  InputFront = frontSensor.measureDistanceCm(27);
  InputRear = rearSensor.measureDistanceCm(27);

  // Make sure the distance is positive
  if (InputRear <= 0)
    InputRear = 0;
  if (InputFront <= 0)
    InputFront = 0;

  // Summing the inputs to calculate the average distance
  avrFront += InputFront;
  avrRear += InputRear;

  counter++;

  delay(1);

  // If counter equals sampleNum, enter the loop and start calculating the correct response. Otherwise continue to the next measurement
  if (counter == sampleNum)
  {
    // Calculating the average and assigning it to inputs
    avrFront /= sampleNum;
    avrRear /= sampleNum;
    InputFront = avrFront;
    InputRear = avrRear;
    avrFront = avrRear = counter = 0;

    // Computing the correct response (output) as a PWM to send it to the dc motors
    pidFront.Compute();
    pidRear.Compute();

    // Make sure the output is positive. Output is negative when the error is negative, i.e., the distance is larger than the setpoint
    OutputRear = abs(OutputRear);
    OutputFront = abs(OutputFront);

    // Processing...
    // Stops if distance is too small
    if (InputFront < SetpointFront && InputRear < SetpointRear)
      OutputFront = OutputRear = Output = 0;

    // Move towards the rear (backward)
    else if (InputFront < SetpointFront && InputRear > SetpointRear)
    {
      isMovingForward = false;
      Output = OutputRear;
      OutputFront = 0;
    }
    // Move towards the front (forward)
    else if (InputFront > SetpointFront && InputRear < SetpointRear)
    {
      isMovingForward = true;
      Output = OutputFront;
      OutputRear = 0;
    }
    // Change the output according to the last output used
    else if (InputFront > SetpointFront && InputRear > SetpointRear)
    {
      if (isMovingForward)
      {
        Output = OutputFront;
        OutputRear = 0;
      }
      else
      {
        Output = OutputRear;
        OutputFront = 0;
      }
    }

    rightMotor.move(Output);
    leftMotor.move(Output);

    // Plotting input and output for debugging purposes
    helper.plot("InputFront", InputFront);
    helper.plot("OutputFront", OutputFront);
    helper.plot("InputRear", InputRear);
    helper.plot("OutputRear", OutputRear, true);
  }
}
