
%% Input data
EndTime = 30;
load('BreathData.mat'); % to impose breath
load('TargetData_StepStart.mat'); % to impose breath

% Lung characteristics 
LungParam.S1S2DR0KvKf = [60 5.9 0.4 0.3  -0.35 0.4]; % lung characteristic
% Softest  --> [60 2.6 0.9 0.47 -0.45 0.01]
% Average  --> [60 5.9 0.4 0.3  -0.35 0.4]
% Stiffest --> [60 9.1 0.85 0.1 -0.6  0.8]


% Simulation type
Mode = 1;   % 1 forced (mask), 0 assisted (helmet)
Supply = 1; % 1 compensated pressure blender, 0 simple blender

InputData.MaskVolume   = 0.3; %(mask = 0.5, helmet =8)
InputData.Stage1Volume = 0.5;
InputData.Stage2Volume = 0.5;
InputData.FiO2Target   = 0.8;

% Supply Pressures
SupplyPress_Air    = 4000; %mbar
SupplyPress_Oxigen = 4000; %mbar
Throttle_Air       = 0.8; % [0 - 1]
Throttle_Oxigen    = 1;   % [0 - 1]

% Manual Regulator !!NO DATA!!
InputData.Regulator.BrkPress  = [1000]; % mbar
InputData.Regulator.Data      = [25];   % l/min

% Pressure Regulator 4 Bar Inlet pressure & 1.5, 1 and 0.5 bar outlet
% InputData.PReg.BrkPress = [0 1430 1480 1500]; % @ 1500 mbar
% InputData.PReg.BrkPress = [0 750 880 1000];    % @ 1000 mbar
InputData.PReg.BrkPress = [0 695 720 750];    % @ 750 mbar
% InputData.PReg.BrkPress = [0 445 470 500];    % @ 500 mbar
InputData.PReg.Data = [4.75 4.4 3.6 0];        % coefficent of flow


% V1 valve AP Taglia 22 - 2.4mm
% InputData.InValve.BrkDuty  = [0 20 40 60 80 100]; % Perc FS con shift x target da simulazione (TBC Camozzi)
% InputData.InValve.BrkPress = [1000 2000 3000 4000]; % mbar
% InputData.InValve.Data     = [0, 0, 0, 0, 4.5, 28; 
%                               0, 0, 0, 1, 30, 62; 
%                               0, 0, 0, 20, 60, 96;
%                               0, 0, 10, 48, 93, 128]; %l/min
                      
% V1 valve CP Taglia 20 - 4.4mm Pressure compensated Test DATA
InputData.InValve.BrkDuty  = [0 33 40 50 60 70 80 90 100]; % Perc FS 
InputData.InValve.BrkPress = [500 750 1000 1500]; % mbar
InputData.InValve.Data     = [0 0 8.21 19.78 27.57 34.97 42.47 49.92 53.16;
                              0 0 9.15 25.90 39.60 52.18 64.99 78.62 87.69;
                              0 0 8.98 27.54 44.50 58.86 74.14 90.27 103.56;
                              0 0 10.5 32.79 54.53 73.77 92.24 114.4 137.81]; %l/min
%                       
InputData.InValve.DutyMax    = 46;   % perc
InputData.InValve.Period     = 5;    % sec
InputData.InValve.PulseWidth = 40; % perc

% Outletvalve CFB 13mm Kv 2.4
InputData.OutValve.BrkPress = 30;  % mbar
InputData.OutValve.Data     = 269; % l/min  kv = Kv*1000/60 poi calcolo in Utilities

InputData.OutValve.DutyMax  = 1;    % [0-1]
InputData.OutValve.PulseWidth = 55; % perc
InputData.OutValve.PhaseDelay = 2.1; % perc

% user tunable parameters

Control.Triggered = 1; %Triggered cycle
Control.PeeP = 5; %PeeP pressure mbar
Control.Scaling = 1;
Control.Flowby = 1; %Flowby duty
Control.Mode = 0; %1 Pressure, 0 Flow
Control.VentValveP =35;
BreatFreq = 12; %Respirazioni/minuto

% control fixed parameters

Control.ValveZero = 32; %Valve zero duty
Control.CurrentFilter = 100; %Hz

