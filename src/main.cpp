#include "chip8.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    using namespace chipp8;
    chip8 cpu;
    init(cpu);
    load_font_sprites(cpu);

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
