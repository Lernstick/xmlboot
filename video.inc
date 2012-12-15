% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
%% trymode - try setting a video mode
%
% ( int1 int2 int3 -- bool )   
%
% int1, int2: width, height
% int3: color bits
% bool: true, if video mode was set successfully, false otherwise
%
% example
%   1024 768 16 trymode             % 1024x768, 16-bit color mode
%
/trymode {
  0 1 videomodes {
    videomodeinfo dup .undef eq {
      pop pop pop pop
    } {
      % compare width, height, colors
      6 index 4 index eq 6 index 4 index eq and 5 index 3 index eq and {
        7 1 roll 6 { pop } repeat 0xbfff and setmode return
      } {
        pop pop pop pop
      } ifelse
    } ifelse
  } for

  pop pop pop false
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
%% trymodes - try setting a good video mode
%
% ( -- )
%
/try_video_modes {
  xml_structure "videomodes" get_entity_child
  get_entity_children {
    dup gettype t_arr eq {
      dup get_entity_name "videomode" eq {
        dup "width" get_attribute_value string_to_int
        exch dup "height" get_attribute_value string_to_int
        exch dup "color_depth" get_attribute_value string_to_int
        4 2 roll rot
        trymode {
          % setting this video mode succeeded
          % set the background image
          % but do NOT *paint* the background image right now
          % the rest of the setup will still take a while and it looks somewhat broken
          % when showing just the background image alone for several seconds...
          "background" get_attribute_value
          findfile setimage
          % exit forall loop
          exit
        } {
          % setting this video mode failed
          % remove video mode entity from stack
          pop
        } ifelse
      } {
        % entity not (yet) supported
        pop
      } ifelse
    } {
      % child entry was just some text
      pop
    } ifelse
  } forall
} def

