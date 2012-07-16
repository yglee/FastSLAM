function xv= predict_true(xv, V,G, WB,dt)
%
% INPUTS:

xv= [xv(1) + V*dt*cos(G+xv(3,:)); 
     xv(2) + V*dt*sin(G+xv(3,:));
     pi_to_pi(xv(3) + V*dt*sin(G)/WB)];
 