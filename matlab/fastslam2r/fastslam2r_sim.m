function data= fastslam2_sim(lm, wp)
%function data= fastslam2_sim(lm, wp)
%
% INPUTS: 
%   lm - set of landmarks
%   wp - set of waypoints
%
% OUTPUTS:
%   data - set of particles representing final state
%
% NOTES:
%   This program is a FastSLAM 2.0 simulator. To use, create a set of landmarks and 
%   vehicle waypoints (ie, waypoints for the desired vehicle path). The program
%   'frontend.m' may be used to create this simulated environment - type
%   'help frontend' for more information.
%       The configuration of the simulator is managed by the script file
%   'configfile.m'. To alter the parameters of the vehicle, sensors, etc
%   adjust this file. There are also several switches that control certain
%   filter options.
%
% Tim Bailey and Juan Nieto 2004.
% Version 1.0

format compact
path(path, '../')
configfile;

if SWITCH_PREDICT_NOISE, warning('Sampling from predict noise usually OFF for FastSLAM 2.0'), end
if SWITCH_SAMPLE_PROPOSAL==0, warning('Sampling from optimal proposal is usually ON for FastSLAM 2.0'), end

h= setup_animations(lm,wp);
veh= [0 -WHEELBASE -WHEELBASE; 0 -1 1];

% initialisations
particles= initialise_particles(NPARTICLES);
xtrue= zeros(3,1);

dt= DT_CONTROLS; % change in time between predicts
dtsum= 0; % change in time since last observation
ftag= 1:size(lm,2); % identifier for each landmark
da_table= zeros(1,size(lm,2)); % data association table 
iwp= 1; % index to first waypoint 
G= 0; % initial steer angle
plines=[];
if SWITCH_SEED_RANDOM ~= 0, rand('state',SWITCH_SEED_RANDOM), randn('state',SWITCH_SEED_RANDOM), end

Qe= Q; Re= R;
if SWITCH_INFLATE_NOISE==1, Qe= 2*Q; Re= 2*R; end

if SWITCH_PROFILE, profile on -detail builtin, end

% main loop 
while iwp ~= 0 
    
    % compute true data
    [G,iwp]= compute_steering(xtrue, wp, iwp, AT_WAYPOINT, G, RATEG, MAXG, dt);
    if iwp==0 & NUMBER_LOOPS > 1, iwp=1; NUMBER_LOOPS= NUMBER_LOOPS-1; end
    xtrue= predict_true(xtrue, V,G, WHEELBASE,dt);
    
    % add process noise
    [Vn,Gn]= add_control_noise(V,G,Q, SWITCH_CONTROL_NOISE);
    
    % Predict step
    for i=1:NPARTICLES
        particles(i)= predict (particles(i), Vn,Gn,Qe, WHEELBASE,dt, SWITCH_PREDICT_NOISE);
        particles(i)= observe_heading(particles(i), xtrue(3), SWITCH_HEADING_KNOWN); % if heading known, observe heading
    end
    
    % Observe step
    dtsum= dtsum + dt;
    if dtsum >= DT_OBSERVE
        dtsum= 0;
        
        % Compute true data, then add noise
        [z,ftag_visible]= get_observations(xtrue, lm, ftag, MAX_RANGE);
        z= add_observation_noise(z,R, SWITCH_SENSOR_NOISE);
        if ~isempty(z), plines= make_laser_lines (z,xtrue); end
        
        % Compute (known) data associations
        Nf= size(particles(1).xf,2);
        [zf,idf,zn,da_table]= data_associate_known(z, ftag_visible, da_table, Nf);
        
        % Observe map features
        if ~isempty(zf) 
            
            % compute weights w = w * p(z_k | x_k-1)
            for i=1:NPARTICLES
                w= compute_weightr(particles(i), zf,idf, Re);
                particles(i).w= particles(i).w * w;
            end
            
            % resampling *before* computing proposal permits better particle diversity
            particles= resample_particles(particles, NEFFECTIVE, SWITCH_RESAMPLE);            
            
            % sample from "optimal" proposal distribution, then update map 
            for i=1:NPARTICLES
                particles(i)= sample_proposal(particles(i), zf,idf, Re, SWITCH_SAMPLE_PROPOSAL); 
                particles(i)= feature_update(particles(i), zf, idf, Re);
            end
        end
        
        % Observe new features, augment map
        if ~isempty(zn)
            for i=1:NPARTICLES
                if isempty(zf) % sample from proposal distribution (if we have not already done so above)
                    particles(i).xv= multivariate_gauss(particles(i).xv, particles(i).Pv, 1);
                    particles(i).Pv= zeros(3);
                end                        
                particles(i)= add_feature(particles(i), zn,Re);
            end
        end
        
    end
    
    % plots
    do_plot(h, particles, xtrue, plines, veh)
