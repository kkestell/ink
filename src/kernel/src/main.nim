import
  framebuffer,
  platform

initPlatform()
clearScreen()

puts("Hello World\n")

proc foo() =
  var ints: seq[int]

  for i in countup(1, 100):
    ints.add(i)
    puts(i.repr.cstring)

foo()

puts("\nGoodbye World!\n")

while true:
  discard