Control.Pressure.Feedforward.Static.Freeze = -0.5;
Control.Pressure.Feedforward.Static.Pressure = [0 2 5 10 15 20 25 30]+Control.PeeP;
Control.Pressure.Feedforward.Static.Duty = [0 1 2 3 4 5 6 6];
Control.Pressure.Feedforward.Dynamic.Pressure = [0 5 10 20 30 40 50 60 200];
Control.Pressure.Feedforward.Dynamic.Duty = [0 1 2 3 5 7 8 8 8];
Control.Pressure.Proportional.MaxDuty = 100;
Control.Pressure.Proportional.MinDuty = -50;
Control.Pressure.Proportional.Error.Pressure = [-30 -25 -20 -15 -10 -5 -2 0 2 5 10 15 20 25 30];
Control.Pressure.Proportional.Error.Gain = [3 3 3 4 4 4 4 2 4 4 4 4 3 3 3];
Control.Pressure.Proportional.Pressure.Pressure = [0 2 5 10 15 20 25 30]+Control.PeeP;
Control.Pressure.Proportional.Pressure.Gain = [1 1 0.8 0.5 0.5 0.8 0.9 1];
Control.Pressure.Integral.Freeze = 0.1;
Control.Pressure.Integral.Reset = 5;
Control.Pressure.Integral.MaxDuty = 15;
Control.Pressure.Integral.MinDuty = -15;
Control.Pressure.Integral.Error.Pressure = [-10 -8 -6 -4 -2 0 2 4 6 8 10];
Control.Pressure.Integral.Error.Gain = [0 0 15 20 15 10 15 20 15 0 0];
Control.Pressure.Integral.Pressure.Pressure = [0 2 5 10 15 20 25 30]+Control.PeeP;
Control.Pressure.Integral.Pressure.Gain = [0 0 1 1 1 1 1 1];

Control.Flow.Scaling = 0.8;
Control.Flow.Feedforward.Static.Freeze = -0.5;
Control.Flow.Feedforward.Static.Flow = [0 3 8 12 20 28 34 40];
Control.Flow.Feedforward.Static.Duty = [0 1 2 3 3 3 3 3];
Control.Flow.Feedforward.Dynamic.Flow = [0 5 10 20 30 40 50 60 200];
Control.Flow.Feedforward.Dynamic.Duty = [0 0 1 2 3 5 5 5 5];
Control.Flow.Proportional.MaxDuty = 100;
Control.Flow.Proportional.MinDuty = -50;
Control.Flow.Proportional.Error.Flow = [-30 -25 -20 -15 -10 -5 -2 0 2 5 10 15 20 25 30];
Control.Flow.Proportional.Error.Gain = [1 1 0.9 0.8 0.7 0.6 0.5 0 1 1 1 1 1 1 1];
Control.Flow.Proportional.Flow.Flow = [0 3 8 12 20 28 34 40];
Control.Flow.Proportional.Flow.Gain = [1 1 0.8 0.5 0.5 0.8 0.9 1];
Control.Flow.Integral.Freeze = 0.1;
Control.Flow.Integral.Reset = 5;
Control.Flow.Integral.MaxDuty = 15;
Control.Flow.Integral.MinDuty = -15;
Control.Flow.Integral.Error.Flow = [-10 -8 -6 -4 -2 0 2 4 6 8 10];
Control.Flow.Integral.Error.Gain = [0 0 15 25 25 20 25 25 15 0 0];
Control.Flow.Integral.Flow.Flow = [0 3 8 12 20 28 34 40];
Control.Flow.Integral.Flow.Gain = [0 0 1 1 1 1 1 1];

% Alarms and Errors

Error.Supply.MinPressure = 1500;
Error.Supply.MaxPressure = 5000;
Error.Regulated.MinPressure = 500;
Error.Regulated.MaxPressure = 1000;
Error.Mouth.MaxPressure = 120;
Error.Out.MaxPressure = 120;


%% Run Sim
dsFiles=dir(['VentilatorModel_v8_5.slx']);
tic
sim(dsFiles.name);
toc

