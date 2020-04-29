

This folder contains the project `VentilatorUI.epw` which will generate the C files required for the GUI of the ventilator.

### Instructions

In order to genrated the .c and .h files required in the ventilator projects (demoxxx or other .uvproj projects) follows these steps.

1. download and install Embedded Wizard [Free Edition](https://www.embedded-wizard.de/pricing/) at https://www.embedded-wizard.de/download selecting the target platform `STM32F746 Discovery`.
2. open file `VentilatorUI.epw` w/ Embedded Wizard Studio by double clicking on it. 
3. Select target STM32F746 and press BUILD. the folder `STM32F746` now contains the generated code. 
4. Copy this folder into  `ventilator/tests/boards/stm32f7disco/ew/app/GeneratedCode` .

Now, the files will be visible into your project.



