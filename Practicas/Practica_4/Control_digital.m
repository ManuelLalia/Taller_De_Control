close all

optionss=bodeoptions;
% optionss.MagVisible='off';
optionss.PhaseMatching='on';
optionss.PhaseMatchingValue=-180;
optionss.PhaseMatchingFreq=1;
optionss.Grid='on';

[z, p, k] = zpkdata(H);
H_control = zpk([], p, 73.38);


s = tf('s');
% K = db2mag(23.6);
% C = zpk([],[0],K);
K = db2mag(46);
C = zpk([],[0, -20],K);

L = minreal(C*H_control);


T = feedback(L,1);
S = 1 - T;
CS = minreal(C*S);
PS = minreal(H_control*S);


figure()
bode(H_control, L, optionss)
legend()

figure()
subplot(1,2,1)
step(T)
subplot(1,2,2)
step(PS)
hold on
step(CS)
legend('Location', 'east')


% Bilineal
Cd_bil = c2d(C, 0.02, 'tustin');
Cd_fw  = c2d(C, 0.02, 'zoh');


function a = transf2eq(sys)
    [z, p, k] = zpkdata(Cd_bil);
    disp(z,p,k)
    
end



