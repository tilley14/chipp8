#include "chip8.h"

int main(int argc, char* argv[]) {
    using namespace chipp8;
    chip8 cpu;
    init(cpu);

    // What is the clock rate?
    // TODO: emulate a clock source
    while (true) {
        // check for user input
        // cpu get current instruction
        // execute instruction
        // increment pc
        // draw display
    }

    return 0;
}
