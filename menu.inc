% menu variables
/menu.selected_index 0 def

% menu entry types
/menu.type.start         0 def
/menu.type.config        1 def
/menu.type.config_option 2 def
/menu.type.option        3 def
/menu.type.menu          4 def
/menu.type.back          5 def

% menu specific indices
/menu.index.type          0 def
/menu.index.icon          1 def
/menu.index.y             2 def
/menu.index.dy            3 def
/menu.index.icon_inset    4 def
/menu.index.font          5 def
/menu.index.edit_x        6 def
/menu.index.edit_y        7 def
/menu.index.edit_width    8 def
/menu.index.indention     9 def
/menu.index.text         10 def
/menu.index.entries      11 def
/menu.index.show_edit    12 def
/menu.index.edit_prompt  13 def

% start menu specific indices
/startmenu.index.icon           1 def
/startmenu.index.text           2 def
/startmenu.index.syslinux_index 3 def
/startmenu.index.label          4 def
/startmenu.index.append_config  5 def

% config menu specific indices
/configmenu.index.id                1 def
/configmenu.index.icon              2 def
/configmenu.index.description_color 3 def
/configmenu.index.y                 4 def
/configmenu.index.dy                5 def
/configmenu.index.font              6 def
/configmenu.index.indention         7 def
/configmenu.index.description_x     8 def
/configmenu.index.description_y     9 def
/configmenu.index.screenshot_x     10 def
/configmenu.index.screenshot_y     11 def

% config option menu entry specific indices
/config_option_menu_entry.index.config_id   1 def
/config_option_menu_entry.index.option_id   2 def
/config_option_menu_entry.index.text        3 def
/config_option_menu_entry.index.description 4 def
/config_option_menu_entry.index.screenshot  5 def
/config_option_menu_entry.index.selected    6 def

% option menu entry specific indices
/option_menu_entry.index.id   1 def

% back entry specific indices
/back.index.icon 1 def
/back.index.text 2 def

