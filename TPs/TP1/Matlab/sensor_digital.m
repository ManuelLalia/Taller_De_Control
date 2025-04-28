close all;

distancia = out.distancia;
tiempo = out.tiempo;

media_dist = mean(distancia);
var_dist = var(distancia);

media_tiempo = mean(tiempo);
var_tiempo = var(tiempo);

res_dist = 3 * sqrt(var_dist);
res_tiempo = 3 * sqrt(var_tiempo);

% Histograma
figure;
histogram(distancia, 6, 'Normalization', 'pdf'); % 20 bins, normalizado como densidad
hold on;

% Superponemos la campana de Gauss teórica
x_values = linspace(min(distancia), max(distancia), 100);
gauss_fit = normpdf(x_values, media_dist, sqrt(var_dist));
plot(x_values, gauss_fit, 'r-', 'LineWidth', 2);

title('Histograma de distancia medida + Ajuste Gaussiano');
xlabel('Distancia (cm)');
ylabel('Densidad de probabilidad');
legend('Histograma', 'Ajuste Gaussiano');
grid on;
hold off;