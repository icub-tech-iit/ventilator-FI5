//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: ert_main.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.520
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Tue May 19 16:20:24 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <stddef.h>
#include <stdio.h>                // This ert_main.c example uses printf/fflush
#include "controller.h"                // Model's header file
#include "rtwtypes.h"
#include "zero_crossing_types.h"

static controllerModelClass controller_Obj;// Instance of model class

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(controller_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  controllerModelClass::P_controller_T parameters = controller_Obj.getBlockParameters();
  parameters.Controller_control_mode = 1;  // set IDLE
  controller_Obj.setBlockParameters(&parameters);

  controllerModelClass::ExtU_controller_T inputs;
  inputs.enable = true;
  inputs.S1_Pi = 0.F;        // S1 Pressure [code] (float)
  inputs.S2_Qi = 0.F;        // S2 Flow Rate [code] (float)
  inputs.S3_i[0] = 0.F;      // S3 Pressure [code] (float)
  inputs.S3_i[1] = 0.F;      // S3 Temperature [code] (float)
  inputs.S4_Pi = 0.F;        // S4 Pressure [code] (float)
  inputs.S5_Qi = 0.F;        // S5 Flow Rate [code] (float)
  inputs.S7_Pi = 0.F;        // S7 Pressure [code] (float)
  inputs.params[0] = 50.F;   // Zephyr FS [SLPM] (float)
  inputs.params[1] = 160.F;  // HSC LP FS [mbar] (float)
  inputs.params[2] = 150.F;  // HSC HP FS [psi] (float)
  controller_Obj.setExternalInputs(&inputs);

  // Step the model
  controller_Obj.step();

  // Get model outputs here

  const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
  outputs.IV_dc;       // CP valve DC command [%] (float)
  outputs.OV_cmd;      // CFB valve ON/OFF command [-] (boolean)
  outputs.Q_ref;       // Target Flow Rate [L/min] (float)
  outputs.P_ref;       // Target Pressure [cmH2O] (float)
  outputs.signals[0];  // S1 Filtered Pressure [cmH2O] (float)
  outputs.signals[1];  // S2 Flow Rate [L/min] (float)
  outputs.signals[2];  // S3 Filtered Pressure [cmH2O] (float)
  outputs.signals[3];  // S3 Temperature [°C] (float)
  outputs.signals[4];  // S4 Filtered Pressure [cmH2O] (float)
  outputs.signals[5];  // S5 Flow Rate [L/min] (float)
  outputs.signals[6];  // S7 Filtered Pressure [cmH2O] (float)
  outputs.signals[7];  // Tidal Volume estimates [L] (float)
  outputs.signals[8];  // Maximum Tidal Volume per 2 cycles [L] (float)
  outputs.signals[9];  // Assisted Ventilation Trigger [-] (float)

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, const char *argv[])
{
  // Unused arguments
  (void)(argc);
  (void)(argv);

  // Initialize model
  controller_Obj.initialize();

  // Attach rt_OneStep to a timer or interrupt service routine with
  //  period 0.01 seconds (the model's base sample time) here.  The
  //  call syntax for rt_OneStep is
  //
  //   rt_OneStep();

  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(controller_Obj.getRTM()) == (NULL)) {
    //  Perform other application tasks here
  }

  // Disable rt_OneStep() here

  // Terminate model
  controller_Obj.terminate();
  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
