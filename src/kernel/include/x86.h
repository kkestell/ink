#pragma once

#include "types.h"

static inline uint8
inb(uint16 port)
{
    uint8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void
outb(uint16 port, uint8 val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void
cli(void)
{
  __asm__ volatile("cli");
}

static inline void
stosb(void *addr, int data, int cnt)
{
  __asm__ volatile("cld; rep stosb" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

static inline void
stosl(void *addr, int data, int cnt)
{
  __asm__ volatile("cld; rep stosl" :
               "=D" (addr), "=c" (cnt) :
               "0" (addr), "1" (cnt), "a" (data) :
               "memory", "cc");
}

struct gatedesc;

static inline void
lidt(struct gatedesc *p, int size)
{
  volatile ushort pd[5];

  pd[0] = size-1;
  pd[1] = (uintp)p;
  pd[2] = (uintp)p >> 16;
  pd[3] = (uintp)p >> 32;
  pd[4] = (uintp)p >> 48;
  __asm__ volatile("lidt (%0)" : : "r" (pd));
}

static inline uint
xchg(volatile uint *addr, uintp newval)
{
  uint result;
  
  // The + in "+m" denotes a read-modify-write operand.
  __asm__ volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" ((uint)newval) :
               "cc");
  return result;
}

static inline uintp
readeflags(void)
{
  uintp eflags;
  __asm__ volatile("pushf; pop %0" : "=r" (eflags));
  return eflags;
}

static inline void
sti(void)
{
  __asm__ volatile("sti");
}
