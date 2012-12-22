/timeout.firstrun true def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Boot timeout counter.
%
% ( timeout time -- )
%
% timeout: total timeout time in 18.2Hz steps
% time: current timeout time
%
% Note: if the user hits a key to stop the timeout this function is called
% with time = 0.
%
/Timeout {
  % we are not interested in the total timeout time
  exch pop

  % check time value  
  dup 0 eq {
    % timeout cancelled
    % erase old value from screen
    erase.timeout
    % reset font to menu font
    menu.font find_cached_file setfont
    return
  } if

  % convert timeout to seconds
  /timeout.s.last timeout.s def
  /timeout.s exch 10 mul 150 add 182 div def

  % determine or set some properties only at the first run and store them
  % for later use to speed up things a little bit for every round
  timeout.firstrun {
    timeout.color setcolor
    timeout.font find_cached_file setfont
    set.timeout.string
    timeout.string strsize
    /timeout.height exch def
    /timeout.width exch def
    /timeout.firstrun false def
  } if

  timeout.s timeout.s.last ne {
    erase.timeout
    timeout.s 0 ne {
      set.timeout.string
      timeout.string show
    } if
  } if
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% erases the timeout string from screen
%
% ( -- )
%
/erase.timeout {
    timeout.x timeout.y moveto
    currentpoint timeout.width timeout.height image
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% sets timeout.string based on the current value of timeout.s
%
% ( -- )
%
/set.timeout.string {
  timeout.s 1 eq {
    timeout.s timeout.format_singular timeout.string sprintf
  } {
    timeout.s timeout.format_plural timeout.string sprintf
  } ifelse
} def

