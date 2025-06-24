close all

AA = [0 1 0 0; 0 -1/0.31 981*pi/180 0; 0 0 0 1; 0 0 -144.1 -17.27];
BB = [0; 0; 0; 61.8];
CC = eye(4);
DD = [0; 0; 0; 0];

%%
close all
% Realimentacion de estados
KK = [-3.9530, -1.0319, -1.3857, -0.0567];
Acl = AA + BB*KK;
% load("Impulso_realim")

% Seguimiento de referencias
% KK = [-1.5877, -0.4662, -0.5410, -0.0891];
% Acl = AA + BB*KK;


% Sistema en lazo cerrado (sin entrada externa)
sys_cl = ss(Acl, [], CC, []);  % sistema autónomo

% dt = 0.001;
% t = 0:dt:2.5;
% % x = 0:dt:0.2;
% Bd = [0; 1; 0; 0];
% d = zeros(size(t));
% d(t <= 0.6) = -300;
% sys_ext = ss(Acl, Bd, CC, []);
% [y, t_out, x] = lsim(sys_ext, d, t);

% Estado inicial: impulso en la primera variable
x0 = zeros(size(AA,1),1);
x0(2) = -80;

% Simulación
% t = 0:0.02:2;  % tiempos
% initial(sys_cl, x0, t);
% [y, t_out, x] = initial(sys_cl, x0, t);

figure;
stairs(out.pos(:,1));

% Obs
% ini = 2500;
% fin = 4000;
% dt = -0.1;

% Refs
% ini = 2500;
% fin = 4000;
% dt = -0.1;

% Int
% ini = 3500;
% fin = 7210;
ini = 6750;
fin = 7000;
dt = -0.1;

% % Gráficos
figure('Position', [50, 50, 800, 600]); %hold on;
subplot(2,2,1); hold on;
% plot(t_out, y(:,1)); Med: #9ddacb; Negro: #2c2b4b; Mod: #a75293; Est: #76a287
plot(out.tout(ini:fin)-out.tout(ini), out.ref(ini:fin), 'Color', 'black', 'LineStyle', '--');
stairs(out.tout(ini:fin)-out.tout(ini), out.pos(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
% stairs(out.tout(ini:fin)-out.tout(ini), out.pos(ini:fin,2), 'LineWidth', 1.3, 'Color', '#76a287');
plot(out.tout(ini:fin)-out.tout(ini), out.pos(ini:fin,3), 'LineWidth', 1.3, 'Color', '#a75293');
xlabel('Tiempo [s]');
ylabel('Pos [cm]');
title('Posición');
legend({'Referencia','Medición', 'Modelo'});
grid();
%
% figure('Position', [100, 100, 800, 600]); hold on;
subplot(2,2,2); hold on;
% plot(t_out, y(:,3));
stairs(out.tout(ini:fin)-out.tout(ini), out.ang(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
% stairs(out.tout(ini:fin)-out.tout(ini), out.ang(ini:fin,2), 'LineWidth', 1.3, 'Color', '#76a287');
plot(out.tout(ini:fin)-out.tout(ini), out.ang(ini:fin,3), 'LineWidth', 1.3, 'Color', '#a75293');
xlabel('Tiempo [s]');
ylabel('Ang [°]');
title('Ángulo');
legend('Medición', 'Modelo');
grid();

% figure('Position', [100, 100, 800, 600]); hold on;
subplot(2,2,3); hold on;
% plot(t_out, y(:,2));
% stairs(out.tout(ini:fin)-out.tout(ini), out.vel(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
stairs(out.tout(ini:fin)-out.tout(ini), out.vel(ini:fin,2), 'LineWidth', 1.3, 'Color', '#76a287');
plot(out.tout(ini:fin)-out.tout(ini), out.vel(ini:fin,3), 'LineWidth', 1.3, 'Color', '#a75293');
xlabel('Tiempo [s]');
ylabel('Vel [cm/s]');
title('Velocidad');
legend('Estimación', 'Modelo');
grid();

% figure('Position', [100, 100, 800, 600]); hold on;
subplot(2,2,4); hold on;
% plot(t_out, y(:,4));
stairs(out.tout(ini:fin)-out.tout(ini), out.w(ini:fin,1), 'LineWidth', 1.3, 'Color', '#9ddacb');
% stairs(out.tout(ini:fin)-out.tout(ini), out.w(ini:fin,2), 'LineWidth', 1.3, 'Color', '#76a287');
plot(out.tout(ini:fin)-out.tout(ini), out.w(ini:fin,3), 'LineWidth', 1.3, 'Color', '#a75293');
xlabel('Tiempo [s]');
ylabel('w [°/s]');
title('Velocidad angular');
legend('Medición', 'Modelo');
grid();

% suptitle('Comportamiento del observador');
%}
