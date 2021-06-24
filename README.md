# Robo-Arm

Video link: https://youtu.be/0WYU3xY0f8I

## Motion Controller Robotic Arm Implemented with ATMega32

A Robotic Arm with 6 servos and 3 rotational degrees of freedom is controlled by a controller module taking rotational orientation readings using a gyroscope (MPU 6050) unit.

The controller measures its roll, pitch and yaw values and transmits them via a pair of bluetooth modules (HC 05) to the arm,
where the. roll, pitch and yaw values are mapped to the position of the servos.


#### Two circuit modules, each with an ATMega32 microchip.

*Communication via two HC 05 bluetooth modules which were previously paired.*

* **ReceiverAndServo** folder contains the [c files](ReceiverAndServo/servo.c) for programming the receiver ATMega32 unit where the 6 servos are attached.

* **TransmitterAndGyroscope** folder contains [c files](TransmitterAndGyroscope/src/main.c) for programming the transmitter ATMega32 unit where the gyroscope is attached.


Use uart folder to use Serial I/O for debugging independently.
Other folders already have required uart directories.
