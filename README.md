Ventilator
==========



This repository contains the code which runs on the ventilator.



**This document is still in draft format.**

### Latest firmware

In https://github.com/icub-tech-iit/ventilator-FI5/blob/code/application/bin/fi5app.hex

### Folder organization

This repository is organized as follows:

- Folder `test` contains the continuous and incremental tests done on the development board which was eventually used for the ventilator. In this folder there are  libraries for RTOS, HAL, graphics, C++ utilities, and also a dedicated folder with a project for each test. The project under folder `demo013` is the one used for the public presentation of the ventilator FI5 on 13 may 2020.
- Folder `VentilatorUI` contains the project of the user interface shown on the display of the ventilator.
- Folders `drivers` and `generated` contain the embedded C code used for reading sensors and applying actuations.
- Folder `fsm` contains the MATLAB code for the state machine of the ventilator.
- Folder `application` will contain the entry point of the embedded application running on the ventilator. At the time of writing this application is under `tests/boards/stm32f7disco/demo013`, but soon the latest demo will be copied into this `application` folder.
- Folder `libraries` will contain the libraries which now are under folder `tests/boards/stm32f7disco` so that they can be linked or used by the application. The libraries are:
  - `stm32hal`:  contains the hardware abstraction layer from ST Microelectronics customized for our development board and compiled into a static library.
  - `stm32-bsp`: contains the board support package offered by STM Microelectronics for our development board which was lightly modified to be used in our demos.
  - `osal`: it contains the RTOS code which beats inside the ventilator.
  - `ew`: contains code from Embedded Wizard which implements the UI on the LCD display of our development board with some changes we did to it so that it could integrate into our applications.  
  - `vnt`: contains a C++ library which wraps HW, OS and offers an easier framework for application development. 



### Description of the application

The application starts running a fully-preemptive RTOS which starts an init thread `tINIT` responsible to initialize all resources required by the application and then starts two threads: 

- the high priority control thread `tCTRL` and 
- the low priority user interface thread `tHMI` . 

The `tCTRL` runs the control loop at 100 Hz and is responsible to:

- acquire sensors data plus user input
- run the control
- apply the actuations

The `tHMI` runs the graphical user interface and displays on the LCD data retrieved by the `tCTRL` thread.