%% Plot results
figure(1)
subplot(2,3,1)
plot(tout,InValveDuty); xlabel ('Time [s]'); ylabel ('Valve Duty [0-100]'); title ('ValveDuty profile');
hold on; grid on; plot(tout,OutValveDuty*100); legend('Inlet','Outlet');
subplot(2,3,2)
plot(tout,InControlPressure); xlabel ('Time [s]'); ylabel ('Relative pressure [mBar]'); title ('Pressure profiles');
hold on; grid on; plot(tout,MouthPress); plot(tout,OutControlPressure); legend('In','Mouth','Out'); ylim([0, max(InControlPressure)*1.1]);
subplot(2,3,3)
plot(tout,FlowrateMix); xlabel ('Time [s]'); ylabel ('LungFlowRate [L/min]'); title ('FlowRate Profile'); ylim([min(FlowrateMix)*1.1,max(InControlFlow)*1.1]);
hold on; grid on; plot(tout,InControlFlow); plot(tout,OutControlFlow); legend('NetFLowRate','FromInValve','FromOutValve')
subplot(2,3,4)
plot(tout,TidalVolumeAbsolute);  xlabel ('Time [s]'); ylabel ('Lung Capacity [L]'); title ('Tidal volume');
hold on; grid on
subplot(2,3,5)
plot(tout,CheckFlow);  xlabel ('Time [s]'); ylabel ('FlowRate [L/min]'); title ('Air Losses from VentTrap');
hold on; grid on; plot(tout,VentFlow); legend('Check 80mbar Flow','Vent 30mbar Flow');
subplot(2,3,6)
plot(tout,TJuncPress); hold on; grid on;
[ax,h1,h2] = plotyy(tout,RegulatedPressure,tout,FiO2*100);  xlabel ('Time [s]'); ylabel ('Pressure [mbar]'); title ('Stage1 Pressure');
hold on; grid on; legend('T-Jun Press','Stage1 OutPress','Oxigen Pecent [%]'); set(ax(2),'YLim',[0 110],'YTick', [0:10:100])

figure(2)
subplot(2,3,1)
hold on; grid on; plot(tout, FiCO2*100); plot(tout, FiCO2InspiredAvg*100); plot(tout,FiO2Effective*100); xlabel ('Time [s]'); ylabel ('FiCO2'); ylabel ('%'); legend('FiCO2','FiCO2InspiredAvg','FiO2Effective');
subplot(2,3,2)
hold on; grid on; plot(tout,TidalVolumeRelative); plot(tout,TidalVolumeAbsolute);xlabel ('Time [s]'); ylabel ('Lung Capacity [L]'); title ('Tidal volume'); legend('Relative','Absolute');
subplot(2,3,3)
hold on; grid on; plot(tout,FlowrateMix); xlabel ('Time [s]'); ylabel ('LungFlowRate [L/min]'); title ('Lung FlowRate');
subplot(2,3,4)
hold on; grid on; plot(tout,MouthPress); xlabel ('Time [s]'); ylabel ('Relative pressure [mBar]'); title ('Pressure profiles');
subplot(2,3,5)
hold on; grid on; plot(tout,InValveDuty/100); plot(tout,OutValveDuty); xlabel ('Time [s]'); ylabel ('Valve Duty'); ylim([-0.1,1.1]); legend('In Valve','Out Valve');
subplot(2,3,6)
hold on; grid on; plot(tout,InControlFlow); plot(tout,OutControlFlow); xlabel ('Time [s]'); ylabel ('Valve Flow Rate [L/min]'); legend('In Valve','Out Valve');

figure(3)
subplot(2,1,1)
hold on; grid on; plot(tout, iPFFWD); plot(tout, iPProportional); plot(tout,iPIntegral); plot(tout,iPFlowby); plot(tout,iPTotal); xlabel ('Time [s]'); ylabel ('Duty %'); title ('Control duty %'); legend('iPFFWD','iPProportional','iPIntegral','iPFlowby','iPTotal');
subplot(2,1,2)
hold on; grid on; plot(tout,TargetPressure); plot(tout,InControlPressure); plot(tout,MouthPress); xlabel ('Time [s]'); ylabel ('Pressure [mbar]'); title ('System pressure'); legend('TargetPressure','InControlPressure','MouthPress');

figure(4)
subplot(2,1,1)
hold on; grid on; plot(tout, iFFFWD); plot(tout, iFProportional); plot(tout,iFIntegral); plot(tout,iFFlowby); plot(tout,iFTotal); xlabel ('Time [s]'); ylabel ('Duty %'); title ('Control duty %'); legend('iFFFWD','iFProportional','iFIntegral','iFFlowby','iFTotal');
subplot(2,1,2)
hold on; grid on; plot(tout,TargetFlow); plot(tout,InControlFlow); plot(tout,FlowrateMix); xlabel ('Time [s]'); ylabel ('Flow rate [L/min]'); title ('Flow'); legend('TargetFlow','InControlFlow','FlowrateMix');

