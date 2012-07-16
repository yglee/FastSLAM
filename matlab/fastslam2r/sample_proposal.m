function particle= sample_proposal(particle, z,idf, R, sampleproposal)
% Compute proposal distribution and then sample from it

xv= particle.xv;
Pv= particle.Pv;

% process each feature, incrementally refine proposal distribution
for i=1:length(idf)
    j= idf(i);
    
    [zpi,Hvi,Hfi,Qfi]= compute_jacobians(particle, j, R);
    Qfi= inv(Qfi); % only need inverse below
    vi= z(:,i)-zpi; vi(2)= pi_to_pi(vi(2));

    Pv= inv(Hvi' * Qfi * Hvi + inv(Pv)); % proposal covariance
    xv= xv + Pv * Hvi' * Qfi * vi; % proposal mean
    
    particle.xv= xv;
    particle.Pv= Pv;
end
    
% sample from proposal distribution
if sampleproposal==1, particle.xv= multivariate_gauss(xv,Pv,1); end
particle.Pv= zeros(3);
