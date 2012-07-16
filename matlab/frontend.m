function varargout = frontend(varargin)
%EKF-SLAM environment-making GUI
%
% This program permits the graphical creation and manipulation 
% of an environment of point landmarks, and the specification of
% vehicle path waypoints therein.
%
% USAGE: type 'frontend' to start.
%   1. Click on the desired operation: <enter>, <move>, or <delete>.
%   2. Click on the type: <waypoint> or <landmark> to commence the 
%   operation.
%   3. If entering new landmarks or waypoints, click with the left
%   mouse button to add new points. Click the right mouse button, or
%   hit <enter> key to finish.
%   4. To move or delete a point, just click near the desired point.
%   5. Saving maps and loading previous maps is accomplished via the
%   <save> and <load> buttons, respectively.
%
% Tim Bailey and Juan Nieto 2004.

% FRONTEND Application M-file for frontend.fig
%    FIG = FRONTEND launch frontend GUI.
%    FRONTEND('callback_name', ...) invoke the named callback.
global WAYPOINTS LANDMARKS FH

if nargin == 0  % LAUNCH GUI

    %initialisation
    WAYPOINTS= [0;0];
    LANDMARKS= [];
    
    % open figure
	fig = openfig(mfilename,'reuse');
    hh= get(fig, 'children');
    set(hh(3), 'value', 1)
    
    hold on
    FH.hl= plot(0,0,'g*'); plot(0,0,'w*')
    FH.hw= plot(0,0,0,0,'ro');
    plotwaypoints(WAYPOINTS);

	% Use system color scheme for figure:
	set(fig,'Color',get(0,'defaultUicontrolBackgroundColor'));
    set(fig,'name', 'SLAM Map-Making GUI')

	% Generate a structure of handles to pass to callbacks, and store it. 
	handles = guihandles(fig);
	guidata(fig, handles);

	if nargout > 0
		varargout{1} = fig;
	end

elseif ischar(varargin{1}) % INVOKE NAMED SUBFUNCTION OR CALLBACK

	try
		[varargout{1:nargout}] = feval(varargin{:}); % FEVAL switchyard
	catch
		disp(lasterr);
	end

end


% --------------------------------------------------------------------
function varargout = waypoint_checkbox_Callback(h, eventdata, handles, varargin)
global WAYPOINTS
set(handles.landmark_checkbox, 'value', 0)
WAYPOINTS= perform_task(WAYPOINTS, handles.waypoint_checkbox, handles);
plotwaypoints(WAYPOINTS);

% --------------------------------------------------------------------
function varargout = landmark_checkbox_Callback(h, eventdata, handles, varargin)
global LANDMARKS
set(handles.waypoint_checkbox, 'value', 0)
LANDMARKS= perform_task(LANDMARKS, handles.landmark_checkbox, handles);
plotlandmarks(LANDMARKS);

% --------------------------------------------------------------------
function varargout = enter_checkbox_Callback(h, eventdata, handles, varargin)
set(handles.enter_checkbox, 'value', 1)
set(handles.move_checkbox, 'value', 0)
set(handles.delete_checkbox, 'value', 0)

% --------------------------------------------------------------------
function varargout = move_checkbox_Callback(h, eventdata, handles, varargin)
set(handles.enter_checkbox, 'value', 0)
set(handles.move_checkbox, 'value', 1)
set(handles.delete_checkbox, 'value', 0)

% --------------------------------------------------------------------
function varargout = delete_checkbox_Callback(h, eventdata, handles, varargin)
set(handles.enter_checkbox, 'value', 0)
set(handles.move_checkbox, 'value', 0)
set(handles.delete_checkbox, 'value', 1)

% --------------------------------------------------------------------
function varargout = load_button_Callback(h, eventdata, handles, varargin)
global WAYPOINTS LANDMARKS
seed = {'*.mat','MAT-files (*.mat)'};
[fn,pn] = uigetfile(seed, 'Load landmarks and waypoints');
if fn==0, return, end

fnpn = strrep(fullfile(pn,fn), '''', '''''');
load(fnpn)
WAYPOINTS= wp; LANDMARKS= lm;
plotwaypoints(WAYPOINTS);
plotlandmarks(LANDMARKS);

% --------------------------------------------------------------------
function varargout = save_button_Callback(h, eventdata, handles, varargin)
global WAYPOINTS LANDMARKS
wp= WAYPOINTS; lm= LANDMARKS;
seed = {'*.mat','MAT-files (*.mat)'};
[fn,pn] = uiputfile(seed, 'Save landmarks and waypoints');
if fn==0, return, end

fnpn = strrep(fullfile(pn,fn), '''', '''''');
save(fnpn, 'wp', 'lm');

% --------------------------------------------------------------------
function plotwaypoints(x)
global FH
set(FH.hw(1), 'xdata', x(1,:), 'ydata', x(2,:))
set(FH.hw(2), 'xdata', x(1,:), 'ydata', x(2,:))

% --------------------------------------------------------------------
function plotlandmarks(x)
global FH
set(FH.hl, 'xdata', x(1,:), 'ydata', x(2,:))

% --------------------------------------------------------------------
function i= find_nearest(x)
xp= ginput(1);
d2= (x(1,:)-xp(1)).^2 + (x(2,:)-xp(2)).^2;
i= find(d2 == min(d2));
i= i(1);

% --------------------------------------------------------------------
function x= perform_task(x, h, handles)
        
if get(h, 'value') == 1
    zoom off
    
    if get(handles.enter_checkbox, 'value') == 1 % enter points
        [xn,yn,bn]= ginput(1);
        while ~isempty(xn) & bn == 1
            x= [x [xn;yn]];
            if h == handles.waypoint_checkbox
                plotwaypoints(x); 
            else
                plotlandmarks(x);
            end
            [xn,yn,bn]= ginput(1);
        end                
    else
        i= find_nearest(x);        
        if get(handles.delete_checkbox, 'value') == 1 % delete nearest point
            x= [x(:,1:i-1) x(:,i+1:end)];
            
        elseif get(handles.move_checkbox, 'value') == 1 % move nearest point
            xt= x(:,i);
            plot(xt(1), xt(2),'kx', 'markersize',10)
            x(:,i)= ginput(1)';
            plot(xt(1), xt(2),'wx', 'markersize',10)
        end            
    end
    
    set(h, 'value', 0)
end
