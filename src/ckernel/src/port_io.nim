import
  port_io

proc inb*(port: uint16_t): uint8_t =
  var ret: uint8_t
  ## !!!Ignored construct:  __asm__ volatile ( inb %1, %0 : =a ( ret ) : Nd ( port ) ) ;
  ## Error: identifier expected, but got: inb %1, %0!!!
  return ret

proc outb*(port: uint16_t; val: uint8_t) =
  ## !!!Ignored construct:  __asm__ volatile ( outb %0, %1 : : a ( val ) , Nd ( port ) ) ;
  ## Error: identifier expected, but got: outb %0, %1!!!
  