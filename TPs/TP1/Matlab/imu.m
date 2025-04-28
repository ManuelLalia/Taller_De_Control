close all;

tiempo = out.tout(50:end, 1);
angulo = out.angulo_imu(50:end, 1); % a partir de 1 seg

media_angulo = mean(angulo);
var_angulo = var(angulo);

res_angulo = 3 * sqrt(var_angulo);

% Histograma
figure;
histogram(angulo, 50, 'Normalization', 'pdf'); % 20 bins, normalizado como densidad
hold on;

% Superponemos la campana de Gauss teórica
x_values = linspace(min(angulo), max(angulo), 100);
gauss_fit = normpdf(x_values, media_angulo, sqrt(var_angulo));
plot(x_values, gauss_fit, 'r-', 'LineWidth', 2);

title('Histograma del ángulo medido + Ajuste Gaussiano');
xlabel('Ángulo (°)');
ylabel('Densidad de probabilidad');
legend('Histograma', 'Ajuste Gaussiano');
grid on;
hold off;