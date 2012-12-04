% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Input event handling.
%
% ( key -- input_buffer menu_entry action )
%
% key
%  bit 0-7      ascii
%  bit 8-15     scan code
%  bit 16-32    status bits (ctrl, shift...)
%
% action
%    0:         ok, stay in input loop
%    1:         switch to text mode (that is, continue with the non-gfxboot bootloader interface)
%  >=2:         boot
%
% Notes:
%  - key = 0 indicates the bootloader timeout is up.
%  - input_buffer is the command line that you would have normally entered in the bootloader.
%    note that for syslinux it must start with the menu label string but not for grub
%  - menu_entry is the number of the menu entry you want to boot
%
/KeyEvent {
  % pre-process keycode a bit
  dup 0xff00 and 16 shl exch 0xff and dup 0xe0 eq { pop 0 } if add
  dup 0xff and { 0xff and } if
  /key exch def

  % 'key' now holds the ASCII code for 'normal' keys and the scan code in the
  % upper 8 bits if there is no ASCII code (e.g. F-keys, cursor keys)

  % process key events...
  key keyDown eq {
    menu.selected_index menu.entries menu.index.entries get length 1 sub lt {
      menu.selected_index menu.deselect
      /menu.selected_index menu.selected_index 1 add def
      menu.selected_index menu.select
    } if
  } if

  key keyUp eq {
    menu.selected_index 0 gt {
      menu.selected_index menu.deselect
      /menu.selected_index menu.selected_index 1 sub def
      menu.selected_index menu.select
    } if
  } if

  key keyEnter eq {
    menu.entries menu.index.entries get menu.selected_index get
    % check menu entry type
    dup 0 get
    dup menu.type.start eq {
      pop
      handle_start_menu_entry
      return
    } {
      dup menu.type.config eq {
        pop
        handle_config_menu_entry
      } {
        dup menu.type.config_option eq {
          pop
          handle_config_option_menu_entry
        } {
          dup menu.type.option eq {
            pop
            handle_option_menu_entry
          } {
            dup menu.type.menu eq {
              pop
              handle_submenu_entry
            } {
              menu.type.back eq {
                handle_back_entry
              } if
            } ifelse
          } ifelse
        } ifelse
     } ifelse
    } ifelse
  } if

 "" 0 0
} def
