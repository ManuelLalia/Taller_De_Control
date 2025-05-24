close all

t = out.tout(200:end);
angulos = out.angulo_imu(200:end);
entrada = out.angulo_servo(200:end);


figure()
plot(t,entrada)
hold on
stairs(t,angulos)


% Y = [ Y-1 Y-2 X] * alpha

Y = angulos(3:end);
Y_1 = angulos(2:end-1);
Y_2 = angulos(1:end-2);

X_1 = entrada(2:end-1);
X_2 = entrada(1:end-2);

matriz = [Y_1 Y_2 X_1 X_2];

alpha = inv(matriz.' * matriz) * matriz.' * Y;



sys = tf([alpha(3) alpha(4)],[1 -alpha(1) -alpha(2)], 0.02);

H = d2c(sys, 'tustin');

[x1, t1] = step(30*sys, 10);
[x2, t2] = step(30*H, 10);

figure()
stairs(t1, x1, 'color', 'blue', 'LineWidth', 1)
hold on
plot(t2, x2, 'color', 'black', 'LineWidth', 1.2)
stairs(t(1:200)-3.98, angulos(1201:1400), 'color', 'red', 'LineWidth', 1)
axis([0 1.5 -0.5 14])
legend('Discreto', 'Continuo', 'Datos', 'Location', 'Southeast')

