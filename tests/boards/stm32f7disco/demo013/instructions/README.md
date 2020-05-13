

## demo013

This demo refactors demo012 for easier introduction of the FSM. It also introduces a newer version of the controller. It fixes the LEDs and the buzzer. The basic tests on prototype-01 are OK.



- The LEDs on prototype-01 and prototype02 were wrongly cabled. Now they work. The LED on shows the current state (IDLE, CPAP, VCV or PRCV) and it also blinks to show wrong action if the user presses the wrong push-button.
- The buzzer is driven now w/ 0 and 1 rather than w/ a PWM value.



Done also some timing tests.



![](assets\idle.10ms.timeHMI.jpg)

**Figure 1**. The scheduling of the system at 10 ms granularity when in IDLE mode. The control loop **tCTRL** executes every 10 ms the start of sensor acquisition. The thread **tHMI** executes every 66 ms. The CPU is idle about 50% of the time.



![](assets\idle.01ms.timeTickCTRL.jpg)

**Figure 2**. The scheduling of the system at 1 ms granularity when in IDLE mode.




![](assets\prcv.01ms.timeTickCTRL.jpg)

**Figure 3**. The scheduling of the system at 1 ms granularity when in PRCV mode.





| Author       | Date        |
| ------------ | ----------- |
| Marco Accame | 11 May 2020 |




