function run

lungs{1} = struct('Em',60,'E',2.6,'Rm',0.9,'R0',0.4,...
                  'Kv',-0.45,'Kf',0.01,...
                  'Vdot_max',50,'Vdot_min',-30,...
                  'type','Respiratory System High Compliance');

lungs{2} = struct('Em',60,'E',5.9,'Rm',0.4,'R0',0.3,...
                  'Kv',-0.35,'Kf',0.4,...
                  'Vdot_max',50,'Vdot_min',-50,...
                  'type','Respiratory System Mid Compliance');

lungs{3} = struct('Em',60,'E',9.1,'Rm',0.85,'R0',0.1,...
                  'Kv',-0.6,'Kf',0.8,...
                  'Vdot_max',50,'Vdot_min',-70,...
                  'type','Respiratory System Low Compliance');

model_name = 'lungs_compliance';
load_system(model_name);
simin = Simulink.SimulationInput(model_name);
tstop = str2double(get_param(model_name, 'StopTime'));
mdlws = get_param(model_name, 'ModelWorkspace');

figure('color', 'white');
tiledlayout(2,3);             
for i=1:3
    assignin(mdlws, 'Em', lungs{i}.Em);
    assignin(mdlws, 'E',  lungs{i}.E);
    assignin(mdlws, 'Rm', lungs{i}.Rm);
    assignin(mdlws, 'R0', lungs{i}.R0);
    assignin(mdlws, 'Kv', lungs{i}.Kv);
    assignin(mdlws, 'Kf', lungs{i}.Kf);
    assignin(mdlws, 'Vdot_max', lungs{i}.Vdot_max);
    assignin(mdlws, 'Vdot_min', lungs{i}.Vdot_min);
    simout = sim(simin);
    
    nexttile(i);
    plot(simout.yout.time, simout.yout.signals(1).values, 'linewidth', 2);
    xlim([0 tstop]); xlabel('time [s]');
    ylim([-70 60]);
    grid('on'); grid('minor');
    title(lungs{i}.type);
    if i == 1 
        ylabel('Flow rate [lpm]');
    end
    
    nexttile(i+3);
    plot(simout.yout.time, simout.yout.signals(2).values, 'linewidth', 2);
    xlim([0 tstop]); xlabel('time [s]');
    ylim([0 100]);
    grid('on'); grid('minor');
    if i == 1  
        ylabel('Pressure [cmH2O]');
    end
end
