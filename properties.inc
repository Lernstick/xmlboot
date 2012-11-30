% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% reads the lines of a file (separated with newline '\n' == 0x0A == 10
%
% ( ptr -- array )
%
% ptr: file pointer
% array: the lines of the file
%
/read_lines {
  /file_pointer exch def
  /start_index 0 def
  /stop_index 0 def
  [
    file_pointer
    {
      10 eq {
        % determine line length
        /line_length stop_index start_index sub def

        % allocate memory for line
        /line line_length 1 add malloc def

        % copy line
        line file_pointer start_index add line_length memcpy

        % leave line as string on stack for array
        line t_str settype

        % update start_index
        /start_index stop_index 1 add def
      } if
      % increment stop index
      /stop_index stop_index 1 add def
    } forall
  ]
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets a string in an array with a given prefix
%
% ( array string1 -- string2 )
%
% array: an array of strings
% string1: the prefix to search for
% string2: if found, the string, otherwise ""
%
/get_string {
  exch
  {
    dup
    2 index exch is_prefix {
      exch pop return
    } if
    pop
  } forall
  pop
  ""
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% tests if a string is a prefix of another string
%
% ( string1 string2 -- bool )
%
% string1: the prefix
% string2: the string to test
% bool: true, if string1 is a prefix of string2, false otherwise
%
/is_prefix {
  % determine prefix length
  over length dup
  % determine string length
  2 index length

  gt {
    % the prefix is longer than the string...
    pop pop false return
  } {
    % test all characters of the prefix
    1 sub 0 1 rot
    {
        % get character from prefix
        dup 3 index exch get
        % get character from string at the same index
        exch 2 index exch get
        % compare
        ne {
          pop pop false return
        } if
    } for
    pop pop true return
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the value of a "key=value" string
%
% ( string1 -- string2 )
%
% string1: the "key=value" string
% string2: the value string
%
/get_value {
  dup
  {
    exch 1 add exch
    '=' eq {
      exit
    } if
  } forall
} def

% parse configuration file
%"live.cfg" findfile read_lines
%/config_lines exch def
%/language config_lines "LANGUAGE" get_string get_value def
%/desktop config_lines "DESKTOP" get_string get_value def
%70 330 moveto
%language show
%70 380 moveto
%desktop show

