#pragma once

typedef int jmp_buf[3]; // EBP, EIP, ESP

#define setjmp(env) __builtin_setjmp(env)
#define longjmp(env, val) __builtin_longjmp(env, val)
