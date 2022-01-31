import
  base,
  boot,
  kalloc,
  uart

let cfg = getBootConfig()

uartInit()
kallocInit(cfg.memory)

proc fib(n: int): uint64 =
  if n > 2 : return fib(n - 1) + fib(n - 2)
  return cast[uint64](n)

proc fibs() =
  var ints: seq[uint64]

  for i in 1..10:
    ints.add(fib(i))
  
  for f in ints:
    printf("%U\n", f)

fibs()

while true:
  discard
