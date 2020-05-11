classdef MapIndex < Simulink.IntEnumType
  enumeration
    MAP_Transfer_None(0)
    MAP_Transfer_Standard(1)
    MAP_Transfer_VCV(2)
    MAP_Transfer_PRVC_1(3)
    MAP_Transfer_PRVC_2(4)
    MAP_Transfer_AutoTrigger(5)
    MAP_Transfer_Test(6)
    MAP_ALARMS_1(7)
    MAP_ALARMS_2(8)
  end
end 