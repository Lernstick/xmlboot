/timeout.firstrun true def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Boot timeout counter.
%
% ( timeout time -- )
%
% timeout: total time in 18.2Hz steps
% time: current value
%
% Note: if the user hits a key to stop the timeout this function is called
% with time = 0.
%
/Timeout {
  % convert timeout to seconds
  /timeout.s.last timeout.s def
  /timeout.s exch 10 mul 150 add 182 div def
  % we don't need the time value
  pop

  % determine or set some properties only at the first run and store them
  % for later use to speed up things a little bit for every round
  timeout.firstrun {
    timeout.color setcolor
    currentfont free
    timeout.font findfile setfont
    timeout.s timeout.format_plural timeout.string sprintf
    timeout.string strsize
    /timeout.height exch def
    /timeout.width exch def
    /timeout.firstrun false def
  } if

  timeout.s timeout.s.last ne {
    % erase old value from screen
    timeout.x timeout.y moveto
    currentpoint timeout.width timeout.height image

    timeout.s 0 ne {
      % format new value
      timeout.s 1 eq {
        timeout.s timeout.format_singular timeout.string sprintf
      } {
        timeout.s timeout.format_plural timeout.string sprintf
      } ifelse

      % paint new value
      timeout.string show
    } if
  } if
} def
