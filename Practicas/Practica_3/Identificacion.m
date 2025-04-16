close all

t = out.tout;
angulos = out.d4;


entrada = zeros(length(angulos),1);
for i=1:2:7
    entrada(i*400+1:i*400+400) = 30 * ones(400,1);
end

figure()
plot(t,entrada)
hold on
plot(t,angulos)

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

figure()
step(30*sys, 10)
hold on
plot(t(1:400), angulos(401:800), 'color', 'red')
% plot(t(1:400), angulos(1201:1600), 'color', 'red')
xlim([0 1])

