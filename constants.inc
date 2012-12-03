% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Some constants we need.
%
/true 0 0 eq def
/false 0 0 ne def

% some key codes
/keyEsc       0x0000001b def
/keyEnter     0x0000000d def
/keyUp        0x48000000 def
/keyLeft      0x4b000000 def
/keyRight     0x4d000000 def
/keyDown      0x50000000 def

% type values
/t_none  0 def
/t_int   1 def
/t_str   4 def
/t_arr  10 def
/t_end  11 def
/t_ptr  12 def

% undefined (nil) value
/.undef 0 t_none settype def

% end token, stops execution
/.end 0 t_end settype def
