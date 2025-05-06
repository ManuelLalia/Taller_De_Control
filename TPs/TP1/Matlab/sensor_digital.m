close all;

distancia = out.distancia;
tiempo = out.tiempo;

media_dist = mean(distancia);
var_dist = var(distancia);

media_tiempo = mean(tiempo);
var_tiempo = var(tiempo);

res_dist = 3 * sqrt(var_dist);
res_tiempo = 3 * sqrt(var_tiempo);


figure;
stairs(out.tout, distancia, 'Color', '#53a08e') % 'Marker','x')
title('Mediciones del Sensor HC-SR04')
xlabel('Tiempo [s]')
ylabel('Distancia [cm]')
%xlim([0 90])
grid on;

% Histograma
figure;
histogram(distancia, 6, 'Normalization', 'pdf', 'FaceColor', '#a9cba6'); % 20 bins, normalizado como densidad
hold on;

% Superponemos la campana de Gauss teórica
x_values = linspace(32.75, 33.2, 1000);
% x_values = linspace(14, 18, 100);
gauss_fit = normpdf(x_values, media_dist, sqrt(var_dist));
plot(x_values, gauss_fit, 'k-', 'LineWidth', 2);

title('Histograma de la Distancia Medida por el Sensor HC-SR04');
xlabel('Distancia [cm]');
ylabel('Densidad de probabilidad');
legend('Histograma', 'PDF Teórica');
grid on;
xlim([32.8 33.17])
hold off;