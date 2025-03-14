#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdint>
#include <stdint.h>

/* https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description
   https://chip8.gulrak.net/
*/


namespace chipp8 {

struct chip8 {
    std::bitset<64 /*width*/ * 32 /*height*/ > pixels;

    // Interpreter occupies the first 512 bytes so most
    // programs begin at memory location 512 (0x200).
    //
    // The uppermost 256 bytes (0xF00-0xFFF) are reserved
    // for display refresh
    //
    // The 96 bytes below that (0xEA0-0xEFF) were reserved for
    // the call stack, internal use, and other variables
    //
    // In modern CHIP-8 impls, where the interpreter is running
    // outside the 4k memory, there is no need to avoid the lower 512 bytes
    // of memory and it is common to store font data there
    std::array<uint8_t, 4096> mem;

    // v0-vFs
    // The VF register doubles as a flag for some instructions
    // and this, it should be avoided
    //
    // In an addition operation, VF is the carry flag, while in
    // subtraction, it is the "no borrow" flag
    //
    // In the draw instruction VF is set upon pixel collision
    std::array<uint8_t, 16> v;

    // The address register is 12 bits wide
    // This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
    uint16_t i;

    // Timers Decrement at a rate of 60 Hz
    // Delay Timer
    uint8_t d_timer;

    // Sound Timer
    uint8_t s_timer;

    // program counter
    uint16_t pc;

    // stack pointer
    uint8_t sp;

    std::array<uint16_t, 16> stack;
};

inline void init(chip8& cpu) {
    cpu.pixels.reset();
    cpu.mem.fill(0);
    cpu.v.fill(0);
    cpu.i = 0;
    cpu.d_timer = 0;
    cpu.s_timer = 0;
    cpu.pc = 0;
    cpu.sp = 0;
    cpu.stack.fill(0);
}

inline uint16_t pop_stack(chip8& cpu) {
    // Take and then decrement
    return cpu.stack[--(cpu.sp)];
}

inline void push_stack(chip8& cpu, uint16_t val) {
    // Increment then add
    cpu.sp++;
    cpu.stack[cpu.sp] = val;
}

// 00E0  - clear the screen
inline void CLS(chip8& cpu) {
    cpu.pixels.reset();
}

// 00EE - return from subroutine to address pulled from stack
inline void RET(chip8& cpu) {
    cpu.pc = pop_stack(cpu);
}

// 0NNN - jump to native assembler subroutine at 0xNNN
inline void SYS(chip8& cpu, uint16_t addr)
{
    cpu.pc = addr;
}

// 1NNN - jump to address NNN
inline void JP(chip8& cpu, uint16_t addr)
{
    cpu.pc = addr;
}

// 2NNN - push return address onto stack and call subroutine at address NNN
inline void CALL(chip8& cpu, uint16_t addr)
{
    push_stack(cpu, cpu.pc);
    cpu.pc = addr;
}

// 3XNN - skip next opcode if vX == NN
inline void SE(chip8& cpu, uint8_t /*V*/x, uint8_t nn)
{
    if (cpu.v[x] == nn) {
        cpu.pc += 2;
    }
}

// 4XNN - skip next opcode if vX != NN
inline void SNE(chip8& cpu, uint8_t /*V*/x, uint8_t nn)
{
    if (cpu.v[x] != nn) {
        cpu.pc += 2;
    }
}

// 5XY0 - skip next opcode if vX == vY
inline void SE_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y)
{
    if (cpu.v[x] == cpu.v[y]) {
        cpu.pc += 2;
    }
}

// 6XNN - set vX to NN
inline void LD(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    cpu.v[x] = nn;
}

// 7XNN - add NN to vX
// NOTE: The carry flag is not changed
inline void ADD(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    cpu.v[x] += nn;
}

// 8XY0 - set vX to the value of vY
inline void LD_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = cpu.v[y];
}

// 8XY1 - set vX to the result of bitwise vX OR vY
inline void OR_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = (cpu.v[x] | cpu.v[y]);
}

// 8XY2 - set vX to the result of bitwise vX AND vY
inline void AND_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = (cpu.v[x] & cpu.v[y]);
}

// 8XY3 - set vX to the result of bitwise vX XOR vY
inline void XOR_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = (cpu.v[x] ^ cpu.v[y]);
}

