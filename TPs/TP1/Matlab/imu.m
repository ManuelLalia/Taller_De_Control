close all;

tiempo = out.tout(50:end, 1);
angulo = out.angulo_imu(50:end, 1); % a partir de 1 seg

media_angulo = mean(angulo);
var_angulo = var(angulo);

res_angulo = 3 * sqrt(var_angulo);