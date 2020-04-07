%% Input data

load('BreathData.mat');
Mode = 1; %1 forced (mask), 0 assisted (helmet)
InputData.MaskVolume = 0.2; %(mask = 0.2, helmet =8)
InputData.Stage1Volume=0.5;
InputData.Stage2Volume=0.5;
InputData.FiO2Target=0.8;
% Supply Pressures
SupplyPress_Air    = 4000; %mbar
SupplyPress_Oxigen = 2500; %mbar
Throttle_Air    = 0.8; % [0 - 1]
Throttle_Oxigen = 1; % [0 - 1]

% Manual Regulator !!NO DATA!!
InputData.Regulator.BrkPress  = [1000]; % mbar
InputData.Regulator.Data      = [10];   % Perc FS

% Pressure Regulator 4 Bar Inlet pressure & 1.5, 1 and 0.5 bar outlet
% InputData.PReg.BrkPress = [0 1430 1480 1500]; % Operating @ 1500 mbar
InputData.PReg.BrkPress = [0; 750; 880; 1000];    % Operating @ 1000 mbar
% InputData.PReg.BrkPress = [0 445 470 500];    % Operating @ 500 mbar
InputData.PReg.Data = [4.75; 4.4; 3.6; 0];        %coefficent of flow


% Inletvalve AP 22 - 2.4mm
InputData.InValve.BrkDuty = [0 20 40 60 80 100]-40; % Perc FS
InputData.InValve.BrkPress = [1000 2000 3000 4000]; % mbar
InputData.InValve.Data = [0, 0, 0, 0, 4.5, 28; 
                          0, 0, 0, 1, 30, 62; 
                          0, 0, 0, 20, 60, 96;
                          0, 0, 10, 48, 93, 128]; %l/min

InputData.InValve.DutyMax  = 50; % perc
InputData.InValve.Period   = 5; % sec
InputData.InValve.PulseWidth = 40; % perc

% Outletvalve  !!NO DATA!!
InputData.OutValve.BrkPress = 30; %mbar
InputData.OutValve.Data     = 266; % l/min

InputData.OutValve.DutyMax  = 1; % [0-1]
InputData.OutValve.PulseWidth = 55; % perc

%% Run Sim
dsFiles=dir(['VentilatorModel_v7.slx']);
tic
sim(dsFiles.name);
toc
%% Plot results

subplot(2,3,1)
plot(tout,duty_InValve); xlabel ('Time [s]'); ylabel ('Valve Duty [0-100]'); title ('ValveDuty profile');
hold on; grid on; plot(tout,duty_OutValve*100); legend('Inlet','Outlet');
subplot(2,3,2)
plot(tout,Valve1Press); xlabel ('Time [s]'); ylabel ('Relative pressure [mBar]'); title ('Pressure profiles');
hold on; grid on; plot(tout,MouthPress); plot(tout,Valve2Press); legend('Valve1','Mouth','Valve2'); ylim([0, 35]);
subplot(2,3,3)
plot(tout,FlowrateMix); xlabel ('Time [s]'); ylabel ('LungFlowRate [L/min]'); title ('FlowRate Profile'); ylim([-40,40]);
hold on; grid on; plot(tout,FlowrateMix_InValve); plot(tout,FlowrateMix_OutValve); legend('NetFLowRate','FromInValve','FromOutValve')
subplot(2,3,4)
plot(tout,Capacity);  xlabel ('Time [s]'); ylabel ('Lung Capacity [L]'); title ('Tidal volume');
hold on; grid on
subplot(2,3,5)
plot(tout,CheckFlow);  xlabel ('Time [s]'); ylabel ('FlowRate [L/min]'); title ('Air Losses from VentTrap');
hold on; grid on; plot(tout,VentFlow); legend('Check 80mbar Flow','Vent 30mbar Flow');
subplot(2,3,6)
plot(tout,TJuncPress); hold on; grid on;
[ax,h1,h2] = plotyy(tout,PressStage1,tout,FiO2*100);  xlabel ('Time [s]'); ylabel ('Pressure [mbar]'); title ('Stage1 Pressure');
hold on; grid on; legend('Stage1 OutPress','Oxigen Pecent [%]'); set(ax(2),'YLim',[0 110],'YTick', [0:10:100])