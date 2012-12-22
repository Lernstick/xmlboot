% the file cache
% it is an array of cache entries
% a cache entry is an array of a string (the file name)
% and a pointer (memory location of file content)
/file_cache [ ] def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% loads a file
% if the file was previously loaded, the cached file pointer is returned
%
% ( string -- ptr )
%
% string: the name of the file to find and load
%
% ptr: the pointer to the file data
%
/find_cached_file {
  % search file name in cache
  file_cache {
    dup 0 get 2 index eq {
      % file name found in cache
      % leave file pointer on stack
      1 get
      % remove file name from stack
      exch pop
      return
    } {
      % this was a cache entry to another file...
      pop
    } ifelse
  } forall

  % the file name was not found in cache
  % create new cache entry
  [
    exch
    dup findfile
  ]

  % enlarge cache (by one)
  enlarge_file_cache
  file_cache file_cache length 1 sub rot put

  % leave file pointer on stack
  file_cache file_cache length 1 sub get 1 get
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% enlarges the file cache (by one)
%
% ( -- )
%
/enlarge_file_cache {
  % allocate memory for enlarged cache
  file_cache length 1 add array

  % copy all pointers from old to new cache
  file_cache length 0 gt {
    0 1 file_cache length 1 sub {
      dup
      file_cache exch get
      2 index
      3 1 roll
      put
    } for
  } if

  % free old cache
  file_cache free

  % point to new, enlarged cache
  /file_cache exch def
} def
