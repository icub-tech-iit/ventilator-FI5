clear all, clc

%% Input data
% Inletvalve 1.6mm
InputData.InValve.BrkDuty  = [40 60 80 100];
InputData.InValve.BrkPress = [2000 4000 6000]; %mbar
InputData.InValve.Data     = [0, 8.5, 40, 52; 0, 26, 59.5, 80; 20, 54, 84, 109]; %l/min
InputData.InValve.DutyMax  = 55; % perc
InputData.InValve.Period   = 5; % sec
InputData.InValve.PulseWidth = 40; % perc

% Outletvalve ?
InputData.OutValve.BrkPress = 100; %mbar
InputData.OutValve.Data     = 64.5; % l/min
InputData.OutValve.DutyMax  = 1; % [0-1]
InputData.OutValve.PulseWidth = 55; % perc

%% Run Sim
dsFiles=dir(['VentilatorModel_v3*']);
sim(dsFiles.name);

%% Plot results
subplot(2,2,1)
plot(tout,duty_InValve); xlabel ('Time [s]'); ylabel ('Valve Duty [0-100]'); title ('ValveDuty profile');
hold on; grid on; plot(tout,duty_OutValve*100); legend('Inlet','Outlet');
subplot(2,2,2)
plot(tout,MouthPress); xlabel ('Time [s]'); ylabel ('Mouth Press [mBar]'); title ('InputPress Profile');
hold on; grid on
subplot(2,2,3)
plot(tout,Flowrate/60); xlabel ('Time [s]'); ylabel ('LungFlowRate [L/min]'); title ('FlowRate Profile');
hold on; grid on; plot(tout,Flowrate_InValve); plot(tout,Flowrate_OutValve); legend('NetFLowRate','FromInValve','FromOutValve')
subplot(2,2,4)
plot(tout,Capacity);  xlabel ('Time [s]'); ylabel ('Lung Capacity [L]'); title ('Air volume');
hold on; grid on