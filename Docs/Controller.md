# Controller
The controller provides wireless teleoperation capabilities to the robot. It is powered by an ATTiny88 microcontroller with a Wii Nunchuk joystick for human input and an NRF24 radio for wireless communication.

<img src="https://github.com/kk6axq/FlipBot/blob/main/media/Controller%20PCB%20Render.jpg?raw=true" alt="Controller PCB Render" width="600">

Controller PCB Render from KiCAD

## Input
A Wii Nunchuk controller is used for input. The controller receives 3.3V and communicates over I2C with the microcontroller. It provides a joystick, two buttons, and pitch and roll estimation, enabling multiple different control schemes. Rather than convert these into motor commands onboard, the controller sends this raw data to the robot where the selected inputs are used to calculate motor values.

## Protocol
For ease of communication, the microcontroller sends a packet consisting of a `struct` that holds all the input fields and has been converted to a byte array. The packet is sent with no acknowledgement, meaning that it will continue to send even if the robot is not in range. This could be beneficial in situations with spotty range, but comes at the cost of increased power consumption.

The `struct` and its byte array conversion are defined as follows:
```
struct RobotMsg {
  byte linear;
  byte angular;
  bool buttonC;
  bool buttonZ;
  float rollAngle;
  float pitchAngle;
};

typedef union {
  RobotMsg msg;
  byte buffer[sizeof(RobotMsg)];
} convert;
```

## Power
The controller is powered by a single 18650 Lithium Ion battery. An onboard battery manager provides a micro USB charging port and an onboard boost converter outputs a steady 5V for the main systems. The runtime on a full charge has not yet been calculated, and likely will not be, as the project used salvaged batteries that likely have lower than spec capacities.
