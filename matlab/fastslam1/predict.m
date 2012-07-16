function particle= predict(particle, V,G,Q, WB,dt, addrandom)
%

% add random noise to controls
if addrandom == 1
    VG= multivariate_gauss([V;G], Q, 1);
    V= VG(1); G= VG(2);
end

% predict state
xv= particle.xv;
particle.xv= [xv(1) + V*dt*cos(G+xv(3,:)); 
              xv(2) + V*dt*sin(G+xv(3,:));
     pi_to_pi(xv(3) + V*dt*sin(G)/WB)];
 