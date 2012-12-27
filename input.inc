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

  % put scan code into the upper 8 bits
  dup 0xFF00 and 16 shl
  % ignore special prefix 'E0' (used e.g. for the arrow keys)
  % see http://en.wikipedia.org/wiki/Scancode for more details
  over 0xFF and dup 0xE0 eq { pop 0 } if add
  /key exch def
  % 'key' now holds the ASCII code for 'normal' keys and the scan code in the
  % upper 8 bits if there is no ASCII code (e.g. F-keys, cursor keys)

  % store status bits (ctrl, shift...)
  16 shr 0xFFFF and
  /key.status exch def

  % map key (needs the scan code!)
  key config.keymap .km.map get { mapkey } if
  % remove the scan code if there is normal ACSII code
  dup 0xFF and { 0xFF and } if
  /key exch def

  % process key events...
  key keyDown eq {
    menu.selected_index current.menu menu.index.entries get length 1 sub lt {
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

  % enter or timeout
  key keyEnter eq key 0 eq or {
    current.menu menu.index.entries get menu.selected_index get
    % check menu entry type
    dup menu.index.type get
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

  % use key for editable input line (if available)
  edit_obj 0 0 settype ne {
    0x000000 setcolor
    edit_obj key edit.input
  } if

  "" 0 0
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Test for AltGr.
%
% (  -- bool )
%
/is_altGr {
  key.status statusAltR and 0 ne key.status statusAltL and 0 eq and
} def


% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Keyboard mapping.
%
% ( key -- key )
%
/mapkey {
   dup 24 shr 0xff and /key.code exch def
   is_altGr {
     % bios is too smart...
     key.code 0x78 ge key.code 0x83 le and { /key.code key.code 0x76 sub def } if
   } if
   0 1 config.keymap .km.map get length 1 sub {
     config.keymap .km.map get exch get
     dup 0 get key.code eq {
       1
       key.status statusShift and { pop 2 } if
       is_altGr { pop 3 } if
       get
       exch pop
     } {
       pop
     } ifelse
   } for
} def

