classdef ControlMode < Simulink.IntEnumType
  enumeration
    IDLE(0)
    CPAP(1)
    VCV(2)
    PRVC(3)
  end
end 