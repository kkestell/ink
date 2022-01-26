import
  boot_info,
  framebuffer,
  platform

initUart()

let info = getBootInfo()

initMemory(info.memory)
initFramebuffer(info.framebuffer)

clearScreen()

proc foo() =
  var ints: seq[int]

  for i in countup(1, 100000):
    ints.add(i)
  
  debugMemory()

foo()

debugMemory()

while true:
  discard
