close all;

distancia = out.distancia;
tiempo = out.tiempo;

media_dist = mean(distancia);
var_dist = var(distancia);

media_tiempo = mean(tiempo);
var_tiempo = var(tiempo);

res_dist = 3 * sqrt(var_dist);
res_tiempo = 3 * sqrt(var_tiempo);