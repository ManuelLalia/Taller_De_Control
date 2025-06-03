close all

% distancia = out.distancia / 100; % Paso de cm a m
d_max = 0.332; % Con el carro en la punta (lo saco de los primeros 20 segs de mediciones)

% Ploteo todo para sacar los intervalos de muestras de las mediciones
% figure(); hold on
% stairs(distancia)
% yline(d_max)
%%

load('med5.mat')
distancia = out.distancia / 100; % Paso de cm a m
% Estas son para med1
d1 = distancia(1258:1321);
d2 = distancia(2101:2168);
d3 = distancia(3067:3136);
d4 = distancia(3540:3619);
d5 = distancia(4025:4095);
d6 = distancia(4523:4596);
d7 = distancia(5239:5303);
d8 = distancia(5806:5869);
d9 = distancia(6371:6438);
d10 = distancia(7085:7161);
d11 = distancia(7680:7751);
d12 = distancia(8224:8291);

% load('med3.mat')
% distancia = out.distancia / 100; % Paso de cm a m
% % Estas son para med1
% d1 = distancia(1423:1466);
% d2 = distancia(2212:2256);
% d3 = distancia(2824:2873);
% d4 = distancia(3753:3794);
% d5 = distancia(4858:4903);
% d6 = distancia(5432:5477);

% load('med1.mat')
% distancia = out.distancia / 100; % Paso de cm a m
% % Estas son para med1
% d1 = distancia(1576:1636);
% d2 = distancia(2312:2375);
% d3 = distancia(3097:3159);
% d4 = distancia(4038:4090);
% d5 = distancia(4747:4803);
% d6 = distancia(6870:6921);
% 
% load('med2.mat')
% distancia = out.distancia / 100; % Paso de cm a m
% % Estas son para med2
% d7 = distancia(1456:1509);
% d8 = distancia(3587:3653);
% d9 = distancia(4428:4486);
% d10 = distancia(4962:5021);
% d11 = distancia(6389:6437); % Me salte una xq estaba fea
% d12 = distancia(7004:7070);

% exp = {d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12};

% Saco algunos que estan bastante feos
% exp = {d4, d5, d6, d7, d9, d10};

exp = {d1, d3, d5, d9, d11, d12};

angulo = deg2rad(mean(out.angulo(500:1000)));
k = double(9.81 * sin(angulo));

ft = fittype('a^2 * k * (exp(-t/a) - 1) + a * k * t + c', 'problem', {'k', 'c'}, 'independent', {'t'});

t = cell(1,size(exp,2));
f = cell(1,size(exp,2));
m_b = cell(1,size(exp,2));

for i = 1:size(exp,2)
    t{i} = linspace(0, length(exp{i})*0.02, length(exp{i}));
    c = double( exp{i}(1) );
    fi = fit(t{i}', exp{i}, ft, 'problem', {k, c}, 'StartPoint', 0.2, 'Lower', 0.005, 'Upper', 1);
    f{i} = fi;
    m_b{i} = fi.a;
end


figure();
for i=1:size(exp,2)
   subplot(2,3,i);hold on
%    subplot(3,4,i);hold on
   plot(f{i}, t{i}, exp{i})
   axis([0 1.4 0 0.35])
   grid()
   legend('Datos', 'Modelo exp', 'Location', 'NorthWest')
   xlabel('t [s]')
   ylabel('x [m]')
   title(['$\frac{m}{b}$ = ', num2str(f{i}.a)], 'Interpreter', 'latex')
end
% titulo = sprintf(
% suptitle(['$\frac{m}{b}$ promedio = ', num2str(mean([m_b{:}]))], 'Interpreter', 'latex')
% suptitle(['$\frac{m}{b}$ promedio = ', num2str(mean([m_b{:}])), '$'], 'Interpreter', 'latex')
sgtitle(['$\frac{m}{b}$ promedio = ', num2str(mean([m_b{:}]))], 'Interpreter', 'latex');



