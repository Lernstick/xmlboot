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

  % store key value
  /key exch def

  % put key ascii into separate value
  key 0xFF and
  % ignore special prefix 'E0' (used e.g. for the arrow keys)
  % see http://en.wikipedia.org/wiki/Scancode for more details
  dup 0xE0 eq {
    pop 0
  } if
  /key.ascii exch def

  % put key scan code into separate value
  key 0xFF00 and 8 shr
  /key.scan_code exch def

  % put key status bits into separate value
  key 16 shr 0xFFFF and
  /key.status exch def

  % paint key details on screen (only used for debugging...)
  % show_key_info

  % process key events

  key.scan_code scanDown eq {
    menu.selected_index current.menu menu.index.entries get length 1 sub lt {
      menu.selected_index menu.deselect
      /menu.selected_index menu.selected_index 1 add def
      menu.selected_index menu.select
    } if
    "" 0 0 return
  } if

  key.scan_code scanUp eq {
    menu.selected_index 0 gt {
      menu.selected_index menu.deselect
      /menu.selected_index menu.selected_index 1 sub def
      menu.selected_index menu.select
    } if
    "" 0 0 return
  } if

  % show memory info when user presses Ctrl+M
  key.scan_code 0x32 eq {
    key.status statusCtrl and {
      toggle_memory_info
      "" 0 0 return
    } if
  } if

  % enter or timeout
  key.ascii asciiEnter eq key 0 eq or {
    % check menu entry type
    current.menu menu.index.entries get menu.selected_index get
    dup menu.index.type get

    dup menu.type.start eq {
      pop
      handle_start_menu_entry
      return
    } if

    dup menu.type.config eq {
      pop
      handle_config_menu_entry
      "" 0 0 return
    } if

    dup menu.type.config_option eq {
      pop
      handle_config_option_menu_entry
      "" 0 0 return
    } if

    dup menu.type.option eq {
      pop
      handle_option_menu_entry
      "" 0 0 return
    } if

    dup menu.type.menu eq {
      pop
      handle_submenu_entry
      "" 0 0 return
    } if

    menu.type.back eq {
      handle_back_entry
      "" 0 0 return
    } if
  } if

  % use key for editable input line (if available)
  key.ascii mapkey
  /key.ascii exch def
  edit_obj .undef ne {
    0x000000 setcolor
    edit_obj key.ascii edit.input
  } if
  "" 0 0
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Test for AltGr.
%
% (  -- bool )
%
/is_altGr {
  key.status statusAltGr and 0 ne key.status statusAltL and 0 eq and
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Keyboard mapping.
%
% ( key -- key )
%
/mapkey {

   is_altGr {
     % BIOS is too smart...
     key.scan_code 0x78 ge key.scan_code 0x83 le and {
       /key.scan_code key.scan_code 0x76 sub def
     } if
   } if

   config.keymap {
     dup keymap.index.scan_code get key.scan_code eq {
       % scan code found in map

       % put mapped value on stack
       keymap.index.ascii
       key.status statusShift and { pop keymap.index.shift } if
       is_altGr { pop keymap.index.altgr } if
       get

       % remove unmapped key value from stack
       exch pop
       exit

     } {
       % another scan code
       pop
     } ifelse
   } forall
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Shows key info on screen
%
% ( -- )
%
/show_key_info {
  0x000000 setcolor
  0 0 moveto 300 120 fillrect
  0xFFFFFF setcolor
  /buf 100 string def
  key "key: 0x" "%s%x" buf sprintf
  buf show
  0 30 moveto
  key.status "key.status: 0x" "%s%x" buf sprintf
  buf show
  0 60 moveto
  key.scan_code "key.scan_code: 0x" "%s%x" buf sprintf
  buf show
  0 90 moveto
  key.ascii "key.ascii: 0x" "%s%x" buf sprintf
  buf show
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% Shows some memory info on screen
%
% ( -- )
%
/toggle_memory_info {
  memory_info_showing {
    % hide memory info
    /memory_info_showing false def
    paint_menu
    menu.selected_index menu.select
  } {
    % show memory info
    /memory_info_showing true def
    0x000000 setcolor
    0 0 moveto screen.size fillrect
    0xFFFFFF setcolor
    /buf 100 string def

    /biosmem sysconfig 20 add getdword def
    biosmem "biosmem: 0x%x" buf sprintf
    buf show

    0 30 moveto
    0 memsize
    "region 0: size of largest free block 0x%x, total free memory: 0x%x" buf sprintf
    buf show

    0 60 moveto
    1 memsize
    "region 1: size of largest free block 0x%x, total free memory: 0x%x" buf sprintf
    buf show

    0 90 moveto
    2 memsize
    "region 2: size of largest free block 0x%x, total free memory: 0x%x" buf sprintf
    buf show

    0 120 moveto
    3 memsize
    "region 3: size of largest free block 0x%x, total free memory: 0x%x" buf sprintf
    buf show
  } ifelse
} def
