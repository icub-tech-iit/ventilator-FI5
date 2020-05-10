classdef AlarmIndex < Simulink.IntEnumType
  enumeration
    ALARM_None(0)
    ALARM_Disconnect(1)
    ALARM_PowerSupply(2)
    ALARM_BreakPR1(3)
    ALARM_OverPressureInlet(4)
    ALARM_PEEP(5)
    ALARM_LowVolume(6)
    ALARM_Oxygen(7)
  end
end 