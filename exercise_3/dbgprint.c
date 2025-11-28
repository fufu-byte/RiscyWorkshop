#include <stdint.h>

static __attribute((noinline)) void print_string(const char* str)
{
    uint64_t len = 0;
    while (str[len])
        ++len;

    register uint64_t    a0 asm("a0") = 1;   // fd = stdout
    register const char* a1 asm("a1") = str; // buf
    register uint64_t    a2 asm("a2") = len; // count
    register uint64_t    a7 asm("a7") = 64;  // syscall: write (RISC-V Linux)
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory");
}

static __attribute((noinline)) uint64_t exit(int exit_code)
{
    register uint64_t a0 asm("a0") = exit_code;
    register uint64_t a1 asm("a1") = 0; // unused
    register uint64_t a7 asm("a7") = 10000;
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a7) : "memory");
    return a0;
}

void _start() __attribute__((section(".text.start")));

void _start()
{
    print_string("Hello from RISC-V!");
    exit(0);
    asm volatile("ebreak");
}
