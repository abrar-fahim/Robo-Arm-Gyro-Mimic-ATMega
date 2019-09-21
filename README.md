# Robo-Arm
Motion Controller Robotic Arm Implemented with ATMega32

A robotic arm with 6 servos and 3 rotational degrees of freedom is controlled by a controller module taking rotational orientation readings using a gyroscope (MPU 6050) unit.

The controller measures its roll, pitch and yaw values and transmits them via a pair of bluetooth modules (HC 05) to the arm,
where the. roll, pitch and yaw values are mapped to the position of the servos.


Two circuit modules, each with an ATMega32 microchip.
Communication via two HC 05 bluetooth modules which were previously paired.


Use uart folder to use Serial I/O for debugging independently.
Other folders already have required uart directories
