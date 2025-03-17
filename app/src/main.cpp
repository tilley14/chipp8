#include "chip8.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    using namespace chipp8;
    chip8 cpu;
    init(cpu);
    load_font_sprites(cpu);

    cpu.v[1] = 2;
    cpu.v[2] = 2;
    cpu.i = 0x50;

    DRW(cpu, 1, 2, 5);

    utils::pp_display(cpu.pixels, 64, 32);

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
