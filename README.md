# FlipBot
FlipBot is a small two wheeled robot development platform. It was developed over the Spring 2021 semester as an honors contract project for EGR102, at Arizona State University on the Polytechnic Campus, under the guidance of Dr. Daniel Frank. The goal for this project was to develop a small robot platform and to gain a better understanding of the design considerations in designing a complete system. Where possible, this project attempted to integrate components directly into the design, rather than using breakout boards or modules.
<img src="https://raw.githubusercontent.com/kk6axq/FlipBot/main/media/CAD%20Render%20with%20Wheels.JPG" alt="Project Photo" width="600">

 ## Components
 There are several major components of FlipBot:
 * **Controller.** The robot is controlled by a wireless remote connected to a Wii Nunchuk. This provides a joystick, two buttons, and pitch/roll data that is processed by a microcontroller and sent wirelessly to the main controller onboard the robot. The controller also has onboard battery management hardware to enable operation from a single 18650 battery.
 <img src="https://raw.githubusercontent.com/kk6axq/FlipBot/main/media/Controller%20PCB%20Render.jpg" alt="Controller PCB" width="300">

 * **Wireless link.** The controller and robot use NRF24 radio modules to communicate. These operate in the 2.4GHz ISM band and provide low latency, medium range. While they have the ability to confirm packet reception, in this application the controller sends packets regardless of whether the received is in range or able to respond.
 * **Onboard Power Management.** This project was developed in conjunction with a second honors contract to develop a power management board for use with LiPo batteries. This PCB provides intelligent power monitoring capabilities on the robot. More information about this part is available in the project repository: [https://github.com/kk6axq/PowerMonitor/](https://github.com/kk6axq/PowerMonitor/).

<img src="https://raw.githubusercontent.com/kk6axq/PowerMonitor/main/media/PCB%20Iso%20View.png" alt="PCB Photo" width="600">

 * **Orientation Sensing.** An onboard MPU6050 accelerometer/gyro provides onboard orientation detection, enabling the controls to be reversed if the robot flips upside down.
 * **Main Board.** The main PCB interfaces to the radio, orientation sensor, and the power management board, in addition to providing motor control and processing input from the motor's encoders (not yet implemented).
<img src="https://raw.githubusercontent.com/kk6axq/FlipBot/main/media/Main%20PCB%20Render.JPG" alt="Main PCB" width="300">

## Project Status
Due to incomplete design verification and limited time, the project was not able to achieve full functionality before the end of the semester. The level converter responsible for interfacing between the motor controllers and the microcontroller was not properly matched to the application and would not properly convert, leading the motor controllers to operate sporadically.

This issue, in addition to a few minor routing decisions, is due to be fixed during a revision of the main board scheduled for Summer 2021, with the hope that the project will be functional by Fall 2021.

**Update Fall 2021:** Over the summer, the hardware design issues were mitigated within the existing PCB and a new PCB was milled for the controller. There still remain some phantom issues with controller connectivity and some larger issues with the initial current draw of the motor controllers. At this point, the project is at a holding point, and may or may not see further development.  
