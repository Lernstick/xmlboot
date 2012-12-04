% menu variables
/menu.selected_index 0 def
/menu.type.start  0 def
/menu.type.config 1 def
/menu.type.option 2 def
/menu.type.menu   3 def
/menu.type.back   4 def

% menu specifig indices
/menu.index.type    0 def
/menu.index.text    1 def
/menu.index.entries 2 def

/menu.alignment.left   0 def
/menu.alignment.center 1 def
/menu.alignment menu.alignment.center def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% parse menu structure
%
% ( array1 -- array2 )
%
% array1: the menu definition in an xml structure
%
% array2: the parsed menu
%
% menus are arrays with the following fields:
%   0: type == menu.type.menu
%   1: text (string, the title of the menu) 
%   2: entries (array of start/config entries and/or other menus)
%
% start menu entries are arrays with the following fields:
%   0: type == menu.type.start
%   1: text (string, the text to display in the start menu)
%   2: index (string, the index in the syslinux config file to start)
%   3: label (string, the label in the syslinux config file to start)
%   4: append_config (bool, if the config should be appended to the label when booting)
%
% config menus are arrays with the following fields:
%   0: type == menu.type.config
%   1: id (string, config id)
%
% back entries are arrays with the following fields:
%   0: type == menu.type.back
%   1: text (string, the text to display in the menu)
%
/parse_menu {
  [
    exch
    % menu type
    menu.type.menu

    % menu text
    exch dup "text" get_attribute_value

    % menu entries
    exch
    [
      exch get_entity_children {
        dup gettype t_arr eq {
          % child is another entity
          % check, if entitiy is supported
          dup get_entity_name "start" eq {
            [
              exch
              menu.type.start
              exch dup "text" get_attribute_value
              exch dup "index" get_attribute_value string_to_int
              exch dup "label" get_attribute_value
              exch "append_config" get_attribute_value
              "true" eq {
                true
              } {
                false
              } ifelse
            ]
          } {
            dup get_entity_name "config" eq {
              [
                exch
                menu.type.config
                exch "id" get_attribute_value
              ]
            } {
              dup get_entity_name "menu" eq {
                % parse submenu (recursively)
                parse_menu
              } {
                dup get_entity_name "back" eq {
                  [
                    exch
                    menu.type.back
                    exch "text" get_attribute_value
                  ]
                } {
                  % entity is not supported
                  pop
                } ifelse
              } ifelse
            } ifelse
          } ifelse
        } {
          % child is just some text
          pop
        } ifelse
      } forall
    ]
  ]
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the selected index of a config options menu
%
% ( array -- int )
%
% array: the config options menu
%
% int: the selected index
%
/get_selected_option_index {
  0
  exch
  {
    4 get {
      return
    } if
    1 add
  } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% converts a config into a menu structure
%
% ( string -- array )
%
% string: the config id
%
% array: the menu structure
%
/config_to_menu {
  /tmp_config_id exch def
  [
    % type
    menu.type.menu

    % text
    tmp_config_id get_configuration
    dup config.index.text get
    exch

    % options
    [
      % loop over all config options
      exch config.index.options get {
        [
          exch
          % option menus are arrays with the following fields:
          %   0: type == menu.type.option
          %   1: config id (string, config id)
          %   2: option id (string, option id)
          %   3: text (string, config description)
          %   4: selected (boolean)

          % type
          menu.type.option
          exch

          % config id
          tmp_config_id
          exch 

          % option id
          dup 0 get

          % text
          exch dup 1 get

          % selected
          exch 4 get
        ]
      } forall
    ]
  ]
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles a start menu entry
%
% ( array -- string int int )
%
% array: the selected start menu entry
%
% string: the label and append line for syslinux
% int: the index for syslinux
% int: the action for syslinux (always 2 == boot)
%
/handle_start_menu_entry {
  % get menu label
  dup 3 get

  % check, if the config should be appended to the label
  exch dup 4 get {
    % yes, append config to label

    % get currently selected config options and
    % assemble them in one large append line
    /append_line 0 string def
    configurations {
      % get options array of config
      config.index.options get {
        % only use selected options
        dup 4 get {
          % option is selected
          % get option specific append line
          2 get
          % concatenate with global append line
          append_line " " concatenate_strings
          exch
          concatenate_strings
          /append_line exch def
        } {
          % option is not selected
          pop
        } ifelse
      } forall
    } forall

    % concatenate label and append line
    exch append_line concatenate_strings
    % get menu entry back to top of stack
    exch
  } if

  % get menu index
  2 get 

  % "2" means that we want to boot
  2
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles a config menu entry
%
% ( array -- )
%
% array: the selected config menu entry
%
/handle_config_menu_entry {
  % paint config options menu
  1 get
  config_to_menu
  /menu.entries exch def

  % add config menu to breadcrumbs
  /last_breadcrumb [
    last_breadcrumb
    menu.entries
    menu.selected_index
  ] def

  % repaint menu
  /menu.alignment menu.alignment.left def
  paint_menu

  %find and highlight selected option
  menu.entries menu.index.entries get get_selected_option_index
  /menu.selected_index exch def
  menu.selected_index menu.select
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles a config option menu entry
%
% ( array -- )
%
% array: the selected config option menu entry
%
/handle_option_menu_entry {
  % update configuration
  % get option id
  dup 2 get
  % get configuration
  exch 1 get get_configuration
  % get config trigger
  dup config.index.trigger get
  /tmp_config_trigger exch def
  % loop through all configuration options
  config.index.options get {
    % get id
    dup 0 get
    % check against id of option menu entry
    2 index eq {
      % this is the selected option
      dup 4 true put
      % get option trigger
      3 get
      /tmp_option_trigger exch def
    } {
      % this is not the selected option
      4 false put
    } ifelse
  } forall
  % remove option id from TOS
  pop

  % handle config trigger (if any)
  tmp_config_trigger "" ne {
    tmp_config_trigger get_configuration
    % loop through all configuration options
    config.index.options get {
      % get id
      dup 0 get
      % check against option trigger id
      tmp_option_trigger eq {
        % this is the selected option
        4 true put
      } {
        % this is not the selected option
        4 false put
      } ifelse
    } forall
  } if

  % restore selected index from breadcrumbs
  /menu.selected_index last_breadcrumb 2 get def

   % get previous menu from breadcrumbs
  last_breadcrumb
  /last_breadcrumb last_breadcrumb 0 get def
  free
  /menu.entries last_breadcrumb 1 get def
  
  % repaint menu 
  % !!! breadcrumbs must be updated before repainting the menu !!!
  /menu.alignment menu.alignment.center def
  paint_menu
  menu.selected_index menu.select

} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles a submenu menu entry
%
% ( array -- )
%
% array: the selected submenu entry
%
/handle_submenu_entry {
  % update pointer to menu entries
  /menu.entries exch def

  % add submenu to breadcrumbs
  /last_breadcrumb [
    last_breadcrumb
    menu.entries
    menu.selected_index
  ] def

  % paint new submenu
  % !!! breadcrumbs must be updated before repainting the menu !!!
  paint_menu

  % select first index in submenu
  /menu.selected_index 0 def
  menu.selected_index menu.select
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles a back entry
%
% ( array -- )
%
% array: the selected back entry
%
/handle_back_entry {
  % there is nothing in the back entry that we need now...
  pop

  % restore selected index from breadcrumbs
  /menu.selected_index last_breadcrumb 2 get def
  
  % get previous menu from breadcrumbs
  last_breadcrumb
  /last_breadcrumb last_breadcrumb 0 get def
  free
  /menu.entries last_breadcrumb 1 get def
  
  % repaint menu 
  % !!! breadcrumbs must be updated before repainting the menu !!!
  paint_menu
  menu.selected_index menu.select
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the x offset to center a string
%
% ( string -- int )
%
% string: the string to show centered
%
% int: the x value to center the string
%
/centered_x_value {
  strsize pop
  screen.size pop
  exch sub
  2 div
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% shows a text centered on screen
%
% ( int string -- )
%
% int: y value of the cursor position
% string: the string to show centered
%
/show_centered {
  dup centered_x_value
  rot moveto
  show
} def
 
% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% add highlight to menu item.
%
% ( int -- )
%
% int: the index to highlight
%
/menu.select {
  % move to menu position
  dup 
  menu.alignment menu.alignment.center eq {
    dup menu.entries menu.index.entries get exch get
    menu_to_text centered_x_value
  } {
    left_align_x
  } ifelse
  exch menu.dy mul menu.y add
  moveto

  % reset menu entry region
  menu.entries menu.index.entries get exch get
  menu_to_text
  dup string_region_reset

  % paint focus rectangle
  dup strsize
  8 mul 10 div
  exch
  10 add
  exch
  -5 0 rmoveto
  0x000000 setcolor
  1 index
  1 index
  fillrect
  -1 -1 rmoveto
  menu.rectangle_color setcolor
  fillrect
  6 1 rmoveto

  % paint text in highlighted color
  menu.selected_color setcolor
  show
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% remove highlight of menu item.
%
% ( int -- )
%
% int: the index where to remove the highlight
%
/menu.deselect {
  % move to menu position
  dup 
  menu.alignment menu.alignment.center eq {
    dup menu.entries menu.index.entries get exch get
    menu_to_text centered_x_value
  } {
    left_align_x
  } ifelse
  exch menu.dy mul menu.y add
  moveto

  % reset menu entry region
  menu.entries menu.index.entries get exch get
  menu_to_text
  dup strsize
  exch
  11 add
  exch
  0xc7d9f0 setcolor
  -6 -1 rmoveto
  currentpoint
  4 2 roll
  image
  6 1 rmoveto

  % paint text in normal color
  menu.color setcolor
  show
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% erases the currently shown menu from screen
%
% ( -- )
%
%
/menu.erase {
  % clear screen
  0 0 moveto
  0 0 image.size image

  % repaint version info
  "font_size_10.fnt" findfile setfont
  0x000000 setcolor
  version_info
  dup strsize exch pop
  screen.size exch pop
  exch sub
  10 sub
  10 exch
  moveto
  show
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% shows the image region of a string
%
% ( string -- )
%
% string: the string whose image region is repainted
%
/string_region_reset {
  currentpoint rot strsize image
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets a configuration by id
%
% ( string -- array )
%
% string: the configuration id
%
% array: the configuration
%
/get_configuration {
  configurations {
    dup config.index.id get
    2 index eq {
      % this is the config we were searching for
      % remove config id from stack
      exch pop
      return
    } {
      % this is another config...
      % just remove it from the stack
      pop
    } ifelse
  } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the textual representation of the selected option of a configuration
%
% ( array -- string )
%
% array: a configuration entry
%
% string: the textual representation of the selected option
%
/get_selected_option_text {
  % loop over all config options
  config.index.options get {
    % check, if option is selected
    dup 4 get {
      % selected option found, leave text on stack
      1 get return
    } {
      % option not selected, remove from stack
      pop
    } ifelse
  } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% gets the textual representation of a menu entry
%
% ( array -- string )
%
% array: a menu entry
%
% string: the textual representation of the menu entry
%
/menu_to_text {
  % check menu entry type
  dup 0 get
  dup menu.type.start eq {
    pop
    % leave start menu text on stack
    1 get
  } {
    dup menu.type.config eq {
      pop
      % get configuration by id
      1 get get_configuration
      % get configuration text
      dup config.index.text get
      % show currently selected option
      ": " concatenate_strings
      exch
      get_selected_option_text
      concatenate_strings
    } {
      dup menu.type.option eq {
        pop

        % get radio button
        dup 4 get {
          % this is the selected option
          % add selected radiobutton prefix
          "◉ "
        } {
          % this is a nonselected option
          % add nonselected radiobutton prefix
          "○ "
        } ifelse

        % get option menu text
        exch 3 get

        % concatenate radio button with text
        concatenate_strings
      } {
        dup menu.type.menu eq {
          pop
          % get (sub)menu text
          menu.index.text get
          breadcrumbs.delimiter concatenate_strings
        } {
          menu.type.back eq {
            % get back entry text
            1 get
          } if
        } ifelse
      } ifelse
    } ifelse
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% paints a menu on screen
%
% ( -- )
%
/paint_menu {
  % reset screen
  menu.erase

  % determine breadcrumbs text
  ""
  last_breadcrumb
  {
    % get breadcrumb menu
    dup 1 get

    % get menu text
    1 get
    % concatenate
    rot
    dup "" eq {
      % this is the last breadcrumb
      % just leave its text on stack
      % and ignore the previous text
      pop
    }  {
      % this is not the last breadcrumb
      % put its text *in front* of the already existing
      % breadcrumbs text
      breadcrumbs.delimiter exch concatenate_strings
      concatenate_strings
    } ifelse

    % move to previous breadcrumb
    exch 
    0 get
    dup 0 eq {
      % this is the first breadcrumb
      pop
      exit
    } if
  } loop

  % paint breadcrumbs text
  breadcrumbs.color setcolor
  breadcrumbs.font findfile setfont
  breadcrumbs.y exch show_centered

  % paint all menu entries
  menu.color setcolor
  /y_offset menu.y def
  menu.font findfile setfont

  menu.alignment menu.alignment.center eq {
    % center all texts
    menu.entries menu.index.entries get {
      menu_to_text
      y_offset exch show_centered
      /y_offset y_offset menu.dy add def
    } forall
  } {
    % left align all texts
    update_left_align_x
    menu.entries menu.index.entries get {
      menu_to_text
      left_align_x y_offset moveto show
      /y_offset y_offset menu.dy add def
    } forall
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% updates the x position if the current menu would be left aligned
%
% ( -- )
%
/update_left_align_x {
  % determine longest text in menu
  /max_width 0 def
  menu.entries menu.index.entries get {
    menu_to_text strsize pop
    dup max_width gt {
      /max_width exch def
    } {
      pop
    } ifelse
  } forall
  screen.size pop
  max_width sub
  2 div
  /left_align_x exch def
} def

