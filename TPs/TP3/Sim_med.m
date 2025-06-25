close all

AA = [0 1 0 0; 0 -1/0.31 981*pi/180 0; 0 0 0 1; 0 0 -144.1 -17.27];
BB = [0; 0; 0; 61.8];
CC = eye(4);
DD = [0; 0; 0; 0];

%%

C = [1 0 0 0];

ctrb([AA [0;0;0;0]; -C [0]], [B;0])


%%
close all
% Realimentacion de estados
KK = [-1.3077, -0.4431, -0.1525, -0.0001];
Acl = AA + BB*KK;

% Accion integral
% KK = [-5.6067, -1.1095, -2.2399, -0.2019, 6.9466];
% Acl = [AA [0;0;0;0]; -[1 0 0 0 0]] + [BB;0]*KK;


% Sistema en lazo cerrado (sin entrada externa)
sys_cl = ss(Acl, [], CC, []);  % sistema autónomo
% sys_cl = ss(Acl, [], [CC [0;0;0;0]], []);

% dt = 0.001;
% t = 0:dt:2.5;
% % x = 0:dt:0.2;
% Bd = [0; 1; 0; 0];
% d = zeros(size(t));
% d(t <= 0.6) = -300;
% sys_ext = ss(Acl, Bd, CC, []);
% [y, t_out, x] = lsim(sys_ext, d, t);

% Estado inicial: impulso en la primera variable
x0 = zeros(size(Acl,1),1);
x0(2) = -82;

% Simulación
t = 0:0.02:3;  % tiempos
% initial(sys_cl, x0, t);
[y, t_out, x] = initial(sys_cl, x0, t);

figure;
stairs(out.pos(:,1));

% Obs
% ini = 2500;
% fin = 4000;
% dt = -0.1;

% Imp
% ini = 1009;
% fin = 1290;
% ini = 4085;
% fin = 4300;
dt = -0.1;

% Refs
% ini = 2500;
% fin = 4000;
% dt = -0.1;

% Int
% ini = 380;
% fin = 15600;
ini = 6000;
fin = 6400;
% dt = -0.1;

% % Gráficos
% Med: #9ddacb; Negro: #2c2b4b; Mod: #a75293; Est: #76a287
figure('Position', [50, 50, 800, 600]); hold on;
% subplot(2,2,1); hold on;
plot(out.tout(ini:fin)-out.tout(ini), out.ref(ini:fin), 'Color', 'black', 'LineStyle', '--');
stairs(out.tout(ini:fin)-out.tout(ini), out.pos(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
stairs(out.tout(ini:fin)-out.tout(ini), out.pos(ini:fin,2), 'LineWidth', 1.3, 'Color', '#d782c3');
plot(out.tout(ini:fin)-out.tout(ini), out.pos(ini:fin,3), 'LineWidth', 1.3, 'Color', '#2c2b4b');
% plot(t_out+1, y(:,1), 'LineWidth', 1.3, 'Color', '#2c2b4b');
xlabel('Tiempo [s]');
ylabel('Pos [cm]');
title('Posición');
legend({'Referencia', 'Medición', 'Estimado', 'Modelo'});
grid();
% xlim([-1,4]);
%
figure('Position', [100, 100, 800, 600]); hold on;
% subplot(2,2,2); hold on;
stairs(out.tout(ini:fin)-out.tout(ini), out.ang(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
stairs(out.tout(ini:fin)-out.tout(ini), out.ang(ini:fin,2), 'LineWidth', 1.3, 'Color', '#d782c3');
plot(out.tout(ini:fin)-out.tout(ini), out.ang(ini:fin,3), 'LineWidth', 1.3, 'Color', '#2c2b4b');
% plot(t_out+1, y(:,3), 'LineWidth', 1.3, 'Color', '#2c2b4b');
xlabel('Tiempo [s]');
ylabel('Ang [°]');
title('Ángulo');
legend('Medición', 'Estimado', 'Modelo');
grid();
% xlim([0,4]);

figure('Position', [100, 100, 800, 600]); hold on;
% subplot(2,2,3); hold on;
stairs(out.tout(ini:fin)-out.tout(ini), out.vel(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
stairs(out.tout(ini:fin)-out.tout(ini), out.vel(ini:fin,2), 'LineWidth', 1.3, 'Color', '#d782c3');
plot(out.tout(ini:fin)-out.tout(ini), out.vel(ini:fin,3), 'LineWidth', 1.3, 'Color', '#2c2b4b');
% plot(t_out+1, y(:,2), 'LineWidth', 1.3, 'Color', '#2c2b4b');
xlabel('Tiempo [s]');
ylabel('Vel [cm/s]');
title('Velocidad');
legend('Medición', 'Estimado', 'Modelo');
grid();
% xlim([0,4]);

figure('Position', [100, 100, 800, 600]); hold on;
% subplot(2,2,4); hold on;
stairs(out.tout(ini:fin)-out.tout(ini), out.w(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
stairs(out.tout(ini:fin)-out.tout(ini), out.w(ini:fin,2), 'LineWidth', 1.3, 'Color', '#d782c3');
plot(out.tout(ini:fin)-out.tout(ini), out.w(ini:fin,3), 'LineWidth', 1.3, 'Color', '#2c2b4b');
% plot(t_out+1, y(:,4), 'LineWidth', 1.3, 'Color', '#2c2b4b');
xlabel('Tiempo [s]');
ylabel('w [°/s]');
title('Velocidad angular');
legend('Medición', 'Estimado', 'Modelo');
grid();
% xlim([0,4]);

% suptitle('Comportamiento del observador');
%}
