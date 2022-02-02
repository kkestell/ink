#include "types.h"

struct spinlock;

// console.c
void            consoleinit(void);
void            consputc(char);

// kalloc.c
char            *kalloc(void);
void            kfree(char *);
void            kinit1(efi_boot_payload_t *boot);
void            kinit2(void*, void*);

// uart.c
void            uartinit(void);
void            uartputc(char a);
char            uartgetc(void);

// spinlock.c
void            acquire(struct spinlock*);
void            getcallerpcs(void*, uintp*);
void            getstackpcs(uintp*, uintp*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);
void            pushcli(void);
void            popcli(void);

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);

// printf.c
void            printf(char*, ...);
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);

// vm.c
// void            seginit(void);
void            kvmalloc(void);
// void            vmenable(void);
// pde_t*          setupkvm(void);
// char*           uva2ka(pde_t*, char*);
// int             allocuvm(pde_t*, uint, uint);
// int             deallocuvm(pde_t*, uintp, uintp);
// void            freevm(pde_t*);
// void            inituvm(pde_t*, char*, uint);
// int             loaduvm(pde_t*, char*, struct inode*, uint, uint);
// pde_t*          copyuvm(pde_t*, uint);
// void            switchuvm(struct proc*);
// void            switchkvm(void);
// int             copyout(pde_t*, uint, void*, uint);
// void            clearpteu(pde_t *pgdir, char *uva);