% alignment
/menu.alignment.left   0 def
/menu.alignment.center 1 def
/menu.alignment menu.alignment.left def

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
%    0: type        (int,     always menu.type.menu)
%    1: icon        (string,  name of the icon file)
%    2: y           (int,     y position of the menu on screen)
%    3: dy          (int,     "line spacing" of the menu)
%    4: icon_inset  (int,     inset between icon and text)
%    5: font        (string,  name of the font file)
%    6: edit_x      (int,     x position of the edit field)
%    7: edit_y      (int,     y position of the edit field)
%    8: edit_width  (int,     width of the edit field)
%    9: indention   (int,     indention to apply when showing this menu in upper menu)
%   10: text        (string,  title of the menu) 
%   11: entries     (array,   back/start/config/option entries and/or other submenus)
%   12: show_edit   (boolean, if the edit input field should be visible when showing this menu)
%   13: edit_prompt (string,  prompt to show in front of the edit field)
%
% start menu entries are arrays with the following fields:
%   0: type == menu.type.start
%   1: icon (string, the name of the icon file)
%   2: text (string, the text to display in the start menu)
%   3: index (string, the index in the syslinux config file to start)
%   4: label (string, the label in the syslinux config file to start)
%   5: append_config (bool, if the config should be appended to the label when booting)
%
% config menus entries are arrays with the following fields:
%    0: type              (int,    always menu.type.config)
%    1: id                (string, config id)
%    2: icon              (string, the name of the icon file)
%    3: description_color (int, the color to use when showing descriptions)
%    4: y                 (int,    y position of the config menu on screen)
%    5: dy                (int,    "line spacing" of the config menu)
%    6: font              (string, name of the font file)
%    7: indention         (int,    indention to apply when showing this config menu in upper menu)
%    8: description_x     (int,    x position of menu entry descriptions)
%    9: description_y     (int,    y position of menu entry descriptions)
%   10: screenshot_x      (int,    x position of menu entry screenshots)
%   11: screenshot_y      (int,    y position of menu entry screenshots)
%
% option menu entries are arrays with the following fields:
%   0: type == menu.type.option
%   1: id (string, option id)
%
% back entries are arrays with the following fields:
%   0: type == menu.type.back
%   1: icon (string, the name of the icon file)
%   2: text (string, the text to display in the menu)
%
/parse_menu {
  [
    exch
    % menu type
    menu.type.menu

    % menu icon
    exch dup "icon" get_attribute_value

    % layout specific y, dy, icon_inset, font, edit_x and indention values
    /tmp_no_layout_found true def
    exch dup get_entity_children {
      dup gettype t_arr eq {
        dup get_entity_name "layout" eq {
          dup "width" get_attribute_value string_to_int
          exch dup "height" get_attribute_value string_to_int
          rot exch screen.size rot eq rot rot eq and {
            % layout config for current screen height found
            dup "y" get_attribute_value string_to_int
            exch dup "dy" get_attribute_value string_to_int
            exch dup "icon_inset" get_attribute_value string_to_int
            exch dup "font" get_attribute_value
            exch dup "edit_x" get_attribute_value string_to_int
            exch dup "edit_y" get_attribute_value string_to_int
            exch dup "edit_width" get_attribute_value string_to_int
            exch "indention" get_attribute_value string_to_int
            /tmp_no_layout_found false def
            exit
          } {
            % a layout config for another screen height (ignore)
            pop
          } ifelse
        } {
          % not (yet) supported
          pop
        } ifelse
      } {
        % child entry was just some text
        pop
      } ifelse
    } forall
    tmp_no_layout_found {
      % the meny layout for the current screen height
      % was not found in config file!
      "menu layout missing!" dtrace pop
    } if
    9 8 roll

    % menu text
    dup "text" get_entity_child_string

    % menu entries
    exch dup
    [
      exch get_entity_children {
        dup gettype t_arr eq {
          % child is another entity
          % check, if entity is supported
          dup get_entity_name "start" eq {
            [
              exch
              menu.type.start
              exch dup "icon" get_attribute_value
              % get text from child element
              exch dup get_entity_children 0 get
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

                % type
                menu.type.config

                % id
                exch dup "id" get_attribute_value

                % icon
                exch dup "icon" get_attribute_value

                % description_color
                exch dup "description_color" get_attribute_value string_to_int

                % layout specific values
                /tmp_no_layout_found true def
                exch dup get_entity_children {
                  dup gettype t_arr eq {
                    dup get_entity_name "layout" eq {
                      dup "width" get_attribute_value string_to_int
                      exch dup "height" get_attribute_value string_to_int
                      rot exch screen.size rot eq rot rot eq and {
                        % layout config for current screen height found
                        dup "y" get_attribute_value string_to_int
                        exch dup "dy" get_attribute_value string_to_int
                        exch dup "font" get_attribute_value
                        exch dup "indention" get_attribute_value string_to_int
                        exch dup "description_x" get_attribute_value string_to_int
                        exch dup "description_y" get_attribute_value string_to_int
                        exch dup "screenshot_x" get_attribute_value string_to_int
                        exch "screenshot_y" get_attribute_value string_to_int
                        /tmp_no_layout_found false def
                        exit
                      } {
                        % a layout config for another screen height (ignore)
                        pop
                      } ifelse
                    } {
                      % not (yet) supported
                      pop
                    } ifelse
                  } {
                    % child entry was just some text
                    pop
                  } ifelse
                } forall
                tmp_no_layout_found {
                  % the meny layout for the current screen height
                  % was not found in config file!
                  "config layout missing!" dtrace pop
                } if
                % remove entity from stack
                9 8 roll pop
              ]
            } {
              dup get_entity_name "option" eq {
                [
                  exch
                  menu.type.option
                  exch "id" get_attribute_value
                ]
              } {
                  dup get_entity_name "submenu" eq {
                  % parse submenu (recursively)
                  parse_menu
                } {
                  dup get_entity_name "back" eq {
                    [
                      exch
                      menu.type.back
                      exch dup "icon" get_attribute_value
                      % get text from child element
                      exch get_entity_children 0 get
                    ]
                  } {
                    % entity is not supported
                    pop
                  } ifelse
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

    % visibility of edit input field
    exch dup "show_edit" get_attribute_value "true" eq {
      true
    } {
      false
    } ifelse

    % edit input field prompt
    exch "edit_prompt" get_entity_child_string
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
    config_option_menu_entry.index.selected get {
      return
    } if
    1 add
  } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% converts a config into a (temporary) menu structure
%
% ( array1 -- array2 )
%
% array1: the selected config menu entry
%
% array2: the (temporary) menu structure containing this configurations options
%
/config_to_menu {
  /tmp_config_menu exch def
  /tmp_config_id tmp_config_menu config_option_menu_entry.index.config_id get def

  [
    % type
    menu.type.menu

    % (no) icon
    ""

    % y
    tmp_config_menu configmenu.index.y get

    % dy
    tmp_config_menu configmenu.index.dy get

    % (no) icon inset
    0

    % font
    tmp_config_menu configmenu.index.font get

    % x,y position of edit input field
    0 0

    % width of edit input field
    0

    % indention
    0

    % text
    tmp_config_id get_configuration
    dup config.index.text get

    % use config options as menu entries
    exch
    [
      % loop over all config options
      exch config.index.options get {
        [
          exch
          % config option menus entries cache the values of its config option
          % they are arrays with the following fields:
          %   0: type        (int,     always menu.type.config_option)
          %   1: config id   (string,  config ID)
          %   2: option id   (string,  option ID)
          %   3: text        (string,  short config option name)
          %   4: description (string,  longer config option description)
          %   5: screenshot  (string,  screenshot for config option)
          %   6: selected    (boolean, if this option is selected)

          % type
          menu.type.config_option
          exch

          % config id
          tmp_config_id
          exch 

          % option id
          dup config_option.index.id get

          % text
          exch dup config_option.index.text get gettext

          % description
          exch dup config_option.index.description get gettext

          % screenshot
          exch dup config_option.index.screenshot get

          % selected
          exch config_option.index.selected get
        ]
      } forall
    ]

    % visibility of edit input field (invisible)
    false

    % prompt of edit input field
    ""
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
  dup startmenu.index.label get

  % check, if the config should be appended to the label
  exch dup startmenu.index.append_config get {
    % yes, append config to label

    % get currently selected config options and
    % assemble them in one large append line
    /append_line 0 string def
    configurations {
      % get options array of config
      config.index.options get {
        % only use selected options
        dup config_option.index.selected get {
          % option is selected
          % get option specific append line
          config_option.index.append get
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

    % append currently selected options
    options {
      % get option specific append line
      dup
      option.index.selected get {
        % option is selected
        option.index.append_selected get
      } {
        % option is not selected
        option.index.append_nonselected get
      } ifelse

      % concatenate with global append line
      dup "" eq {
        % ignore empty options
        pop
      } {
        append_line " " concatenate_strings
        exch
        concatenate_strings
        /append_line exch def
      } ifelse
    } forall

    % append custom options
    append_line " custom_options " concatenate_strings
    edit_input_buffer concatenate_strings
    /append_line exch def

    % concatenate label and append line
    exch append_line concatenate_strings
    % get menu entry back to top of stack
    exch
  } if

  % get syslinux index
  startmenu.index.syslinux_index get 

  % "2" means that we want to boot
  2
  
  % !!! Do *NOT* use keepmode !!!
  % keepmode breaks the hardware detection tool and memtest.
  % I just leave it here commented for documentation...
  % 1 keepmode
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles a config menu entry
%
% ( array -- )
%
% array: the selected config menu entry
%
/handle_config_menu_entry {
  % update description position
  dup configmenu.index.description_x get
  /description_x exch def
  dup configmenu.index.description_y get
  /description_y exch def
  dup configmenu.index.description_color get
  /description_color exch def
  dup configmenu.index.screenshot_x get
  /screenshot_x exch def
  dup configmenu.index.screenshot_y get
  /screenshot_y exch def

  % paint config options menu
  config_to_menu
  /current.menu exch def

  % add config menu to breadcrumbs
  /last_breadcrumb [
    last_breadcrumb
    current.menu
    menu.selected_index
  ] def

  % repaint menu
  /menu.alignment menu.alignment.left def
  paint_menu

  %find and highlight selected option
  current.menu menu.index.entries get
  get_selected_option_index
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
/handle_config_option_menu_entry {
  % update configuration
  % get option id
  dup config_option_menu_entry.index.option_id get
  /tmp_option_id exch def
  tmp_option_id
  % get configuration
  exch config_option_menu_entry.index.config_id get
  /tmp_config_id exch def
  tmp_config_id
  get_configuration
  % get config trigger
  dup config.index.trigger get
  /tmp_config_trigger exch def
  % loop through all configuration options
  config.index.options get {
    % get id
    dup config_option.index.id get
    % check against id of option menu entry
    2 index eq {
      % this is the selected option
      dup config_option.index.selected true put
      % get option trigger
      config_option.index.trigger get
      /tmp_option_trigger exch def
    } {
      % this is not the selected option
      config_option.index.selected false put
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
      dup config_option.index.id get
      % check against option trigger id
      tmp_option_trigger eq {
        % this is the selected option
        config_option.index.selected true put
      } {
        % this is not the selected option
        config_option.index.selected false put
      } ifelse
    } forall
  } if

  % handle special options
  tmp_config_id "language" eq {
    tmp_option_id ".translation" concatenate_strings
    read_translation_file
  } if

  % restore selected index from breadcrumbs
  /menu.selected_index last_breadcrumb breadcrumb.index.current_index get def

  % free dynamically allocated memory of this menu entry
  current.menu menu.index.entries get {
    free
  } forall
  current.menu free

   % get previous menu from breadcrumbs
  last_breadcrumb
  /last_breadcrumb last_breadcrumb breadcrumb.index.previous get def
  free
  /current.menu last_breadcrumb breadcrumb.index.current_menu get def

  % repaint menu 
  % !!! breadcrumbs must be updated before repainting the menu !!!
  % /menu.alignment menu.alignment.center def
  paint_menu
  menu.selected_index menu.select

} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% handles an option menu entry
%
% ( array -- )
%
% array: the selected option menu entry
%
/handle_option_menu_entry {
  % toggle option
  option_menu_entry.index.id get get_option
  dup
  dup option.index.selected get {
    % update option
    option.index.selected false put

    % handle off->on triggers
    dup option.index.off_on_triggers get {
      dup get_option
      option.index.selected true put
      repaint_option
    } forall

    % handle off->off triggers
    option.index.off_off_triggers get {
      dup get_option
      option.index.selected false put
      repaint_option
    } forall
  } {
    % update option
    option.index.selected true put

    % handle on->on triggers
    dup option.index.on_on_triggers get {
      dup get_option
      option.index.selected true put
      repaint_option
    } forall

    % handle on->off triggers
    option.index.on_off_triggers get {
      dup get_option
      option.index.selected false put
      repaint_option
    } forall
  } ifelse

  % repaint option in menu
  menu.selected_index menu.select
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% repaints an option (if visible)
%
% ( string -- )
%
% string: the id of the option
%
/repaint_option {
  /tmp_index 0 def
  current.menu menu.index.entries get {
    dup menu.index.type get
    menu.type.option eq {
      % check if this is the option we have to repaint
      option_menu_entry.index.id get 1 index eq {
        tmp_index menu.deselect
      } if
    } {
      % other menu type, ignored
      pop
    } ifelse
    /tmp_index tmp_index 1 add def
  } forall
  pop
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
  /current.menu exch def

  % add submenu to breadcrumbs
  /last_breadcrumb [
    last_breadcrumb
    current.menu
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
  /menu.selected_index last_breadcrumb breadcrumb.index.current_index get def

  % get previous menu from breadcrumbs
  last_breadcrumb
  /last_breadcrumb last_breadcrumb breadcrumb.index.previous get def
  free
  /current.menu last_breadcrumb breadcrumb.index.current_menu get def

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
% gets the indention of a menu item
%
% ( array -- int )
%
% arry: the menu item
%
% int: the indention of the menu item
%
/get_indention {
  dup menu.index.type get
  dup menu.type.config eq {
    pop
    configmenu.index.indention get
  } {
    menu.type.menu eq {
      menu.index.indention get
    } {
      % menu entries without indention
      pop
      0 return
    } ifelse
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% moves to the location of a menu entry
%
% ( int -- )
%
% int: the index of the menu entry
%
/moveto_menu_coordinates {

  /tmp_index exch def

  % get indention
  current.menu menu.index.entries get tmp_index get
  get_indention
  /indention exch def

  % move to menu position
  menu.alignment menu.alignment.center eq {
    current.menu menu.index.entries get tmp_index get
    menu_to_text centered_x_value
  } {
    left_align_x indention add
  } ifelse
  tmp_index menu.dy mul menu.y add
  moveto
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% add highlight to menu item.
%
% ( int -- )
%
% int: the index to highlight
%
/menu.select {
  % move to correct position on screen
  dup moveto_menu_coordinates

  % get menu text
  current.menu menu.index.entries get exch get
  menu_to_text

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

  % show screenshot and/or description (if supported by entry type)
  current.menu menu.index.entries get menu.selected_index get
  dup menu.index.type get
  menu.type.config_option eq {
    % show config option screenshot
    dup config_option_menu_entry.index.screenshot get
    dup "" eq {
      % no screenshot file configured
      pop
    } {
      screenshot_x screenshot_y moveto
      % remember screenshot dimensions
      find_cached_file setimage image.size
      /screenshot_height exch def
      /screenshot_width exch def
      0 0 screenshot_width screenshot_height image
      default_image setimage
    } ifelse

    % show config option description
    config_option_menu_entry.index.description get
    description_color setcolor
    description_x description_y moveto
    show
  } {
    % menu type does not have a description
    pop
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% remove highlight of menu item.
%
% ( int -- )
%
% int: the index where to remove the highlight
%
/menu.deselect {
  dup moveto_menu_coordinates

  % reset menu entry region
  current.menu menu.index.entries get exch get
  menu_to_text
  dup strsize
  exch
  11 add
  exch
  -6 -1 rmoveto
  currentpoint
  4 2 roll
  image
  6 1 rmoveto

  % paint text in normal color
  menu.color setcolor
  show

  % reset screenshot and/or description (if supported by entry type)
  current.menu menu.index.entries get menu.selected_index get
  dup menu.index.type get
  menu.type.config_option eq {
    % reset config option screenshot
    dup config_option_menu_entry.index.screenshot get
    dup "" eq {
      % no screenshot file configured
      pop
    } {
      pop
      default_image setimage
      screenshot_x screenshot_y moveto
      screenshot_x screenshot_y screenshot_width screenshot_height image
    } ifelse

    % reset config option description
    description_x description_y moveto
    description_x description_y
    rot
    config_option_menu_entry.index.description get strsize
    image
  } {
    % menu type does not have a description
    pop
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% erases the currently shown menu from screen
%
% ( -- )
%
%
/menu.erase {
  % restore default background
  0 0 moveto
  0 0 image.size image

  % repaint system info
  % (this text is non-static, it changes after selecting another locale)
  system_info gettext
  system_info_font find_cached_file setfont
  0x000000 setcolor
  system_info_x system_info_y moveto
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
% gets an option by id
%
% ( string -- array )
%
% string: the option id
%
% array: the configuration
%
/get_option {
  options {
    dup option.index.id get
    2 index eq {
      % this is the option we were searching for
      % remove option id from stack
      exch pop
      return
    } {
      % this is another option...
      % just remove it from the stack
      pop
    } ifelse
  } forall
  "option id not found!" dtrace pop
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
    dup config_option.index.selected get {
      % selected option found, leave text on stack
      config_option.index.text get return
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
  dup menu.index.type get

  dup menu.type.start eq {
    pop
    % leave start menu text on stack
    startmenu.index.text get gettext
    return
  } if

  dup menu.type.config eq {
    pop

    % get configuration by id
    configmenu.index.id get get_configuration

    % get configuration text
    dup config.index.text get gettext

    % show currently selected option
    ": " concatenate_strings
    exch
    get_selected_option_text gettext
    concatenate_strings
    return
  } if

  dup menu.type.config_option eq {
    pop

    % get radio button
    dup config_option_menu_entry.index.selected get {
      % this is the selected option
      % add selected radiobutton prefix
      "◉ "
    } {
      % this is a nonselected option
      % add nonselected radiobutton prefix
      "○ "
    } ifelse

    % get config option menu text
    exch config_option_menu_entry.index.text get

    % concatenate radio button with text
    concatenate_strings
    return
  } if

  dup menu.type.option eq {
    pop

    % get option by id
    option_menu_entry.index.id get get_option

    % get checkbox
    dup option.index.selected get {
      % this is a selected option
      % add selected checkbox prefix
      "☑ "
    } {
      % this is a nonselected option
      % add nonselected checkbox prefix
      "☐ "
    } ifelse

    % get option text
    exch option.index.text get gettext

    % concatenate checkbox with text
    concatenate_strings
    return
  } if

  dup menu.type.menu eq {
    pop
    % get (sub)menu text
    menu.index.text get gettext
    breadcrumbs.delimiter concatenate_strings
    return
  } if

  menu.type.back eq {
    % get back entry text
    back.index.text get gettext
  } if
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% paints the breadcrumbs text on screen
%
% ( -- )
%
/paint_breadcrumbs {
  % assemble breadcrumbs text
  ""
  last_breadcrumb
  {
    % get breadcrumb menu
    dup breadcrumb.index.current_menu get

    % get menu text
    menu.index.text get gettext
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
    breadcrumb.index.previous get
    dup 0 eq {
      % this is the first breadcrumb
      pop
      exit
    } if
  } loop

  % paint breadcrumbs text
  breadcrumbs.color setcolor
  breadcrumbs.font find_cached_file setfont
  breadcrumbs.y exch show_centered
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% paints a menu on screen
%
% ( -- )
%
/paint_menu {

  menu.erase

  paint_breadcrumbs

  % get menu specific layout values
  /menu.y          current.menu menu.index.y          get def
  /menu.dy         current.menu menu.index.dy         get def
  /menu.icon_inset current.menu menu.index.icon_inset get def
  /menu.font       current.menu menu.index.font       get def

  % paint all menu entries
  menu.color setcolor
  /y_offset menu.y def
  menu.font find_cached_file setfont

  menu.alignment menu.alignment.center eq {
    % center all texts
    current.menu menu.index.entries get {
      % TODO: icon support
      menu_to_text
      y_offset exch show_centered
      /y_offset y_offset menu.dy add def
    } forall
  } {
    % left align all texts
    update_left_align_x
    current.menu menu.index.entries get {

      % text
      dup menu_to_text

      % indention
      /indention 0 def

      % paint icon
      % determine index of icon element in this menu entry
      exch dup menu.index.type get
      dup menu.type.start eq {
        pop
        startmenu.index.icon
      } {
        dup menu.type.config eq {
          pop
          dup configmenu.index.indention get
          /indention exch def
          configmenu.index.icon
        } {
          dup menu.type.menu eq {
            pop
            dup menu.index.indention get
            /indention exch def
            menu.index.icon
          } {
            dup menu.type.back eq {
              pop
              back.index.icon
            } {
              pop
              % menu type is not (yet) supported
              -1
            } ifelse
          } ifelse
        } ifelse
      } ifelse
      dup -1 eq {
        % menu type is not (yet) supported
        % remove index (-1) from stack
        pop
        % remove menu entry from stack
        pop
      } {
        % supported menu type
        get
        dup "" eq {
          % file could not be found
          pop
        } {
          % set the image (temporarily) to the icon file
          find_cached_file setimage
          % get text height
          dup strsize exch pop 7 mul 10 div
          % get icon height
          image.size exch pop
          % get offset
          sub
          % move to a good position
          left_align_x indention add y_offset moveto
          image.size pop menu.icon_inset add neg exch rmoveto
          % paint icon
          0 0 image.size image
          % reset image to default background
          default_image setimage
        } ifelse
      } ifelse

      % paint text
      left_align_x indention add y_offset moveto show
      /y_offset y_offset menu.dy add def
    } forall
  } ifelse

  % handle edit input field
  current.menu menu.index.show_edit get {
    % show prompt
    current.menu menu.index.edit_x get
    current.menu menu.index.edit_y get
    moveto
    current.menu menu.index.edit_prompt get gettext
    show

    % show editable input line
    edit_obj free
    /edit_obj [
      currentpoint exch 10 add exch                                 % x,y
      current.menu menu.index.edit_width get fontheight savescreen  % background image, defines input field size
      edit_input_buffer                                             % input buffer
      dup cvp length 1 sub                                          % buffer size
      .undef                                                        % for internal use
    ] def
    0x000000 setcolor
    edit_obj edit_input_buffer edit.init
  } {
    % hide editable input line
    edit_obj 0 0 settype ne {
      edit_obj edit.done
    } if
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
  current.menu menu.index.entries get {
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
