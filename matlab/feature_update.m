function particle= feature_update(particle, z, idf, R)
%function particle= feature_update(particle, z, idf, R)
%
% Having selected a new pose from the proposal distribution, this pose is assumed
% perfect and each feature update may be computed independently and without pose uncertainty.

xf= particle.xf(:,idf);
Pf= particle.Pf(:,:,idf);

[zp,Hv,Hf,Sf]= compute_jacobians(particle, idf, R);
v= z-zp; v(2,:)= pi_to_pi(v(2,:));

for i=1:length(idf)
    vi= v(:,i);
    Hfi= Hf(:,:,i);
    Pfi= Pf(:,:,i);
    xfi= xf(:,i);

    [xf(:,i), Pf(:,:,i)]= KF_cholesky_update(xfi,Pfi, vi,R,Hfi);
end

particle.xf(:,idf)= xf;
particle.Pf(:,:,idf)= Pf;
