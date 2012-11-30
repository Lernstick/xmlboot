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
  0
  exch
  {
    exch 10 mul exch
    48 sub add
  } forall
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