end

if SWITCH_PROFILE, profile report, end

data= particles;

% 
%

function p= make_laser_lines (rb,xv)
if isempty(rb), p=[]; return, end
len= size(rb,2);
lnes(1,:)= zeros(1,len)+ xv(1);
lnes(2,:)= zeros(1,len)+ xv(2);
lnes(3:4,:)= transformtoglobal([rb(1,:).*cos(rb(2,:)); rb(1,:).*sin(rb(2,:))], xv);
p= line_plot_conversion (lnes);

function p= initialise_particles(np)
for i=1:np
    p(i).w= 1/np;
    p(i).xv= [0;0;0];
    p(i).Pv= zeros(3);
    p(i).xf= [];
    p(i).Pf= [];
    p(i).da= [];
end

function p= make_covariance_ellipses(particle)
N= 10;
inc= 2*pi/N;
phi= 0:inc:2*pi;
circ= 2*[cos(phi); sin(phi)];

p= make_ellipse(particle.xv(1:2), particle.Pv(1:2,1:2) + eye(2)*eps, circ);

lenf= size(particle.xf,2);
if lenf > 0
    
    xf= particle.xf;
    Pf= particle.Pf;
    p= [p zeros(2, lenf*(N+2))];

    ctr= N+3;
    for i=1:lenf
        ii= ctr:(ctr+N+1);
        p(:,ii)= make_ellipse(xf(:,i), Pf(:,:,i), circ);
        ctr= ctr+N+2;
    end
end

function p= make_ellipse(x,P,circ)
% make a single 2-D ellipse 
r= sqrtm_2by2(P);
a= r*circ;
p(2,:)= [a(2,:)+x(2) NaN];
p(1,:)= [a(1,:)+x(1) NaN];

%
%

function h= setup_animations(lm,wp)
figure
plot(lm(1,:),lm(2,:),'g*')
hold on, axis equal
plot(wp(1,:),wp(2,:), wp(1,:),wp(2,:),'ro')

h.xt= patch(0,0,'g','erasemode','xor'); % vehicle true
h.xm= patch(0,0,'r','erasemode','xor'); % mean vehicle estimate
h.obs= plot(0,0,'y','erasemode','xor'); % observations
h.xfp= plot(0,0,'r.','erasemode','background'); % estimated features (particle means)
h.xvp= plot(0,0,'r.','erasemode','xor'); % estimated vehicle (particles)
h.cov= plot(0,0,'erasemode','xor'); % covariances of max weight particle


function do_plot(h, particles, xtrue, plines, veh)

xvp = [particles.xv];
xfp = [particles.xf];
w = [particles.w]; 

ii= find(w== max(w)); 
xvmax= xvp(:,ii);

xt= transformtoglobal(veh,xtrue);
xm= transformtoglobal(veh,xvmax);
set(h.xt, 'xdata', xt(1,:), 'ydata', xt(2,:))
set(h.xm, 'xdata', xm(1,:), 'ydata', xm(2,:))
set(h.xvp, 'xdata', xvp(1,:), 'ydata', xvp(2,:))
if ~isempty(xfp), set(h.xfp, 'xdata', xfp(1,:), 'ydata', xfp(2,:)), end
if ~isempty(plines), set(h.obs, 'xdata', plines(1,:), 'ydata', plines(2,:)), end
pcov= make_covariance_ellipses(particles(ii(1)));
if ~isempty(pcov), set(h.cov, 'xdata', pcov(1,:), 'ydata', pcov(2,:)); end

drawnow
