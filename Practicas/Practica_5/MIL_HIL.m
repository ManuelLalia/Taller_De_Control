close all;

optionss=bodeoptions;
% optionss.MagVisible='off';
optionss.PhaseMatching='on';
optionss.PhaseMatchingValue=-180;
optionss.PhaseMatchingFreq=1;
optionss.Grid='on';


s = tf('s');

P = -0.004233/(s + 0.002397);

% k = -db2mag(-5);
% C = zpk([-0.01],[0, -0.05],k);
k = -db2mag(17);
C = zpk([-0.01],[0],k);



L = minreal(C*P);

figure();
bode(L, optionss)


T = feedback(L,1);
S = 1 - T;
CS = minreal(C*S);
PS = minreal(P*S);

figure()
subplot(2,2,1)
step(T)
title('Salida')

subplot(2,2,2)
step(PS)
title('PS')

subplot(2,2,3)
step(S)
title('Error')

subplot(2,2,4)
step(CS)
title('Control')

Cd =c2d(C, 1, 'tustin');

