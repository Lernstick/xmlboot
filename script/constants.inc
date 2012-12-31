% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Some constants we need.
%

% boolean values
/true 0 0 eq def
/false 0 0 ne def

% some ASCII codes
/asciiEsc   0x1B def
/asciiEnter 0x0D def

% some scan codes
/scanUp    0x48 def
/scanLeft  0x4b def
/scanRight 0x4d def
/scanDown  0x50 def

% some status codes
/statusAlt   0x0208 def
/statusAltL  0x0200 def
/statusAltR  0x0008 def
/statusCtrlL 0x0104 def
/statusCtrlR 0x0004 def
/statusShift 0x0003 def

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
