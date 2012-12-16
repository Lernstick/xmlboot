% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% allocates memory for a string
%
% ( int -- string )
%
% int: the length of the string to allocate
%
% string: the allocated string
%
/string {
  1 add malloc 4 settype
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% formats strings
%
% ( obj1 ... objN str ptr -- )
%
% obj1 ... objN: printf arguments
% str1: format string
% ptr: buffer for storing the resulting string
%
/sprintf {
  dup t_ptr settype length exch snprintf
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% converts a character into an integer
%
% ( int1 -- int1 )
%
% int1: the input character
%
% int2: the integer representation of the character
%
/char_to_int {
  % characters within range "0".."9"
  dup 47 gt exch dup 58 lt rot and {
    48 sub return
  } if

  % characters within range "A".."F"
  dup 64 gt exch dup 71 lt rot and {
    55 sub return
  } if

  % assume characters within range "a".."f"
  87 sub
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% converts a string into an integer
%
% ( string -- int )
%
% string: the input string
%
% int: the integer representation of the string
%
/string_to_int {
  % put int for result on stack
  0

  % determine factor
  exch
  dup 0 get exch dup 1 get
  rot
  48 eq exch 120 eq and {
    % string started with "0x"
    % skip "0x"
    2 add
    % this is a hexadecimal number
    16
  } {
    % this is a decimal number
    10
  } ifelse
  exch
  {
    rot 2 index mul exch
    char_to_int add
    exch
  } forall

  % remove factor from stack
  pop
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% concatenate two strings
%
% ( string1 string2 -- string3 )
%
% string1: the first string
% string2: the second string
%
% string3: the concatenated strings ("<string1><string2>")
%
/concatenate_strings {
  % reverse order because of snprintf reversed argument order
  exch

  % allocate space for new string
  dup length
  2 index length
  add
  string
  /concatenated_string exch def

  % concatenate
  "%s%s" concatenated_string sprintf
  concatenated_string
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% converts a comma separated string into an array
%
% ( string -- array )
%
% string: a string with comma separated values
% array: an array containing all strings
%
/csv_to_array {
  /csv_string exch def

  % early return for empty strings
  csv_string length 0 eq {
    [ ]
    return
  } if

  /start_index 0 def
  /stop_index 0 def
  [
    csv_string
    {
      % the ASCII comma ',' is '44' in decimal notation
      44 eq {
        % leave value on stack
        private_get_csv_value
        % update start_index
        /start_index stop_index 1 add def
      } if
      % increment stop index
      /stop_index stop_index 1 add def
    } forall
    % leave final value (after last comma) on stack
    private_get_csv_value
  ]
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% puts a cvs value on stack
%
% ( -- string )
%
% string: the found csv value
%
/private_get_csv_value {
  % allocate memory for value
  stop_index start_index sub string
  /value exch def

  % copy value
  /value_index 0 def
  start_index 1 stop_index 1 sub {
    csv_string exch get
    value value_index rot put
    /value_index value_index 1 add def
  } for

  % leave value as string on stack for array
  value
} def
