function angle = pi_to_pi(angle)

%function angle = pi_to_pi(angle)
% Input: array of angles.
% Tim Bailey 2000

%angle = mod(angle, 2*pi); 
% mod() not necessary as angles are (nearly) always bounded by -2*pi <= angle <= 2*pi
% and, also, the mod() function is very slow. [Tim Bailey 2004]
i= find(angle<-2*pi | angle>2*pi); % replace with a check
if ~isempty(i) 
%    warning('pi_to_pi() error: angle outside 2-PI bounds.')
    angle(i) = mod(angle(i), 2*pi);
end

i= find(angle>pi);
angle(i)= angle(i)-2*pi;

i= find(angle<-pi);
angle(i)= angle(i)+2*pi;
