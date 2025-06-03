close all

% m = 0.028;
% m_b = 0.23;
m_b = 0.3;
g = 9.81;
k = -1/m_b;

A = [0 1 0 0; 0 k g 0; 0 0 0 1; 0 0 -144.1 -17.27];
B = [0; 0; 0; 61.8];
C = [1 0 0 0];
D = 0;

Ts = 0.02;
s = tf('s');
[b,a] = ss2tf(A,B,C,D);
H = zpk(tf(b,a));

kp = 1.8; %2
kd = kp/500;
ki = 0; %0.95

s = tf('s');
C = kp + ki/s + kd*s;
[b_c, a_c] = tfdata(C, 'v');


L = minreal(C*H);

T = feedback(L,1);
S = 1 - T;
CS = minreal(C*S);
PS = minreal(H*S);

figure()
subplot(2,1,1)
step(T)
subplot(2,1,2)
% step(PS)
hold on
impulse(T)
legend('Location', 'east')
