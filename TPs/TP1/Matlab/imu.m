close all;

tiempo = out.tout(1250:end, 1);
angulo = out.angulo_imu(1250:end, 1); % a partir de 1 seg

media_angulo = mean(angulo);
var_angulo = var(angulo);

angulo = angulo - media_angulo;
res_angulo = 3 * sqrt(var_angulo);

figure;
plot(tiempo - 25.14 + 0.1, angulo, 'Color', '#c25848')
title('Mediciones de la IMU')
xlabel('Tiempo [s]')
ylabel('Ángulo [deg]')
xlim([0 90])
grid on;

figure;
histogram(angulo, 'Normalization', 'pdf', 'BinWidth', 0.001, 'FaceColor', '#dba887'); 
hold on;
x_values = linspace(min(angulo), max(angulo), 1000);
gauss_fit = normpdf(x_values + media_angulo, media_angulo, sqrt(var_angulo));
plot(x_values, gauss_fit, 'k-', 'LineWidth', 2);

title('Histograma del Ángulo Medido por la IMU');
xlabel('Ángulo [deg]');
ylabel('Densidad de probabilidad');
legend('Histograma', 'PDF Teórica');
grid on;
hold off;