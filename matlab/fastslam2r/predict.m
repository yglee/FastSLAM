function particle= predict(particle, V,G,Q, WB,dt, addrandom)
%
% INPUTS:
%   xv - vehicle pose sample
%   Pv - vehicle pose predict covariance
%
% Note: Pv must be zeroed after each observation. It accumulates the
% vehicle pose uncertainty between measurements.

xv= particle.xv;
Pv= particle.Pv;

% Jacobians     
phi= xv(3);
Gv= [1 0 -V*dt*sin(G+phi);
     0 1  V*dt*cos(G+phi);
     0 0 1];
Gu= [dt*cos(G+phi) -V*dt*sin(G+phi);
     dt*sin(G+phi)  V*dt*cos(G+phi);
     dt*sin(G)/WB   V*dt*cos(G)/WB];

% predict covariance
particle.Pv= Gv*Pv*Gv' + Gu*Q*Gu';

% optional: add random noise to predicted state
if addrandom == 1
    VG= multivariate_gauss([V;G], Q, 1);
    V= VG(1); G= VG(2);
end

% predict state
particle.xv= [xv(1) + V*dt*cos(G+xv(3,:)); 
              xv(2) + V*dt*sin(G+xv(3,:));
     pi_to_pi(xv(3) + V*dt*sin(G)/WB)];

%
%

function x= pi_to_pi(x)
if x > pi
    x= x - 2*pi;
elseif x < -pi
    x= x + 2*pi;
end
