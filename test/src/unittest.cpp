#include "unittest.h"

#include <cassert>

#include "chip8.h"

#define ASSERT(condition, message) \
   do { \
      assert(condition && #message); \
   } while (0);


using namespace chipp8;

namespace test {

void test_pop_stack() {
    chip8 cpu;
    init(cpu);

    cpu.stack[0u] = 0u;
    cpu.stack[1u] = 1u;
    cpu.stack[2u] = 2u;
    cpu.stack[3u] = 3u;

    cpu.sp = 2;

    auto ret = pop_stack(cpu);

    ASSERT(ret == 2u, "The popped value is 2")
    ASSERT(cpu.sp == 1, "The stack pointer is now 1")
}

void test_push_stack() {
    chip8 cpu;
    init(cpu);

    cpu.stack[0u] = 0u;
    cpu.stack[1u] = 1u;
    cpu.stack[2u] = 2u;

    cpu.sp = 2;

    push_stack(cpu, 9u);

    ASSERT(cpu.sp == 3u, "The stack pointer is 3")
    ASSERT(cpu.stack[cpu.sp] == 9u, "The stack pointer now points to the value 9")
}

void run_tests() {
    test_pop_stack();
    test_push_stack();
}

} // namespace test
