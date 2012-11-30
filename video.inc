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
/trymodes {
  1366 768 32 trymode not {
    1366 768 16 trymode not {
      1280 800 32 trymode not {
        1280 800 16 trymode not {
          1024 768 32 trymode not {
            1024 768 16 trymode not {
              800 600 32 trymode not {
                800 600 16 trymode not {
                  false .end
                } if
              } if
            } if
          } if
        } if
      } if
    } if
  } if
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
%% paint_splash - paints a splash image that fits the current mode
%
% ( -- )   
%
/paint_splash {
  screen.size pop
  dup 1366 eq {
    pop
    "splash_1366x768.jpg"
  } {
    dup 1280 eq {
      pop
      "splash_1280x800.jpg"
    } {
      1024 eq {
        "splash_1024x768.jpg"
      } {
        "splash_800x600.jpg"
      } ifelse
    } ifelse
  } ifelse
  findfile setimage
  0 0 moveto
  0 0 image.size image
} def

