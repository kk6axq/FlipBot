# Main Board
The main PCB interfaces with all the major components of the robot. The core of the board is a Teensy 3.2 microcontroller, which was selected for the large number of interrupts and processing power available to handle high speed encoder measurement. The functions of the board can be roughly divided into three sections: communications, sensing, and output.

<img src="https://github.com/kk6axq/FlipBot/blob/main/media/Main%20PCB%20Render.JPG?raw=true" alt="Main PCB Render" width="600">

Main PCB Render from KiCAD.

## Communications
The board communicates with the controller using an NRF24 radio module. The received command packets, detailed more thoroughly in the controller documentation, are processed, fused with sensor data, and converted into motor commands.

Programming and serial monitoring is handled over the Teensy's onboard USB connector. Due to backfeeding concerns, the VUSB jumper on the Teensy was cut, meaning that it can only be programmed when the board is receiving through the external 5V supply pins.

## Sensing
The two major sensors on board are the MPU6050 accelerometer/gyro, and the power management board. The MPU6050 is currently used to measure the direction of the gravity vector for determining orientation. Depending on the orientation, the controls may be swapped to ensure the driver can continue to operate it normally.

The power management board provides connections for a monitoring port, where the Teensy could receive data on current draw, voltage, and fault states. Currently, there is no need for this in the system, and thus it is not implemented. The Teensy does not do anything that requires a shutdown procedure, so there was not a need for it to monitor the power, as it will automatically be turned off if fault conditions exist. This link could be used for possible additional features, including:
* **Safe shutdown of the payload.** If there were a payload like a Raspberry Pi, it would be useful to know when power is going to be cut so the computer could be safely shutdown.
* **Voltage indicator on controller.** Currently, the controller only communicates to the main board, but a future design could include an indicator LED for battery status and a signal could be sent to the controller to indicate that.
* **Automatic docking/recharging.** If the project were to become autonomous, monitoring the power could enable it to return to a docking station for more power.
* **Current restriction.** The main board could throttle the motors based on the current draw to optimize battery life or detect stall conditions.

## Output
The output system consists of a level converter and two motor controllers. The current revision uses the TXB0108 bidirectional level converter and two A4950 3.5A motor driver ICs. The level converter does not currently interface well with the motor controllers, likely because of the direction sensing not handling the current draw of the motor controllers well, and it will be replaced in the next revision. The motor controllers have been a bit temperamental, but appear to work well when they are installed undamaged. Together, they drive two Pololu 34:1 geared DC motors that can each provide around 63 oz-in of torque at 2.1A current draw.

Once the level shifter is replaced, the interface should be fairly straightforward, as other motor controllers using the same chipset just send normal PWM signals.
