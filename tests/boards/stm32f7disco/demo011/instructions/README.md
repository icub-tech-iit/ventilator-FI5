

## demo011

This demo is the result of tests done for the demo of 7 may.

the objective is to:

- integrate the change of (freq, pmax, ie ratio, tidal volume) w/ knobs
- use of the state machine

if possible not using the debugger.





## Results



Done:

- use of the buttons to change mode and the four params (req, pmax, ie ratio, tidal volume) when in idle
- the change of parameters is done using the debugger. when the mode is IDLE we must change the values of variables `FREQvalue`, `PMAXvalue`, `IERvalue`, `TIDVOLvalue`.

To do:

- fix the reading of the knob positions. the ADC gives only the first reading. it must be a regression bug because on evening of 5 may it worked.
- fix the LEDs which are not driven correctly (always off). maybe they are not connected.
- implement and test button reading w/out debounce.
- integrate the matlab-generated state machine.
- verify why the GUI shortens cycles at end of graphic.
- surely something else.







| Author       | Date       |
| ------------ | ---------- |
| Marco Accame | 4 May 2020 |




