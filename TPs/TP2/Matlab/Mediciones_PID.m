close all

plot(out.salida)

%%
close all

N = 1000;
M = 1800;
t = out.tout(N:M);
ref = out.ref(N:end);
error = out.error(N:M,:);
u = out.u(N:M,:);
salida = out.salida(N:M,:);

[y,x] = impulse(T);

figure(); hold on
% subplot(2,1,1); hold on
% plot(linspace(0,0.02*length(ref),length(ref)), ref, 'Color', '#2c2b4b', 'LineWidth', 1.5)
% stairs(t-t(1), salida(:,1), 'Color', '#a75293', 'LineWidth', 1.5)
plot(x+2.1, -7*y, 'Color', '#a75293', 'LineWidth', 1.5)
stairs(t-t(1), salida(:,2), 'Color', '#9ddacb', 'LineWidth', 1.5)
plot([0,2.1], [0, 0], 'Color', '#a75293', 'LineWidth', 1.5)
plot([8,16], [0, 0], 'Color', '#a75293', 'LineWidth', 1.5)
xlabel('t [s]')
ylabel('x [cm]')
% axis([0 180 -15 25])
xlim([0 16])
grid()
legend('Ref', 'Modelo','Mediciones', 'Location', 'SouthEast', 'Orientation', 'horizontal')
% t1 = title('Salida', 'Interpreter', 'latex');

% subplot(2,1,2); hold on
% stairs(t-t(1), u(:,1), 'Color', '#a75293', 'LineWidth', 1.5)
% stairs(t-t(1), u(:,2), 'Color', '#9ddacb', 'LineWidth', 1.5)
% xlabel('t [s]')
% ylabel('x [cm]')
% axis([0 180 -30 30])
% yticks([-20 -10 0 10 20])
% grid()
% legend('Modelo','Mediciones', 'Location', 'NorthWest', 'Orientation', 'horizontal')
% t2 = title('Acción de control', 'Interpreter', 'latex');
% 
sg = sgtitle('Control PD ($k_p = 1.8, k_d = \frac{k_p}{500}$)', 'Interpreter', 'latex');
set([t1 t2 sg], 'FontWeight', 'normal')
