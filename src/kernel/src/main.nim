import 
  boot_info,
  framebuffer,
  memory,
  uart

initUart()

let info = getBootInfo()

initMemory(info.memory)
initFramebuffer(info.framebuffer)

clearScreen()

proc foo() =
  var ints: seq[int]

  for i in countup(1, 100000):
    ints.add(i)

foo()

puts("Hello, World\n")

while true:
  discard
