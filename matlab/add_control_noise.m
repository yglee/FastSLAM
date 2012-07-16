function [V,G]= add_control_noise(V,G,Q, addnoise)
% Add random noise to nominal control values

if addnoise == 1
%     V= V + randn(1)*sqrt(Q(1,1)); % if assume Q is diagonal
%     G= G + randn(1)*sqrt(Q(2,2));
    
     C= multivariate_gauss([V;G],Q, 1); % if Q might be correlated
     V= C(1); G= C(2);
end