// 8XY4 - add vY to vX, vF is set to 1 if an overflow happened, to 0 if not, even if X=F!
inline void ADD_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    uint16_t sum = cpu.v[x] + cpu.v[y];
    if (sum > 255u) {
        cpu.v[0xFu] = 1u;
    } else {
        cpu.v[0xFu] = 0u;
    }
    cpu.v[x] = static_cast<uint8_t>(sum & 0x00FFu);
}

bool parse_op(chip8& cpu, uint16_t instruct) {
    switch (instruct & 0xF000u) {
        case 0x0000u: {
            switch (instruct & 0x0FFFu) {
                case 0x00E0u: { CLS(cpu); } break;
                case 0x00EEu: { RET(cpu); } break;
                default: { SYS(cpu, instruct & 0x0FFFu); } break;
            }
        } break;

        case 0x1000u: { JP(cpu, instruct & 0x0FFFu); } break;
        case 0x2000u: { CALL(cpu, instruct & 0x0FFFu); } break;
        case 0x3000u: {
            SE(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x4000u: {
            SNE(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x5000u: {
            SE_REG(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
        } break;

        case 0x6000u: {
            LD(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x7000u: {
            ADD(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x8000u: {
            switch (instruct & 0x000Fu) {
                case 0x0000u: {
                    LD_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0001u: {
                    OR_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0002u: {
                    AND_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0003u: {
                    XOR_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0) >> 4u));
                } break;

                case 0x0004u: {
                    ADD_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0005u: {
                    // 8XY5 - subtract vY from vX, vF is set to 0 if an underflow happened, to 1 if not, even if X=F!

                } break;

                case 0x0006u: {
                    // 8XY6 - set vX to vY and shift vX one bit to the right, set vF to the bit shifted out, even if X=F!

                } break;

                case 0x0007u: {
                    // 8XY7 - set vX to the result of subtracting vX from vY, vF is set to 0 if an underflow happened, to 1 if not, even if X=F!

                } break;

                case 0x000Eu: {
                    // 8XYE - set vX to vY and shift vX one bit to the left, set vF to the bit shifted out, even if X=F!

                } break;

            }

        } break;

        case 0x9000u: {
            // 9XY0 - skip next opcode if vX != vY

        } break;

        case 0xA000u: {
            // ANNN - set I to NNN

        } break;

        case 0xB000u: {
            // BNNN - jump to address NNN + v0

        } break;

        case 0xC000u: {
            // CXNN - set vx to a random value masked (bitwise AND) (Typically: 0 to 255) with NN

        } break;

        case 0xD000u: {
            // DXYN - draw 8xN pixel sprite at position vX, vY with data starting at the address in I, I is not changed
            // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen.

        } break;

        case 0xE000u: {
            switch (instruct & 0x00FFu) {
                case 0x009Eu: {
                    // EX9E - skip next opcode if key in the lower 4 bits of vX is pressed

                } break;

                case 0x00A1u: {
                    // EXA1 - skip next opcode if key in the lower 4 bits of vX is not pressed

                } break;
            }

        } break;

        case 0xF000u: {
            switch (instruct & 0x00FFu) {
                case 0x0007u: {
                    // FX07 - set vX to the value of the delay timer
                } break;

                case 0x000Au: {
                    // FX0A - wait for a key pressed and released and set vX to it
                } break;

                case 0x0015u: {
                    // FX15 - set delay timer to vX
                } break;

                case 0x0018u: {
                    // FX18 - set sound timer to vX, sound is played as long as the sound timer reaches zero
                } break;

                case 0x001Eu: {
                    // FX1E - add vX to I
                } break;

                case 0x0029u: {
                    // FX29 - set I to the 5 line high hex sprite for the lowest nibble in vX
                } break;

                case 0x0033u: {
                    // FX33 - write the value of vX as BCD value at the addresses I, I+1 and I+2
                } break;

                case 0x0055u: {
                    // FX55 - write the content of v0 to vX at the memory pointed to by I, I is incremented by X+1
                } break;

                case 0x0065u: {
                    // FX65- read the bytes from memory pointed to by I into the registers v0 to vX, I is incremented by X+1
                } break;
            }

        } break;

        default: {

        } break;
    }

    return false;
}

} // namespace chipp8
