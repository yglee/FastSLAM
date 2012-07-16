function particle= sample_proposal(particle, z,idf, R)
% Compute proposal distribution, then sample from it, and compute new
% particle weight.

xv= particle.xv;
Pv= particle.Pv;
xv0= xv;
Pv0= Pv;

% process each feature, incrementally refine proposal distribution
for i=1:length(idf)
    j= idf(i);
    
    [zpi,Hvi,Hfi,Sf]= compute_jacobians(particle, j, R);
    Sfi= inv(Sf); 
    vi= z(:,i)-zpi; 
    vi(2)= pi_to_pi(vi(2));

    Pv= inv(Hvi' * Sfi * Hvi + inv(Pv)); % proposal covariance
    xv= xv + Pv * Hvi' * Sfi * vi; % proposal mean

    particle.xv= xv;
    particle.Pv= Pv;
end

% sample from proposal distribution
xvs= multivariate_gauss(xv,Pv,1); 
particle.xv= xvs;
particle.Pv= zeros(3);

% compute sample weight: w = w * p(z|xk) p(xk|xk-1) / proposal
like = likelihood_given_xv(particle, z,idf, R);
prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0);
prop =  gauss_evaluate(delta_xv(xv, xvs), Pv);
particle.w = particle.w * like * prior / prop;

%
%

function w = likelihood_given_xv(particle, z,idf, R)
% For FastSLAM, p(z|xv) requires the map part to be marginalised from p(z|xv,m)
w = 1;
for i=1:length(idf)
    [zp,Hv,Hf,Sf]= compute_jacobians(particle, idf(i), R);
    v= z(:,i)-zp; 
    v(2)= pi_to_pi(v(2));

    w = w * gauss_evaluate(v,Sf);
end

%
%

function dx = delta_xv(xv1, xv2)
% Compute innovation between two xv estimates, normalising the heading component
dx = xv1 - xv2;
dx(3,:) = pi_to_pi(dx(3,:));
