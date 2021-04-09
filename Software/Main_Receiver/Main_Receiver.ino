/*
  Based off of Easy_RX example from NRFLite.
  NRF code based off of NRFLite library Easy_RX.ino example.
  Joystick to tank drive formula from: https://home.kendra.com/mauser/joystick.html
  MPU6050 code based off of MPU6050 library MPU6050_raw.ino example.

*/

#include <SPI.h>
#include <NRFLite.h>

#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define Z_HIGH_THRESH 15000
#define Z_LOW_THRESH 0
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 14;
const static uint8_t PIN_RADIO_CSN = 15;


struct RobotMsg {
  byte linear;
  byte angular;
  bool buttonC;
  bool buttonZ;
  float rollAngle;
  float pitchAngle;
};

NRFLite _radio;
RobotMsg _radioData;
void setup()
{
  Serial.begin(115200);
  initAccelerometer();

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
  Serial.println("Good to go");
}

void loop()
{
  if (_radio.hasData())
  {
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.



    printMsg(_radioData);

    float linear = driveLinear(_radioData.linear);
    float angular = driveAngular(_radioData.angular);
    Serial.print("Linear: "); Serial.println(linear);
    Serial.print("Angular: "); Serial.println(angular);

    int left = joystickToTankDriveLeft(linear, angular);
    int right = joystickToTankDriveRight(linear, angular);

    bool flipped = readAccelerometer();
    //Assumes positive motor values are clockwise.
    if (!flipped) {
      //Normal orientation.
      //Left side reversed.
      left *= -1; //Reverse left because it needs to be ccw.
    } else {
      //Upside down orientation.
      //Motors are swapped and right side is reversed.
      int temp = left;
      left = right;
      right = -1 * temp; //Right side needs to be reversed to be ccw.
    }
    Serial.print("Flipped: ");
    Serial.print(flipped);
    Serial.print(" Motor values: Left: ");
    Serial.print(left);
    Serial.print(" Right: ");
    Serial.println(right);
  }else{
    bool flipped = readAccelerometer();
    Serial.println(flipped);
  }
}

void printMsg(RobotMsg msg) {
  String out = "Data: Linear: ";
  out += msg.linear;
  out += " Angular: ";
  out += msg.angular;
  out += " Pitch: ";
  out += msg.pitchAngle;
  out += " Roll: ";
  out += msg.rollAngle;
  out += " C: ";
  out += msg.buttonC;
  out += " Z: ";
  out += msg.buttonZ;
  Serial.println(out);
}
int driveLinear(byte input) {
  int val;
  if (input < 140 && input > 132) {
    val = 0;
  } else {
    val = map(input, 27, 230, -255, 255);
  }
  return val;
}

int driveAngular(byte input) {
  if (input < 141 && input > 129) {
    return 0;
  }
  return map(input, 27, 237, -255, 255);
}

/**
   Maps joystick x and y into tank drive
   left motor and right motor speeds.
   Assumes input is -255 to 255, outputs -255 to 255.
*/
float joystickToTankDriveRight(float x, float y) {
  x /= 2.55;
  y /= 2.55;
  x *= -1.0;
  float v = (100 - abs(x)) * (y / 100.0) + y;
  float w = (100 - abs(y)) * (x / 100.0) + x;
  float r = (v + w) / 2;
  float l = (v - w) / 2;
  return r;
}

float joystickToTankDriveLeft(float x, float y) {
  x /= 2.55;
  y /= 2.55;
  x *= -1.0;
  float v = (100 - abs(x)) * (y / 100.0) + y;
  float w = (100 - abs(y)) * (x / 100.0) + x;
  float r = (v + w) / 2;
  float l = (v - w) / 2;
  return l;
}

void initAccelerometer() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  accelgyro.initialize();

}

/**
 * Checks the gravity vector from the IMU,
 * to determine the orientation. Returns
 * true if it is upside down, false otherwise.
 */
bool readAccelerometer() {

  bool up = true;
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print(ax);
  Serial.print(", ");
  Serial.print(ay);
  Serial.print(", ");
  Serial.println(az);
  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);
  if (az < Z_LOW_THRESH) {
    up = false;
  }
  if (az > Z_HIGH_THRESH) {
    up = true;
  }
  return !up;
  //Serial.println(up ? 1 : 0);
}
