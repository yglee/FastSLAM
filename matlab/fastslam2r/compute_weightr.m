function w= compute_weight(particle, z, idf, R)

[zp,Hv,Hf,Sf]= compute_jacobians(particle, idf, R);
Pv= particle.Pv;

v= z-zp;
v(2,:)= pi_to_pi(v(2,:));

% combine jacobians into joint
N = size(z,2)*2;
vj = reshape(v,N,1);
Hvj = zeros(N,3);
Sfj = zeros(N);

for i=1:size(z,2)
    ii = 2*i + (-1:0);
    Hvj(ii,:) = Hv(:,:,i);
    Sfj(ii,ii) = Sf(:,:,i);
end

% evaluate likelihood of joint
S= Hvj*Pv*Hvj' + Sfj; % innovation covariance, includes pose and feature uncertainty
den= sqrt((2*pi)^N * det(S));
num= exp(-0.5 * vj' * inv(S) * vj);
w = num/den;
