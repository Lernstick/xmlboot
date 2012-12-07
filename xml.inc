% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% deserializes an xml document into an internal representation
%
% ( ptr -- array )
%
% ptr: pointer to the xml file
% array: top entity of the xml document
%
% every entity is an array with the following fields 
% 	name (string)
% 	attributes (array)
%       texts (string) and/or child entities (array) (array)
%
/parse_xml_file {

  /xml_file_pointer exch def
  xml_file_pointer

  move_to_next_tag
  /xml_file_pointer exch def

  {
    % open tag
    [
      % read entity name
      [ '/' '>' ' ' ] xml_file_pointer read_string
      /xml_file_pointer exch def

      % read attributes
      xml_file_pointer read_attributes
      /xml_file_pointer exch def

      % reading attributes above stops at the end of the open tag, either at the
      % '/' of an empty element '/>' or at the '>' of a non-empty element
      % test if current character is '/'
      xml_file_pointer 0 get '/' eq {
        % this is an empty element, close tag...
        ]
      } {
        % this is a "normal" element
        % read node texts and/or child entities
        [
          % jump over '>'
          /xml_file_pointer xml_file_pointer 1 add def

          xml_file_pointer 0 get '<' ne {
            % read node text
            [ '<' ] xml_file_pointer read_string
            /xml_file_pointer exch def
          } if
          % continue parsing
          xml_file_pointer parse_xml_file
      } ifelse

      % continue parsing
      xml_file_pointer parse_xml_file
  } {
    % check, if we have a close tag or EOF
    xml_file_pointer length 0 ne {
      % ok, we are at a close tag
      % we have to close the array for both the children and the entity itself
      ]
      ]
      % continue parsing
      xml_file_pointer parse_xml_file
    } if
  } ifelse

} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% searches for the next open or close tag
%
% ( ptr1 -- bool ptr2 )
%
% ptr1: pointer to the current position in the xml file
% 
% bool: true = open tag, false = close tag / no tag
% ptr2: updated pointer to the current position in the xml file
%
/move_to_next_tag {

  % evaluate every character of the pointer
  dup
  {
    % update pointer
    exch 1 add exch
    '<' eq {

      % We found '<'. Now check the next character.
      dup 0 get
      dup '/' eq {
        % this is a close tag ("</")
        pop false exch return
      } if
      dup '?' ne exch '!' ne and {
        % This is neither a declaration tag "<?" nor a comment "<!"
        % but a "real" open tag.
        true exch return
      } if
    } if
  } forall

  % no tag was found
  false exch return
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% reads a string
%
% ( array ptr1 -- string ptr2 )
%
% array: array with characters that stop reading
% ptr1: pointer to the current position in a file
%
% string: the read string
% ptr2: updated pointer to the current position in the file
%
/read_string {
  dup
  % put name end-pointer on stack
  0 exch
  {
    % check current character against all stop characters
    false
    4 index
    {
      2 index eq {
        pop true exit
      } if
    } forall
    exch pop
    {
      exit
    } if
    % increase name end-pointer
    1 add
  } forall
  % pop array with stop characters from stack
  3 -1 roll pop
  % allocate memory for entity name
  dup 1 add malloc
  % copy entity name into allocated string
  dup 3 index 3 index memcpy
  % convert pointer to string
  t_str settype
  % update text position pointer
  3 1 roll add
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% reads the attributes of an xml entity
%
% ( ptr1 -- array ptr2 )
%
% ptr1: pointer to the current position in the xml file
%
% array: the attributes of the entity (every attribute is an array of [ key value ]
% ptr2: updated pointer to the current position in the xml file
%
/read_attributes {
  [ exch
  {
    % check if the open tag ends here ('>' or '/>')
    dup 0 get '>' eq 1 index 0 get '/' eq or {
      % end of open tag reached
      /xml_file_pointer exch def
      ]
      xml_file_pointer
      return
    } {
      [ exch
      % read key
      skip_whitespaces
      [ ' ' '=' ] exch read_string

      % read value
      '"' forward 1 add
      [ '"' ] exch read_string 1 add
      /xml_file_pointer exch def
      ]
      xml_file_pointer
      skip_whitespaces
    } ifelse 
  } loop
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% forwards reading up to a certain character
%
% ( ptr1 int -- ptr2 )
%
% ptr1: pointer to the current position in the xml file
% int: the character where to stop forwarding
% ptr2: updated pointer to the current position in the xml file
%
/forward {
  exch
  dup
  {
    2 index eq {
      % we hit the character, remove it from stack
      exch pop return
    } {
      1 add
    } ifelse
  } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% skips whitespaces
%
% ( ptr1 -- ptr2 )
%
% ptr1: pointer to the current position in the xml file
% ptr2: updated pointer to the current position in the xml file
%
/skip_whitespaces {
  dup
  {
    ' ' eq {
      1 add
    } {
      return
    } ifelse
  } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the name of an xml entity
%
% ( ptr1 -- string )
%
% ptr1: pointer to an xml entity
%
% string: the name of the entity
%
/get_entity_name {
  0 get
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the children of an xml entity
%
% ( ptr1 -- array )
%
% ptr1: pointer to an xml entity
%
% array: the children of the entity
%
/get_entity_children {
  2 get
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets a named child of an xml entity
%
% ( ptr1 string -- ptr2 )
%
% ptr1: pointer to an xml entity
% string: name of the child
%
% ptr2: pointer to the child (ptr to 0, if not found)
%
/get_entity_child {
  % loop over all entity children
  exch
  get_entity_children
  {
    % check element type
    dup gettype t_str eq {
      % ignore text elements
      pop
    } {
      % check child name
      dup get_entity_name 2 index eq {
        % remove child name from stack
        % (only leave found child on stack)
        exch pop return
      } {
        % this is the wrong child
        pop
      } ifelse
    } ifelse
  } forall

  % child was not found...
  % remove child name from stack
  pop
  % leave pointer to 0 on stack
  0 t_none settype
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the value of an xml entity attribute
%
% ( ptr1 string1 -- string2 )
%
% ptr1: pointer to an xml entity
% string1: the name of the entity attribute
%
% string2: the value of the entity attribute or "" if the attribute was not found
%
/get_attribute_value {
  % loop over attribute array
  exch
  1 get
  {
    dup
    0 get 2 index eq {
      % attribute key found, leave value on stack
      1 get
      % remove attribute name from stack
      exch pop
      return
    } if
    pop
  } forall

  % attribute key not found
  % remove attribute name from stack
  % and leave "" on stack
  pop
  ""
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the string content of an entity child
%
% ( array string1 -- string2 )
%
% array: an xml entity
% string1: the name of the entity child
%
% string2: the string content of the entity child or "" if it was not found
%
/get_entity_child_string {
  get_entity_child
  dup 0 t_none settype eq {
    % there was no text child
    pop
    ""
  } {
    get_entity_children
    dup length 0 eq {
      % text child was empty
      pop
      ""
    } {
      % get content of text child
      0 get
    } ifelse
  } ifelse
} def
