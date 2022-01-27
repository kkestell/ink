import
  console,
  framebuffer,
  platform

initPlatform()

let fb = getFramebuffer()
fb.clearScreen()

puts "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\n"

proc count() =
  var ints: seq[int]

  for i in 1..1_000_000:
    ints.add(i)

proc fib(n: uint64): uint64 =
  if n > 2.uint64 : return fib(n - 1) + fib(n - 2)
  return n

puts "Counting to 1,000,000\n"
count()

puts "Calculating fib(46)\n"
puts fib(46).repr.cstring

while true:
  discard
