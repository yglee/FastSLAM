function particles= resample_particles(particles, Nmin, doresample)
%function particles= resample_particles(particles, Nmin, doresample)
%
% Resample particles if their weight variance is such that N-effective
% is less than Nmin.
%

N= length(particles);
w= zeros(1,N);
for i=1:N, w(i)= particles(i).w; end
ws= sum(w); w= w/ws;
for i=1:N, particles(i).w= particles(i).w / ws; end

[keep, Neff] = stratified_resample(w);
if Neff < Nmin & doresample==1
    particles= particles(keep);
    for i=1:N, particles(i).w= 1/N; end
end
