#include <stdint.h>

static __attribute((noinline)) uint64_t resolve_import(const char* module, const char* function)
{
    register const char* a0 asm("a0") = module;   // module name
    register const char* a1 asm("a1") = function; // function name
    register uint64_t    a7 asm("a7") = 10105;    // syscall: resolve_import
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a7) : "memory");
    return (uint64_t)a0;
}

static __attribute((noinline)) uint64_t host_call(uint64_t fn, uint64_t args[13])
{
    register uint64_t  a0 asm("a0") = fn;            // function address
    register uint64_t* a1 asm("a1") = args;          // args array pointer
    register uint64_t  a7 asm("a7") = 20000;         // syscall: host_call
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a7) : "memory");
    return a0;
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
    // Resolve the puts function (module=0 means default/libc)
    uint64_t puts_fn = resolve_import(0, "puts");
    
    // Prepare arguments for puts call
    uint64_t args[13];
    args[0] = (uint64_t)"Hello from RISC-V!";
    for (int i = 1; i < 13; i++)
        args[i] = 0;
    
    // Call puts via host_call
    host_call(puts_fn, args);
    
    exit(0);
    asm volatile("ebreak");
}
