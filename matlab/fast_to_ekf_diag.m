function [x,P]= fast_to_ekf_diag(p)
%function [x,P]= fast_to_ekf_diag(particles)
%
% particles(i) has xv, Pv, {xf, Pf}
% For this code to work, the particles must be of equal weight.
% Also assume all particles possess the same features (in the
% same order).

N= length(p); 
lenf= size(p(1).xf, 2);
lenx= 3 + 2*lenf;
x= zeros(lenx, 1);
P= zeros(lenx, lenx);

% vehicle mean
ii= 1:3;
x(ii)= mean([p.xv],2);

% vehicle covariance
Pvv= zeros(3);
for i=1:N
    xv= p(i).xv;
    Pvv= Pvv + xv * xv'; 
end
P(ii,ii)= Pvv/N - x(ii) * x(ii)';

% feature-related parts
for i=1:lenf
    xfs= [0;0];
    Pvf= zeros(3,2);
    Pff= zeros(2);
    for k=1:N
        xv= p(k).xv;
        xf= p(k).xf(:,i);
        xfs= xfs + xf;
        Pvf= Pvf + xv * xf';
        Pff= Pff + xf * xf' + p(k).Pf(:,:,i);
    end

    % feature mean
    jj= 2*i + 2; 
    jj= jj:(jj+1);
    x(jj)= xfs/N;

    % feature covariance
    P(jj,jj)= Pff/N - x(jj) * x(jj)';
    
    % vehicle-feature correlations
    P(ii,jj)= Pvf/N - x(ii) * x(jj)';
    P(jj,ii)= P(ii,jj)';
end
