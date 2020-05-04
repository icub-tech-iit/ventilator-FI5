

## demo008

This demo adds:

-  integration of low level drivers for reading sensors / inputs + writing actuators + outputs 





| Author       | Date       |
| ------------ | ---------- |
| Marco Accame | 4 May 2020 |





## Results



Timings are as follows. The init and actuation time are blocking, read time is non blocking.



| case                                               | init time | read time | actuation time |
| -------------------------------------------------- | --------- | --------- | -------------- |
| 4 pressure + 2 flow + gpio; read BOARD_ALL_SENSORS | 240 msec  | 3854 usec | 393 usec       |
| 4 pressure + 2 flow + gpio; read BOARD_PRESSURE_1  | 240 msec  | 694 usec  | 392 usec       |
|                                                    |           |           |                |



the reading was tested effectively for:

- all 4 pressure sensors + associate temperatures (pressure1 -> S7, pressure2 -> S1, pressure3 -> S4, pressure4 -> S3)
- all 2 flow sensors (flow1 -> S2, flow2 -> S5)

The actuation was tested effectively for 

- valve1 -> PV. min pwm with value = 0, max  with value = 5000
- valve2 -> OOV. reverted logic with respect to PV.
- buzzer -> the PCB mounted one. it must be disabled w/ value > 5000.



tested also:

- HMI works during acquisition
- trace works during acquisition and HMI
- eventviewer works during acquisition also with HMI.



![](.\assets\timing01.jpg)





![](.\assets\timing02.jpg)