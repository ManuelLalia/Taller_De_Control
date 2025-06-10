close all

Ts = 0.02;

A = eye(2) + [0 1; -144.1 -17.27]*Ts;
B = [0; 61.8]*Ts;
C = [1 0];
D = 0;

K = place(A,-B,[0.8, 0.5]);
F = inv(C * inv(eye(2) - (A + B*K)) * B);

K
F

