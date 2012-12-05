% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Boot timeout counter.
%
% ( timeout time -- )
%
% timeout: total time in 18.2Hz steps, time: current value.
%
% Note: if the user hits a key to stop the timeout this function is called
% with time = 0.
%
/Timeout {
  /timeout.s.last timeout.s def
  /timeout.s exch 10 mul 150 add 182 div def

  pop   % we don't need it

  timeout.s timeout.s.last ne {
    % erase old value from screen
    timeout.x timeout.y moveto currentpoint timeout.string strsize image

    timeout.s 0 ne {
      % format new value
      timeout.s 1 eq {
        timeout.s timeout.format_singular timeout.string sprintf
      } {
        timeout.s timeout.format_plural timeout.string sprintf
      } ifelse

      % paint new value
      timeout.color setcolor
      timeout.fontfile setfont
      timeout.string show
    } if
  } if
} def
