:noname
  ."   Type 'help' for detailed information" cr
  \ ."   boot secondary slave cdrom: " cr
  \ ."    0 >  boot hd:2,\boot\vmlinuz root=/dev/hda2" cr
  ; DIAG-initializer

: make-openable ( path )
  find-dev if
    begin ?dup while
      \ install trivial open and close methods
      dup active-package! is-open
      parent
    repeat
  then
;

: preopen ( chosen-str node-path )
  2dup make-openable

  " /chosen" find-device
  open-dev ?dup if
    encode-int 2swap property
  else
    2drop
  then
;

:noname
  set-defaults
; SYSTEM-initializer

\ preopen device nodes (and store the ihandles under /chosen)
:noname
  " memory" " /memory" preopen

; SYSTEM-initializer

\ use the tty interface if available
:noname
  " /builtin/console" find-dev if drop
    " /builtin/console" " input-device" $setenv
    " /builtin/console" " output-device" $setenv
  then
; SYSTEM-initializer

:noname
  " keyboard" input
; CONSOLE-IN-initializer

device-end

: rmap@    ( virt -- rmentry )
  drop 0
  ;

