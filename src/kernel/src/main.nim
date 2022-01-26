import
  boot_info,
  framebuffer,
  platform

initUart()

let info = getBootInfo()

initMemory(info.memory)

initFramebuffer(info.framebuffer)
initConsole(info.framebuffer)

clearScreen()
puts("The quick brown fox jumps over the lazy dog.\nHello World\nGoodbye World!\n")

proc foo() =
  var ints: seq[int]

  for i in countup(1, 100):
    ints.add(i)
    puts(i.repr.cstring)
  
  debugMemory()

foo()

debugMemory()

while true:
  discard
