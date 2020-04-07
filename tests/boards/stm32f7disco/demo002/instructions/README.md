

## Demo version 002 on board **32F746GDISCOVERY**.

This demo tests basic functionalities of the board. It

- blinks a LED at various timing, 
- prints a string on the TRACE port for debug purposes
- uses a graphic LCD display with bitmaps, geometric shapes, user text at various fonts.
- tests the QSPI FLASH

It also shows the use of both C and C++ files.



It does not do much more than the previous demo001, but it selects from the huge  `stm32cube_fw_f7_v1160.zip` archive only the code which is needed to run the demo, so that we can put this code under git.

In particular, the project is organized as the ones we have successfully managed for years in `icub-tech` and is formed as follow:

- it links the library `stm32hal.f7disco.v1280.lib` which contains the STM32 HAL drivers plus  basic configuration of some peripherals. This library has a single inclusion point in the file `stmrehal.h`.

-  it uses a `startup_stm32f7disco.s` file which defines the default IRQHandlers,  size of HEAP and size of STACK.
- it uses a selection of the BSP provided by STM for the `32F746GDISCOVERY` board for the use of the peripherals of major interest to us (so far): LDC display with SDRAM sued for its frame buffer, QSPI FLASH memory.
- a `demo-main.cpp` file. The advantage of the C++ file is that it can include both C and C++ modules (if the C are well constructed in their .h interface) whereas the C file cannot include C++ modules.



### Requirements

The demo requires:

- a ARM Keil MDK compiler (https://www.keil.com/demo/eval/arm.htm)
- the board (https://www.st.com/en/evaluation-tools/32f746gdiscovery.html)
- a mini USB cable.



### How to compile and run

Here is how to run the demo.

All the required code is contained in the git repo.

1. open the project `stm32f7disco/stm32hal/proj/stm32hal.f7.uvprojx` and compile to produce the  `stm32hal.f7disco.v1280.lib` library.

3. open the project `stm32f7disco/demo002/proj/demo002.uvprojx`, compile, attach a USB cable to the board, program the FLASH and run the program

   ![](./assets/running.jpg)




### How this demo project is generated

The project `demo002.uvprojx` is an evolution of project `demo001.uvproj`  with an extraction of only the files relevant to our demo board and target application with a mild customization. When a change to STM original files is required, the change is always tagged with string `IIT-ICUB-TECH-CUSTOMIZATION`. 



