function plot_FI5_log(filename)
%#ok<*NASGU>

log = import_FI5_log(filename);
N = size(log,1);

Fs = 100; % Hz
t = (0:N-1)' / Fs;

IV_dc = log(:,7);
OV_cmd = log(:,8);
trigger = log(:,end);

P_R = log(:,10);
S1_Po = log(:,11);
S3_Po = log(:,13);
S4_Po = log(:,15);
S7_Po = log(:,17);

Q_R = log(:,9);
S2_Qo = log(:,12);
S5_Qo = log(:,16);

TD = log(:,18);
MaxTD = log(:,19);

figure('color','white');
tiledlayout(4,1);

ax1 = nexttile;
stairs(t,[IV_dc, OV_cmd*100]);
title('Duty Cycle');
ylabel('[%]');
legend({'IV', 'OV'}); legend('boxoff');
grid on; grid minor;
xlim([t(1) t(end)]);
ylim([-10 110]);

ax2 = nexttile;
stairs(t,[P_R, S1_Po, S3_Po, S4_Po, S7_Po/100]);
title('Pressure');
ylabel('[cmH2O]');
legend({'P_R', 'S1', 'S3', 'S4', 'S7/100'}); legend('boxoff');
grid on; grid minor;
xlim([t(1) t(end)]);
ylim([-10 30]);

ax3 = nexttile;
stairs(t,[Q_R, S2_Qo, S5_Qo]);
title('Flow Rate');
ylabel('[l/min]');
legend({'Q_R', 'S2', 'S5'}); legend('boxoff');
grid on; grid minor;
xlim([t(1) t(end)]);
ylim([-50 50]);

ax4 = nexttile;
stairs(t,[TD, MaxTD]);
title('Volume');
ylabel('[l]');
xlabel('t [s]');
legend({'TD', 'MaxTD'}); legend('boxoff');
grid on; grid minor;
xlim([t(1) t(end)]);
ylim([-0.1 1]);

linkaxes([ax1 ax2 ax3 ax4],'x');
