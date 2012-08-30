function [zp,Hv,Hf,Sf]= compute_jacobians(particle, idf, R)

xv= particle.xv;
xf= particle.xf(:,idf);
Pf= particle.Pf(:,:,idf);

for i=1:length(idf)
    dx= xf(1,i)-xv(1);
    dy= xf(2,i)-xv(2);
    d2= dx^2 + dy^2;
    d= sqrt(d2);
    
    zp(:,i)= [d; pi_to_pi(atan2(dy,dx) - xv(3))]; % predicted observation
    
    Hv(:,:,i)= [-dx/d,  -dy/d,   0; % Jacobian wrt vehicle states
                 dy/d2, -dx/d2, -1];
    Hf(:,:,i)= [ dx/d,   dy/d; % Jacobian wrt feature states
                -dy/d2,  dx/d2];
    Sf(:,:,i)= Hf(:,:,i) * Pf(:,:,i) * Hf(:,:,i)' + R; % innovation covariance of 'feature observation given the vehicle'
end
