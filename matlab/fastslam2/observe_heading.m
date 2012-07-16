function particle= observe_heading(particle, phi, useheading)

if useheading==0, return, end
sigmaPhi= 0.01*pi/180; % radians, heading uncertainty

xv= particle.xv;
Pv= particle.Pv;

H= [0 0 1];
v= pi_to_pi(phi - xv(3));

[xv, Pv]= KF_joseph_update(xv, Pv, v, sigmaPhi^2,H);
particle.xv= xv;
particle.Pv= Pv;
