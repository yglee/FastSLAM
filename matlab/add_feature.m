function particle= add_feature(particle, z,R)
% add new features 

lenz= size(z,2);
xf= zeros(2,lenz);
Pf= zeros(2,2,lenz);
xv= particle.xv;

for i=1:lenz
    r= z(1,i); b= z(2,i);
    s= sin(xv(3)+b); 
    c= cos(xv(3)+b);
    
    xf(:,i)= [xv(1) + r*c;  
              xv(2) + r*s];
    
    Gz= [c -r*s; 
         s  r*c];
    Pf(:,:,i)= Gz*R*Gz';
end

lenx= size(particle.xf,2);
ii= (lenx+1):(lenx+lenz);
particle.xf(:,ii)= xf;
particle.Pf(:,:,ii)= Pf;
