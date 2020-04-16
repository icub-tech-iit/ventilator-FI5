%% Input data
EndTime = 30;
load('BreathData.mat'); % to impose breath

% Lung characteristics 
LungParam.S1S2DR0KvKf = [60 2.6 0.9 0.47 -0.45 0.01]; % Tuned AVG lung characteristic
% Softest  --> [60 2.6 0.9 0.47 -0.45 0.01]
% Average  --> [60 5.9 0.4 0.3  -0.35 0.4]
% Stiffest --> [60 9.1 0.85 0.1 -0.6  0.8]


% Simulation type
Mode = 1;   % 1 forced (mask), 0 assisted (helmet)
Supply = 1; % 1 compensated pressure blender, 0 simple blender

InputData.MaskVolume   = 0.5; %(mask = 0.5, helmet =8)
InputData.Stage1Volume = 0.5;
InputData.Stage2Volume = 0.5;
InputData.FiO2Target   = 0.8;

% Supply Pressures
SupplyPress_Air    = 4000; %mbar
SupplyPress_Oxigen = 2500; %mbar
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
InputData.InValve.BrkDuty  = [0 30 40 50 60 70 80 90 100]; % Perc FS 
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

%% Run Sim
dsFiles=dir(['VentilatorModel_v8_3.slx']);
tic
sim(dsFiles.name);
toc

%% Plot results
figure()
subplot(2,3,1)
plot(tout,duty_InValve); xlabel ('Time [s]'); ylabel ('Valve Duty [0-100]'); title ('ValveDuty profile');
hold on; grid on; plot(tout,duty_OutValve*100); legend('Inlet','Outlet');
subplot(2,3,2)
plot(tout,Valve1Press); xlabel ('Time [s]'); ylabel ('Relative pressure [mBar]'); title ('Pressure profiles');
hold on; grid on; plot(tout,MouthPress); plot(tout,Valve2Press); legend('Valve1','Mouth','Valve2'); ylim([0, max(Valve1Press)*1.1]);
subplot(2,3,3)
plot(tout,FlowrateMix); xlabel ('Time [s]'); ylabel ('LungFlowRate [L/min]'); title ('FlowRate Profile'); ylim([min(FlowrateMix)*1.1,max(FlowrateMix_InValve)*1.1]);
hold on; grid on; plot(tout,FlowrateMix_InValve); plot(tout,FlowrateMix_OutValve); legend('NetFLowRate','FromInValve','FromOutValve')
subplot(2,3,4)
plot(tout,TidalVolumeAbsolute);  xlabel ('Time [s]'); ylabel ('Lung Capacity [L]'); title ('Tidal volume');
hold on; grid on
subplot(2,3,5)
plot(tout,CheckFlow);  xlabel ('Time [s]'); ylabel ('FlowRate [L/min]'); title ('Air Losses from VentTrap');
hold on; grid on; plot(tout,VentFlow); legend('Check 80mbar Flow','Vent 30mbar Flow');
subplot(2,3,6)
plot(tout,TJuncPress); hold on; grid on;
[ax,h1,h2] = plotyy(tout,PressStage1,tout,FiO2*100);  xlabel ('Time [s]'); ylabel ('Pressure [mbar]'); title ('Stage1 Pressure');
hold on; grid on; legend('T-Jun Press','Stage1 OutPress','Oxigen Pecent [%]'); set(ax(2),'YLim',[0 110],'YTick', [0:10:100])

% figure()
% subplot(2,3,1)
% hold on; grid on; [AX, H1, H2]=plotyy(tout, FiCO2*100,tout,FiO2Effective*100); xlabel ('Time [s]'); ylabel ('FiCO2'); ylim([0,30]); ylabel (AX(2),'FiO2'); ylim(AX(2),[0,100]);
% subplot(2,3,2)
% hold on; grid on; plot(tout,TidalVolumeRelative); xlabel ('Time [s]'); ylabel ('Lung Capacity [L]'); title ('Tidal volume');
% subplot(2,3,3)
% hold on; grid on; plot(tout,FlowrateMix); xlabel ('Time [s]'); ylabel ('LungFlowRate [L/min]'); title ('Lung FlowRate');
% subplot(2,3,4)
% hold on; grid on; plot(tout,MouthPress); xlabel ('Time [s]'); ylabel ('Relative pressure [mBar]'); title ('Pressure profiles');
% subplot(2,3,5)
% hold on; grid on; plot(tout,duty_InValve/100); plot(tout,duty_OutValve); xlabel ('Time [s]'); ylabel ('Valve Duty'); ylim([-0.1,1.1]); legend('In Valve','Out Valve');
% subplot(2,3,6)
% hold on; grid on; plot(tout,FlowrateMix_InValve); plot(tout,FlowrateMix_OutValve); xlabel ('Time [s]'); ylabel ('Valve Flow Rate [L/min]'); legend('In Valve','Out Valve');