### This folder contains libraries used by application of FI5



This folder is a placeholder for the libraries or extra code used by the application of the ventilator Fi5.

So far, they are still inside  `ventilator-FI5/tests/boards/stm32f7disco` and are:

- `stm32hal` which holds the low level drivers supplied by STM adapted for the ventilator boards. This code is used by the application in form of a `stm32hal.f7disco.v1280.lib` file with a single inclusion header file `stm32hal.h`.
- `stm32f7-bsp` which holds the bsp of the STM development board inside the ventilator as provided by STM. It is used in the application with direct inclusions of its .c files
- `osal` which holds the used RTOS. This code is used by the application in form of a `osal.cm4.dbg.lib` file with a single inclusion folder `osal/api`. 

- `vnt` is a library of C++ code organized in namespaces. This code is used by the application with direct inclusion of its source files.
- `ew` contains the code from  https://www.embedded-wizard.de/download which moves onto the embedded board the GUI defined in `ventilator-FI5/VentilatorUI`. 









