#ifndef DOOMGENERIC_LIBC
#define DOOMGENERIC_LIBC

// Overridable "libc" style functions

typedef int (*DG_putchar_t)(int ch);
extern DG_putchar_t DG_putchar;

typedef int (*DG_puts_t)(char const* str);
extern DG_puts_t DG_puts;

typedef int (*DG_printf_t)(char const* format, ...);
extern DG_printf_t DG_printf;

#endif
