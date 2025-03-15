#include "chip8.h"

#include "utils.h"

int main(int argc, char* argv[]) {
    using namespace chipp8;
    chip8 cpu;
    init(cpu);

    for (const auto &a : {F_0, F_1, F_2, F_3, F_4, F_5, F_6, F_7, F_8, F_9, F_A, F_B, F_C, F_D, F_E, F_F})
        utils::pp_array(a);

    utils::pp_display(cpu.pixels, 64u, 32u);

    // What is the clock rate?
    // TODO: emulate a clock source
    while (false /*true */) {
        // check for user input
        // cpu get current instruction
        // execute instruction
        // increment pc
        // draw display
    }

    return 0;
}
